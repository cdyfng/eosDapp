#include <eosio/eosio.hpp>
//#include <eosio/asset.hpp>

using namespace eosio;

class [[eosio::contract("addressbook")]] addressbook : public eosio::contract {

public:
  using contract::contract;
  
  addressbook(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
  void upsert(name user, std::string first_name, std::string last_name, uint64_t age, std::string street, std::string city, std::string state) {
    require_auth( user );
    //address_index addresses(_code, _code.value);
    address_index addresses(get_first_receiver(), get_first_receiver().value);
    auto iterator = addresses.find(user.value);
    if( iterator == addresses.end() )
    {
      addresses.emplace(user, [&]( auto& row ) {
       row.key = user;
       row.first_name = first_name;
       row.last_name = last_name;
       row.age = age; 
       row.street = street;
       row.city = city;
       row.state = state;
      });
      send_summary(user, " successfully emplaced record to address book");
    }
    else {
      std::string changes;
      addresses.modify(iterator, user, [&]( auto& row ) {
        row.key = user;
        row.first_name = first_name;
        row.last_name = last_name;
        row.age = age; 
        row.street = street;
        row.city = city;
        row.state = state;
      });
      send_summary(user, " successfully modify record to address book");
    }
  }

  [[eosio::action]]
  void erase(name user) {
    require_auth(user);

    //address_index addresses(_self, _code.value);
    address_index addresses(get_first_receiver(), get_first_receiver().value);

    auto iterator = addresses.find(user.value);
    //eosio_assert(iterator != addresses.end(), "Record does not exist");
    check(iterator != addresses.end(), "Record does not exist");
    addresses.erase(iterator);
    send_summary(user, " successfully erased record from addressbook");
  }

  [[eosio::action]]
  void findbyage(name user, uint64_t age) {
   require_auth(user); 
   
   //address_index addresses(_self, _code.value);
   address_index addresses(get_first_receiver(), get_first_receiver().value);
   
   auto age_index = addresses.get_index<"byage"_n>();
   auto itr = age_index.find(age);
   //eosio_assert(itr != age_index.end(), "Yes, we have someone with that age");
   check(itr != age_index.end(), "Yes, we have someone with that age");
  }

private:
  struct [[eosio::table]] person {
    name key;
    std::string first_name;
    std::string last_name;
    uint64_t age;
    std::string street;
    std::string city;
    std::string state;
    uint64_t primary_key() const { return key.value; }
    uint64_t get_secondary_1() const { return age;}
  };

  void send_summary(name user, std::string messgae){
    action(
      permission_level{get_self(), "active"_n},
      get_self(),
      "notify"_n,
      std::make_tuple(user, name{user}.to_string() + messgae)
      ).send();
  }
  //typedef eosio::multi_index<"people"_n, person> address_index;
  typedef eosio::multi_index<"peopletable"_n, person, indexed_by<"byage"_n, const_mem_fun<person, uint64_t, &person::get_secondary_1>>> address_index;

};

EOSIO_DISPATCH( addressbook, (upsert)(erase)(findbyage))
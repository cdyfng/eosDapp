#include "Players.hpp"

namespace Chainpeer {
    using namespace eosio;
    using std::string;
    
    class Players : public contract {
    	using contract::contract;

    public:
    	//@abi action
    	void add(const account_name account, string& username, uint64_t level){
    		//require_auth(account);

    		playerIndex players(_self, _self);

    		auto iterator = players.find(account);
    		eosio_assert(iterator == players.end(), "Address for account already exists");

    		players.emplace(account, [&](auto& player){
    			player.account_name = account;
    			player.username = username;
    			player.level = 1;
    			player.health_points = 1000;
    			player.energy_points = 1000;
    		});
    	}

	   	//@abi action
    	void update(const account_name account, uint64_t level, uint64_t healthPoints, uint64_t energyPoints){

    		//require_auth(account);

    		playerIndex players(_self, _self);

    		auto iterator = players.find(account);
    		eosio_assert(iterator != players.end(), "Address for account not found");

    		players.modify(iterator, account, [&](auto& player){
    			player.level = level;

    			if(player.health_points - healthPoints < 0){
    				player.health_points = 0;
    			}else{
    				player.health_points -= healthPoints;
    			}

    			if(player.energy_points - energyPoints < 0){
    				player.energy_points = 0;
    			}else{
    				player.energy_points -= energyPoints;
    			}
    		});
    	}
 
    	//@abi action
    	void getplayer(const account_name account){
    		playerIndex players(_self, _self);

    		auto iterator = players.find(account);
    		eosio_assert(iterator != players.end(), "Address for account not found");

    		auto currentPlayer = players.get(account);
    		print("Username: ", currentPlayer.username.c_str(), " Level: ", currentPlayer.level, " Health: ", currentPlayer.health_points, " Energy: ", currentPlayer.energy_points);

    	}


    private:

    	//@abi table player i64
    	struct player{
    		uint64_t account_name;
    		string username;
    		uint64_t level;
    		uint64_t health_points = 1000;
    		uint64_t energy_points = 1000;

    		uint64_t primary_key() const { return account_name;}
    		
    		EOSLIB_SERIALIZE(player, (account_name)(username)(level)(health_points)(energy_points))
    	};

    	typedef multi_index<N(player), player> playerIndex;
     

    };
    EOSIO_ABI(Players, (add)(update)(getplayer))
};	

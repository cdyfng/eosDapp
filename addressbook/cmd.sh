eosname1=`sed '/^eosname1=/!d;s/.*=//' config`
eosname2=`sed '/^eosname2=/!d;s/.*=//' config`
pswd=`sed '/^pswd=/!d;s/.*=//' config`
echo $eosname1
echo $eosname2
echo $pswd

cleos wallet unlock -n main --password $(cat $pswd)

cleos -u 'https://api.eosnewyork.io' set contract $eosname1  ../addressbook -p $eosname1@active
echo 'set contract ok'
sleep 1
cleos -u 'https://api.eosnewyork.io' get table $eosname1  $eosname1 peopletable

sleep 1s&&cleos -u 'https://api.eosnewyork.io'  push action $eosname1 upsert '["'$eosname1'", "alice1", "liddell1", 9, "street none", "123 drink me way", "wonderland", "amsterdam"]' -p $eosname1@active
sleep 1s&&cleos -u 'https://api.eosnewyork.io' get table $eosname1  $eosname1 peopletable

cleos -u 'https://api.eosnewyork.io'  push action $eosname1 findbyage '["'$eosname1'", 19]' -p $eosname1@active

sleep 1s
cleos -u 'https://api.eosnewyork.io'  push action $eosname1  findbyage '["'$eosname1'", 9]' -p $eosname1@active
sleep 10s
cleos -u 'https://api.eosnewyork.io'  push action $eosname1  erase '["'$eosname1'"]' -p $eosname1@active
sleep 1s&&cleos -u 'https://api.eosnewyork.io' get table $eosname1  $eosname1 peopletable

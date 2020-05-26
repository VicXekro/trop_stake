#include <tropiumstake.hpp>

ACTION tropiumstake::regstaker (name username){
  require_auth(username);
  auto itr = _staker_list.find(username.value);

  check(itr == _staker_list.end(), "You are already registered, you can stake your TRPM");

  asset temp_asset(0, tropium_symb);
  itr = _staker_list.emplace(username, [&](auto& row){
    row.username = username;
    row.fund_staked = temp_asset;
    row.isstaked = false;
  });

}

void tropiumstake::stake(name username, name receiver, asset quantity, string msg){
  require_auth(username);
  if (receiver != get_self() || username == get_self()) return;
  auto itr = _staker_list.find(username.value);

  check(itr != _staker_list.end(), "You cannot stake, you are not yet registered");
  check(quantity.symbol == tropium_symb, "wrong token used");
  check(msg == "increment" || msg =="start", "Please use \"increment\" to increase your stake or \"start\" to deposit your first stake");
  
  if(msg == "start" && itr->isstaked == false){
  check(quantity.amount >= 5000, "Staked amount not enough, stake at least 5000");
    _staker_list.modify(itr, username, [&](auto& row){
      row.fund_staked = quantity;
      row.isstaked = true;
    });
  }
  else if (msg == "increment" && itr->isstaked == true){
    _staker_list.modify(itr, username, [&](auto& row){
      row.fund_staked += quantity;
    });
  }
  else check(false, "Error with staking options, please check you status");

  require_recipient(username);
}
/*
void tropiumstake::regstake(name username, name receiver, asset quantity, string memo) {
  // enforce user authentication
  if (receiver != get_self() || username == get_self()) return;
  require_auth(username);
  // set iterator on table
  auto itr = _staker_list.find(username.value);
  
  //check if not already in staker list
  check(itr == _staker_list.end(), "You are already a staker, check you staked amount instead");
  
  //check if quantity given is at least the minimum required and required currency
  check(quantity.amount >= 5000, "Staked amount not enough, stake at least 5000");
  check(quantity.symbol == tropium_symb, "wrong token used");

  //TODO: to modify syntax below to check with tropium_symb
  //check(quantity.symbol == hodl_symbol, "These are not the droids you are looking for.");
  
  //register user in staking table
  itr = _staker_list.emplace(username, [&](auto& row){
    row.username = username;
    row.fund_staked = quantity;
  });
  //require_recipient(username);
}

void tropiumstake::incrstake(name username, name receiver, asset quantity, string memo){
  //enforce user authentication
  if (receiver != get_self() || username == get_self()) return;
  require_auth(username);

  // set iterator on table
  auto itr = _staker_list.find(username.value);

 //check if not already in staker list
  check(itr != _staker_list.end(), "Your are not a staker, please register first");

  //check if quantity given is valid
  check(quantity.is_valid() && (quantity.symbol == tropium_symb), "Given amount is not valid");

  //increment amount stake
   _staker_list.modify(itr, username, [&]( auto& row ) {
      row.username = username;
      row.fund_staked += quantity;
    });

}*/

ACTION tropiumstake::unstake (name username){
  require_auth(username);
  auto itr = _staker_list.find(username.value);
  check(itr != _staker_list.end(), "Your are not a staker, please register first");
  print(get_self());

  //TODO: Chance contract name to eosio.token during deployment
  //token::transfer_action payout("test1"_n, {get_self(), "active"_n});
  //payout.send(get_self(), username, itr->fund_staked, "your funds have been released");

  action{
        permission_level{get_self(), "active"_n},
        "test1"_n,
        "transfer"_n,
        std::make_tuple(get_self(), username, itr->fund_staked, std::string("Your fund have been released"))
      }.send();
}
//_staker_list.erase(itr);
//std::make_tuple(get_self(), username, itr->fund_staked, std::string("Your fund have been released"))

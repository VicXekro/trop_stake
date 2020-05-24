#include <tropiumstake.hpp>

void tropiumstake::regstake(name username, name receiver, asset quantity, string memo) {
  // enforce user authentication
  if (receiver != get_self() || username == get_self()) return;
  require_auth(username);
  // set iterator on table
  auto itr = _staker_list.find(username.value);
  
  //check if not already in staker list
  check(itr == _staker_list.end(), "You are already a staker, check you staked amount instead");
  
  //check if quantity given is at least the minimum required
  check(quantity.amount >= 5000, "Staked amount not enough, stake at least 5000");

  //TODO: to modify syntax below to check with tropium_symb
  //check(quantity.symbol == hodl_symbol, "These are not the droids you are looking for.");
  
  //register user in staking table
  itr = _staker_list.emplace(username, [&](auto& row){
    row.username = username;
    row.fund_staked = quantity;
  });
}

void tropiumstake::incrstake(name username, name receiver, asset quantity, string memo){
  //enforce user authentication
  if (receiver != get_self() || username == get_self()) return;
  require_auth(username);

  // set iterator on table
  auto itr = _staker_list.find(username.value);

 //check if not already in staker list
  check(itr != _staker_list.end(), "Your are not a staker, please register first");

  //increment amount stake
   _staker_list.modify(itr, username, [&]( auto& row ) {
      row.username = username;
      row.fund_staked += quantity;
    });

}

ACTION tropiumstake:: unstake (name username){
  //enforce user authentication
  require_auth(username);

   // set iterator on table
  auto itr = _staker_list.find(username.value);

  //check if not already in staker list
  check(itr != _staker_list.end(), "Your are not a staker, please register first");

  action{
        permission_level{get_self(), "active"_n},
        "eosio.token"_n,
        "transfer"_n,
        std::make_tuple(get_self(), username, itr->fund_staked, std::string("Your fund have been released"))
      }.send();
}

EOSIO_DISPATCH(tropiumstake, (regstake)(incrstake)(unstake))
/*ACTION tropiumstake::clear() {
  require_auth(get_self());

  messages_table _messages(get_self(), get_self().value);

  // Delete all records in _messages table
  auto msg_itr = _messages.begin();
  while (msg_itr != _messages.end()) {
    msg_itr = _messages.erase(msg_itr);
  }
}

EOSIO_DISPATCH(tropiumstake, (hi)(clear))*/

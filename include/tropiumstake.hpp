/*
Author: Victor
Github: https://github.com/VicXekro
*/

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>

using namespace std;
using namespace eosio;

CONTRACT tropiumstake : public contract {
  public:
    using contract::contract;

     tropiumstake(name receiver, name code, datastream<const char*> ds):
        contract(receiver, code, ds),
        _staker_list(receiver, receiver.value){}

    /*
      Register the user and stake its amounts
      --> if user already registered, send error message
    */
    [[eosio::on_notify("eosio.token::transfer")]]
    void regstake(name username, name receiver, asset quantity, string memo);

    /*
    Increase the stake of a registered user by the precised quantity
    --> if user not registered, send error message
    */
    [[eosio::on_notify("eosio.token::transfer")]]
    void incrstake(name username, name receiver, asset quantity, string memo);

    /*
    Cancel the stake of a user and remove him from the table, refund the held amount
    */
    ACTION unstake (name username);

    /*
    Check the amount staked by username
    */
    ACTION checkstake(name username);

  private:
    // table used to maintain the list of staking nodes
        TABLE staker{
        name username; //name of the staking user
        asset fund_staked; // funds to be staked
        uint64_t primary_key() const {return username.value;}
        };

        typedef eosio::multi_index<name("stakerlist"), staker> stakers;

        stakers _staker_list;

        //the symbol of the tropium token
        //TODO: define this variable
        const symbol tropium_symb;
};

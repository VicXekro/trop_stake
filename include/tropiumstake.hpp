/**
* Staking smart contract for the Tropiumchain.io 
* Author: Victor
* Github: https://github.com/VicXekro
*/

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/transaction.hpp>
#include <eosio.token.hpp>

using namespace std;
using namespace eosio;

CONTRACT tropiumstake : public contract {
  public:
    using contract::contract;

     tropiumstake(name receiver, name code, datastream<const char*> ds):
        contract(receiver, code, ds),
        _staker_list(receiver, receiver.value),
        tropium_symb("TRPM",0){} //thre precision of the symbol is the decimal precision of
                                  //the created token

    /**
    * Register a user in the list of stakers.
    * @param: username, the account name of the staker
    */
    ACTION regstaker(name username);

    /**
    * Catch the transfer from eosio.token and register the funds staked
    * @param: receiver, the account owning this smart contract
    * @param: msg, define the action to be used. "start" to deposit the first funds, and 
    *         "increment" to increase the staked funds 
    */
    [[eosio::on_notify("*::transfer")]]
    void stake (name username, name receiver, asset quantity, string msg);

    /**
    * Check the "standing" of a staker in our staking smart contract
    * i.e., if the account is registered or not, if yes, then show the staked amount. Also check if account is banned
    * @param: username, the account whose standing is checked.
    */
    //ACTION checkstaker (name username);

    /*
    * Cancel the stake of a user and remove him from the table, refund the held amount
    * @param: username, the account wanting to unstake its funds. 
    */
    ACTION unstake (name username);

  private:
      // table used to maintain the list of staking nodes
        TABLE staker{
          name username; //name of the staking user
          asset fund_staked; // funds to be staked
          bool isstaked; //if the users has already staked funds or not
          uint64_t primary_key() const {return username.value;}
        };

        typedef eosio::multi_index<name("stakerlist"), staker> stakers;

        stakers _staker_list;

        //the symbol of the tropium token (TRPM, 0.0000)
        const symbol tropium_symb;

        /*// table used to maintain banned people. Doctors approval needed to remove them from the list
        TABLE banned{
          name username;
          uint64_t primary_key() const {return username.value;}
        }*/

        //void isbanned(name username);


};

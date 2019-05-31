#include "veos.hpp"

#include <cmath>
#include <enulib/action.hpp>
#include <enulib/asset.hpp>
#include "enu.token.hpp"

using namespace enumivo;
using namespace std;

void veos::got_enu_send_eln(const currency::transfer &transfer) {

  auto to = transfer.from;

  double received = transfer.quantity.amount;
  received = received/10000;

  // get ENU balance
  double enu_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ENU_SYMBOL).name()).amount;
  
  enu_balance = enu_balance/10000;
  enu_balance = enu_balance-received;  

  // get ELN balance
  double eln_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ELN_SYMBOL).name()).amount;

  eln_balance = eln_balance/10000;

  double product = eln_balance * enu_balance;
  double amount = eln_balance - (product / (received + enu_balance));

  auto quantity = asset(10000*amount, ELN_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(token.mm), 
    N(transfer),
    std::make_tuple(_self, to, quantity, std::string("Received ENU, sent ELN"))).send();
}

void veos::got_enu_send_veos(const currency::transfer &transfer) {
  auto to = transfer.from;

  double received = transfer.quantity.amount;
  received = received/10000;

  // get ENU balance
  double enu_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ENU_SYMBOL).name()).amount;
  
  enu_balance = enu_balance/10000;
  enu_balance = enu_balance - received;

  // get ELN balance
  double eln_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ELN_SYMBOL).name()).amount;

  eln_balance = eln_balance/10000;

  // get VEOS supply
  double veos_supply = enumivo::token(N(token.mm)).
	  get_supply(enumivo::symbol_type(VEOS_SYMBOL).name()).amount;

  veos_supply = veos_supply/10000;

  double a = eln_balance;
  double b = 2 * eln_balance * enu_balance;
  double c = - eln_balance * enu_balance * received;

  double eu = (sqrt((b*b)-(4*a*c)) - b)/(2*a);
  double ee = received - eu;

  double new_veos = veos_supply * (ee/(eu+enu_balance));
  auto quantity = asset(10000*new_veos, VEOS_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(token.mm), 
    N(issue),
    std::make_tuple(to, quantity, std::string("Received ENU, issued VEOS"))).send();
}

void veos::got_eln_send_enu(const currency::transfer &transfer) {
  auto to = transfer.from;

  double received = transfer.quantity.amount;
  received = received/10000;

  // get ELN balance
  double eln_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ELN_SYMBOL).name()).amount;
  
  eln_balance = eln_balance/10000;
  eln_balance = eln_balance-received;  

  // get ENU balance
  double enu_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ENU_SYMBOL).name()).amount;

  enu_balance = enu_balance/10000;

  double product = enu_balance * eln_balance;
  double amount = enu_balance - (product / (received + eln_balance));

  auto quantity = asset(10000*amount, ENU_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(token.mm), 
    N(transfer),
    std::make_tuple(_self, to, quantity, std::string("Received ELN, sent ENU"))).send();
}

void veos::got_eln_send_veos(const currency::transfer &transfer) {
  auto to = transfer.from;

  double received = transfer.quantity.amount;
  received = received/10000;

  // get ELN balance
  double eln_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ELN_SYMBOL).name()).amount;
  
  eln_balance = eln_balance/10000;
  eln_balance = eln_balance - received;

  // get ENU balance
  double enu_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ENU_SYMBOL).name()).amount;
  
  enu_balance = enu_balance/10000;

  // get VEOS supply
  double veos_supply = enumivo::token(N(token.mm)).
	   get_supply(enumivo::symbol_type(VEOS_SYMBOL).name()).amount;

  veos_supply = veos_supply/10000;

  double a = enu_balance;
  double b = 2 * enu_balance * eln_balance;
  double c = - enu_balance * eln_balance * received;

  double ue = (sqrt((b*b)-(4*a*c)) - b)/(2*a);
  double uu = received - ue;

  double new_veos = veos_supply * (uu/(ue+eln_balance));

  auto quantity = asset(10000*new_veos, VEOS_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(token.mm), 
    N(issue),
    std::make_tuple(to, quantity, std::string("Received ELN, issued VEOS"))).send();
}

void veos::got_veos_send_enu(const currency::transfer &transfer) {
  auto to = transfer.from;

  double received = transfer.quantity.amount;
  received = received/10000;

  // get ENU balance
  double enu_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ENU_SYMBOL).name()).amount;
  
  enu_balance = enu_balance/10000;

  // get ELN balance
  double eln_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ELN_SYMBOL).name()).amount;
  
  eln_balance = eln_balance/10000;

  // get VEOS supply
  double veos_supply = enumivo::token(N(token.mm)).
	   get_supply(enumivo::symbol_type(VEOS_SYMBOL).name()).amount;

  veos_supply = veos_supply/10000;

  double enu_portion = enu_balance * (received / veos_supply);
  double eln_portion = eln_balance * (received / veos_supply);

  double new_enu_balance = enu_balance - enu_portion;
  double new_eln_balance = eln_balance - eln_portion;

  double product = new_enu_balance * new_eln_balance;
  double enu_bought = new_enu_balance - (product / (eln_portion + new_eln_balance));

  double amount = enu_portion + enu_bought;

  auto quantity = asset(10000*amount, ENU_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(token.mm), 
    N(transfer),
    std::make_tuple(_self, to, quantity, std::string("Received VEOS, sent ENU")))
    .send();

  action(
    permission_level{_self, N(active)}, 
    N(token.mm), 
    N(retire),
    std::make_tuple(transfer.quantity, std::string("Retired VEOS"))).send();
}

void veos::got_veos_send_eln(const currency::transfer &transfer) {
  auto to = transfer.from;

  double received = transfer.quantity.amount;
  received = received/10000;

  // get ELN balance
  double eln_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ELN_SYMBOL).name()).amount;
  
  eln_balance = eln_balance/10000;

  // get VEOS supply
  double veos_supply = enumivo::token(N(token.mm)).
	   get_supply(enumivo::symbol_type(VEOS_SYMBOL).name()).amount;

  veos_supply = veos_supply/10000;

  // get ENU balance
  double enu_balance = enumivo::token(N(token.mm)).
	  get_balance(_self, enumivo::symbol_type(ENU_SYMBOL).name()).amount;
  
  enu_balance = enu_balance/10000;

  double eln_portion = eln_balance * (received / veos_supply);
  double enu_portion = enu_balance * (received / veos_supply);

  double new_eln_balance = eln_balance - eln_portion;
  double new_enu_balance = enu_balance - enu_portion;

  double product = new_eln_balance * new_enu_balance;
  double eln_bought = new_eln_balance - (product / (enu_portion + new_enu_balance));

  double amount = eln_portion + eln_bought;

  auto quantity = asset(10000*amount, ELN_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(token.mm), 
    N(transfer),
    std::make_tuple(_self, to, quantity, std::string("Received VEOS, sent ELN")))
    .send();

  action(
    permission_level{_self, N(active)}, 
    N(token.mm), 
    N(retire),
    std::make_tuple(transfer.quantity, std::string("Retired VEOS"))).send();
}

void veos::apply(account_name contract, action_name act) {

  if (act == N(transfer)) {
    
    auto transfer = unpack_action_data<currency::transfer>();
    
    if (transfer.to != _self) 
      return;
    
    if (contract == N(token.mm)) {

      if (transfer.quantity.symbol == VEOS_SYMBOL){  

        if (transfer.memo == "ENU") 
          got_veos_send_enu(transfer);
        else if (transfer.memo == "ELN") 
          got_veos_send_eln(transfer);
        else 
          enumivo_assert(false, "Memo must be ENU or ELN");

      } else 

        enumivo_assert(false, "Must send ENU or ELN or VEOS");

    } else if (contract == N(enu.token)) {

      if (transfer.quantity.symbol == ENU_SYMBOL){

        if (transfer.memo == "ELN") 
          got_enu_send_eln(transfer);
        else if (transfer.memo == "VEOS") 
          got_enu_send_veos(transfer);
        else 
          enumivo_assert(false, "Memo must be ELN or VEOS");

      } else 

        enumivo_assert(false, "Must send ENU or ELN or VEOS");

    } else if (contract == N(eln.coin)) {
        
      if (transfer.quantity.symbol == ELN_SYMBOL){

        if (transfer.memo == "ENU") 
          got_eln_send_enu(transfer);
        else if (transfer.memo == "VEOS") 
          got_eln_send_veos(transfer);
        else 
          enumivo_assert(false, "Memo must be ENU or VEOS");

      } else     

        enumivo_assert(false, "Must send ENU or ELN or VEOS");

    } else  

      enumivo_assert(false, "Must send ENU or ELN or VEOS");
  } 
}

extern "C" {
  [[noreturn]] void apply(uint64_t receiver, uint64_t code, uint64_t action) {
    veos mm(receiver);
    mm.apply(code, action);
    enumivo_exit(0);
  }
}

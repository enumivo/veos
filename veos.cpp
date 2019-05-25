#include "veos.hpp"

#include <cmath>
#include <enulib/action.hpp>
#include <enulib/asset.hpp>
#include "enu.token.hpp"

using namespace enumivo;
using namespace std;

void veos::got_enu_send_veos(const currency::transfer &transfer) {
  auto to = transfer.from;

  double received = transfer.quantity.amount;
  received = received/10000;

  // get ENU balance
  double enu_balance = enumivo::token(N(enu.token)).
	  get_balance(_self, enumivo::symbol_type(ENU_SYMBOL).name()).amount;
  
  enu_balance = enu_balance/10000;
  enu_balance = enu_balance - received;

  // get VEOS supply
  double veos_supply = enumivo::token(N(veos.coin)).
	   get_supply(enumivo::symbol_type(VEOS_SYMBOL).name()).amount;

  veos_supply = veos_supply/10000;

  double amount = veos_supply*(pow(1+(received/enu_balance),0.50)-1);

  auto quantity = asset(10000*amount, VEOS_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(veos.coin), 
    N(issue),
    std::make_tuple(to, quantity, std::string("Received ENU, issued VEOS"))).send();
}

void veos::got_eln_send_veos(const currency::transfer &transfer) {
  auto to = transfer.from;

  double received = transfer.quantity.amount;
  received = received/10000;

  // get ELN balance
  double eln_balance = enumivo::token(N(eln.coin)).
	  get_balance(_self, enumivo::symbol_type(ELN_SYMBOL).name()).amount;
  
  eln_balance = eln_balance/10000;
  eln_balance = eln_balance - received;

  // get VEOS supply
  double veos_supply = enumivo::token(N(veos.coin)).
	   get_supply(enumivo::symbol_type(VEOS_SYMBOL).name()).amount;

  veos_supply = veos_supply/10000;

  double amount = veos_supply*(pow(1+(received/eln_balance),0.50)-1);

  auto quantity = asset(10000*amount, VEOS_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(veos.coin), 
    N(issue),
    std::make_tuple(to, quantity, std::string("Received ELN, issued VEOS"))).send();
}

void veos::got_veos_send_enu(const currency::transfer &transfer) {
  auto to = transfer.from;

  double received = transfer.quantity.amount;
  received = received/10000;

  // get ENU balance
  double enu_balance = enumivo::token(N(enu.token)).
	  get_balance(_self, enumivo::symbol_type(ENU_SYMBOL).name()).amount;
  
  enu_balance = enu_balance/10000;

  // get VEOS supply
  double veos_supply = enumivo::token(N(veos.coin)).
	   get_supply(enumivo::symbol_type(VEOS_SYMBOL).name()).amount;

  veos_supply = veos_supply/10000;

  double amount = enu_balance*(1-(pow(1-(received/veos_supply),(1/0.50))));

  auto quantity = asset(10000*amount, ENU_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(enu.token), 
    N(transfer),
    std::make_tuple(_self, to, quantity, std::string("Received VEOS, sent ENU")))
    .send();

  auto retire = asset(10000*received, VEOS_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(veos.coin), 
    N(retire),
    std::make_tuple(retire, std::string("Retired VEOS"))).send();
}

void veos::got_veos_send_eln(const currency::transfer &transfer) {
  auto to = transfer.from;

  double received = transfer.quantity.amount;
  received = received/10000;

  // get ELN balance
  double eln_balance = enumivo::token(N(eln.coin)).
	  get_balance(_self, enumivo::symbol_type(ELN_SYMBOL).name()).amount;
  
  eln_balance = eln_balance/10000;

  // get VEOS supply
  double veos_supply = enumivo::token(N(veos.coin)).
	   get_supply(enumivo::symbol_type(VEOS_SYMBOL).name()).amount;

  veos_supply = veos_supply/10000;

  double amount = eln_balance*(1-(pow(1-(received/veos_supply),(1/0.50))));

  auto quantity = asset(10000*amount, ELN_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(eln.coin), 
    N(transfer),
    std::make_tuple(_self, to, quantity, std::string("Received VEOS, sent ELN")))
    .send();

  auto retire = asset(10000*received, VEOS_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(veos.coin), 
    N(retire),
    std::make_tuple(retire, std::string("Retired VEOS"))).send();
}

void veos::apply(account_name contract, action_name act) {

  if (act == N(transfer)) {
    
    auto transfer = unpack_action_data<currency::transfer>();
    
    if (transfer.to != _self) 
      return;
    
    if (contract == N(enu.token)) 
      got_enu_send_veos(transfer);
    else if (contract == N(eln.coin)) 
      got_eln_send_veos(transfer);
    else if (contract == N(veos.coin)) {
      if (transfer.memo == "ENU") 
        got_veos_send_enu(transfer);
      else if (transfer.memo == "ELN") 
        got_veos_send_eln(transfer);
      else 
        enumivo_assert(false, "Memo must be ENU or ELN");
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

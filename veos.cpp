#include "veos.hpp"

#include <cmath>
#include <enulib/action.hpp>
#include <enulib/asset.hpp>
#include "enu.token.hpp"

using namespace enumivo;
using namespace std;

void veos::got_enu_send_eln(const currency::transfer &transfer) {
  auto to = transfer.from;

  // get ENU balance
  double enu_balance = enumivo::token(N(enu.token)).
	   get_balance(_self, enumivo::symbol_type(ENU_SYMBOL).name()).amount;
  
  enu_balance = enu_balance/10000;

  // get ELN balance
  double eln_balance = enumivo::token(N(eln.coin)).
	   get_balance(_self, enumivo::symbol_type(ELN_SYMBOL).name()).amount;

  eln_balance = eln_balance/10000;

  double product = eln_balance * enu_balance;
  double amount = eln_balance - (product / (received + enu_balance));

  auto to = transfer.from;
  auto quantity = asset(10000*amount, ELN_SYMBOL);

  action(
    permission_level{_self, N(active)}, 
    N(eln.coin), 
    N(transfer),
    std::make_tuple(_self, to, quantity, std::string("veos.mm received ENU, sent ELN"))).send();
}

void veos::got_enu_send_veos(const currency::transfer &transfer) {
}

void veos::got_eln_send_enu(const currency::transfer &transfer) {
}

void veos::got_eln_send_veos(const currency::transfer &transfer) {
}

void veos::got_veos_send_enu(const currency::transfer &transfer) {
}

void veos::got_veos_send_eln(const currency::transfer &transfer) {
}

void veos::apply(account_name contract, action_name act) {

  if (act == N(transfer)) {
    
    auto transfer = unpack_action_data<currency::transfer>();
    
    if (transfer.to != _self) 
      return;
    
    if (contract == N(enu.token)) {
      if (transfer.memo == "ELN") 
        got_enu_send_eln(transfer);
      else if (transfer.memo == "VEOS") 
        got_enu_send_veos(transfer);
      else 
        enumivo_assert(false, "Memo must be ELN or VEOS");
    } else if (contract == N(eln.coin)) {
      if (transfer.memo == "ENU") 
        got_eln_send_enu(transfer);
      else if (transfer.memo == "VEOS") 
        got_eln_send_veos(transfer);
      else 
        enumivo_assert(false, "Memo must be ENU or VEOS");

    } else if (contract == N(veos.coin)) {
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

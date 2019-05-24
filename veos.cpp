#include "veos.hpp"

#include <cmath>
#include <enulib/action.hpp>
#include <enulib/asset.hpp>
#include "enu.token.hpp"

using namespace enumivo;
using namespace std;

void veos::got_enu_send_eln(const currency::transfer &transfer) {
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

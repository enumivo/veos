#include "veos.hpp"

#include <cmath>
#include <enulib/action.hpp>
#include <enulib/asset.hpp>
#include "enu.token.hpp"

using namespace enumivo;
using namespace std;

void veos::got_enu(const currency::transfer &transfer) {
}

void veos::got_eln(const currency::transfer &transfer) {
}

void veos::got_veos(const currency::transfer &transfer) {
}

void veos::apply(account_name contract, action_name act) {

  if (contract == N(enu.token) && act == N(transfer)) {
    auto transfer = unpack_action_data<currency::transfer>();
    enumivo_assert(transfer.quantity.symbol == ENU_SYMBOL, "Must send ENU");
    got_enu(transfer);
    return;
  }

  if (contract == N(eln.coin) && act == N(transfer)) {
    auto transfer = unpack_action_data<currency::transfer>();
    enumivo_assert(transfer.quantity.symbol == ELN_SYMBOL, "Must send ELN");
    got_eln(transfer);
    return;
  }

  if (contract == N(veos.coin) && act == N(transfer)) {
    auto transfer = unpack_action_data<currency::transfer>();
    enumivo_assert(transfer.quantity.symbol == VEOS_SYMBOL, "Must send VEOS");
    got_veos(transfer);
    return;
  }

  if (act == N(transfer)) {
    auto transfer = unpack_action_data<currency::transfer>();
    enumivo_assert(false, "Must send ENU, ELN, or VEOS");
    return;
  }

  if (contract != _self) return;
}

extern "C" {
  [[noreturn]] void apply(uint64_t receiver, uint64_t code, uint64_t action) {
    veos mm(receiver);
    mm.apply(code, action);
    enumivo_exit(0);
  }
}

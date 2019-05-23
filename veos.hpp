#include <enulib/currency.hpp>
#include <enulib/enu.hpp>
#include <vector>

#define ENU_SYMBOL S(4, ENU)  
#define ELN_SYMBOL S(4, ELN)  
#define VEOS_SYMBOL S(4, VEOS)  

using namespace enumivo;

class veos : public contract {
 public:
  veos(account_name self): contract(self) {}

  void got_eln(const currency::transfer& transfer);
  void got_enu(const currency::transfer& transfer);
  void got_veos(const currency::transfer& transfer);

  void apply(account_name contract, action_name act);
};

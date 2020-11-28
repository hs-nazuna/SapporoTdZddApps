#ifndef SAPPORO_TDZDD_APPS_EXT_OPERATIONS_HPP
#define SAPPORO_TDZDD_APPS_EXT_OPERATIONS_HPP

#include "ZBDD.h"

namespace sapporo_tdzdd_apps {

/*****
 * check_sapporo_vars(n)
 *****/
void check_sapporo_vars(int n) {
    while (BDD_VarUsed() < n) BDD_NewVar();
}

} // namespace sapporo_tdzdd_apps

#endif
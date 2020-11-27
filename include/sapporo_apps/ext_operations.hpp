#ifndef SAPPORO_TDZDD_APPS_EXT_OPERATIONS_HPP
#define SAPPORO_TDZDD_APPS_EXT_OPERATIONS_HPP

#include "ZBDD.h"

namespace sapporo_tdzdd_apps {

void check_sapporo_vars(int n, int offset = 0) {
    while (BDD_VarUsed() < n + offset) BDD_NewVar();
}

} // namespace sapporo_tdzdd_apps

#endif
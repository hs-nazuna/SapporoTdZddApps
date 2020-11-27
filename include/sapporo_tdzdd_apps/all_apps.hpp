#ifndef SAPPORO_TDZDD_APPS_HPP
#define SAPPORO_TDZDD_APPS_HPP

#include "sapporo_apps.hpp"
#include "tdzdd_apps.hpp"
#include "converter.hpp"

namespace sapporo_tdzdd_apps {

tdzdd::DdStructure<2> to_ddstructure(const ZBDD& zbdd) {
    return tdzdd::DdStructure<2>(tdzdd::SapporoZdd(zbdd));
}

ZBDD to_zbdd(const tdzdd::DdStructure<2>& dd) {
    return dd.evaluate(tdzdd::ToZBDD());
}

} // namespace sapporo_tdzdd_apps

#endif
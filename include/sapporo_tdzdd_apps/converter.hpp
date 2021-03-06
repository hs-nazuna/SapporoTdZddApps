#ifndef SAPPORO_TDZDD_APPS_CONVERTER_HPP
#define SAPPORO_TDZDD_APPS_CONVERTER_HPP

#include <tdzdd/spec/SapporoZdd.hpp>
#include <tdzdd/eval/ToZBDD.hpp>
#include <tdzdd/DdStructure.hpp>

namespace sapporo_tdzdd_apps {

/*****
 * to_ddstructure(zbdd)
 *      Convert ZBDD to DdStructure.
 *****/
tdzdd::DdStructure<2> to_ddstructure(const ZBDD& zbdd) {
    return tdzdd::DdStructure<2>(tdzdd::SapporoZdd(zbdd));
}

/*****
 * to_zbdd(dd)
 *      Convert DdStructure to ZBDD.
 *****/
ZBDD to_zbdd(const tdzdd::DdStructure<2>& dd) {
    return dd.evaluate(tdzdd::ToZBDD());
}

} // namespace sapporo_tdzdd_apps

#endif
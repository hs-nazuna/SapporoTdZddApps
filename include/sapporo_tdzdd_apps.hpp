#ifndef SAPPORO_TDZDD_APPS_HPP
#define SAPPORO_TDZDD_APPS_HPP

#include "sapporo_apps/ext_operations.hpp"
#include "tdzdd_apps/graph_data.hpp"
#include "tdzdd_apps/graph_specs.hpp"
#include "tdzdd_apps/linear_specs.hpp"

#include <tdzdd/spec/SapporoZdd.hpp>
#include <tdzdd/eval/ToZBDD.hpp>
#include <tdzdd/DdStructure.hpp>

namespace sapporo_tdzdd_apps {

// for TdZdd

tdzdd::DdStructure<2> tdzdd_linear_inequalities(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::string>& sign,
    const std::vector<int>& b
) {
    LinearIneqSpec spec(A, sign, b);
    tdzdd::DdStructure<2> dd(spec);
    dd.zddReduce();
    return dd;
}

// for SAPPOROBDD

void check_sapporo_vars(int n, int offset = 0) {
    while (BDD_VarUsed() < n + offset) BDD_NewVar();
}

ZBDD zbdd_power_set(int n) {
    check_sapporo_vars(n);
	ZBDD f(1);
	for (int i = 0; i < n; ++i) f = f.Change(BDD_VarOfLev(i + 1)) + f;
	return f;
}

// other utilities

tdzdd::DdStructure<2> to_ddstructure(const ZBDD& zbdd) {
    return tdzdd::DdStructure<2>(tdzdd::SapporoZdd(zbdd));
}

ZBDD to_zbdd(const tdzdd::DdStructure<2>& dd) {
    return dd.evaluate(tdzdd::ToZBDD());
}

std::vector<std::vector<int>> unfold_zdd(
    int n_vars,
    const tdzdd::DdStructure<2>& dd,
    bool sorted = false
) {
    std::vector<std::vector<int>> answer_set;
    for (auto path = dd.begin(); path != dd.end(); ++path) {
        std::vector<int> ans;
        for (auto it = path->begin(); it != path->end(); ++it) {
            int i = n_vars - *it;
            ans.push_back(i);
        }
        std::reverse(ans.begin(), ans.end());
        answer_set.push_back(ans);
    }
    if (sorted) std::sort(answer_set.begin(), answer_set.end());
    return answer_set;
}

} // namespace sapporo_tdzdd_apps

#endif
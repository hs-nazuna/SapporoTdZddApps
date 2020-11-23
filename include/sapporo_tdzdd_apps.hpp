#ifndef SAPPORO_TDZDD_APPS_HPP
#define SAPPORO_TDZDD_APPS_HPP

#include "sapporo_apps/ext_operations.hpp"
#include "tdzdd_apps/graph_data.hpp"
#include "tdzdd_apps/itemset_data.hpp"
#include "tdzdd_apps/graph_specs.hpp"
#include "tdzdd_apps/linear_specs.hpp"

#include <tdzdd/DdStructure.hpp>

namespace sapporo_tdzdd_apps {

tdzdd::DdStructure<2> tdzdd_linear_inequalities(
    const std::vector<std::vector<int>>& A,
    const std::vector<int>& b,
    const std::vector<std::string>& sign
) {
    LinearIneqSpec spec(A, b, sign);
    tdzdd::DdStructure<2> dd(spec);
    dd.zddReduce();
    return dd;
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
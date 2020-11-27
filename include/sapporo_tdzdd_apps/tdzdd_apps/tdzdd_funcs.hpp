#ifndef SAPPORO_TDZDD_APPS_TDZDD_FUNCS_HPP
#define SAPPORO_TDZDD_APPS_TDZDD_FUNCS_HPP

#include <tdzdd/DdStructure.hpp>
#include "graph_data.hpp"
#include "graph_specs.hpp"
#include "linear_specs.hpp"

namespace sapporo_tdzdd_apps {

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

std::vector<std::vector<int>> unfold_tdzdd(
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
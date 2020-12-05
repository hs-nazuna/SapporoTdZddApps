#ifndef SAPPORO_TDZDD_APPS_SAPPORO_FUNCS_HPP
#define SAPPORO_TDZDD_APPS_SAPPORO_FUNCS_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
#include <set>
#include "for_sapporo/ext_operations.hpp"

namespace sapporo_tdzdd_apps {

/*****
 * zbdd_power_set(n_vars)
 *      Construct ZBDD representing the power set of n_vars items.
 *****/
ZBDD zbdd_power_set(int n_vars) {
    check_sapporo_vars(n_vars);
	ZBDD f(1);
	for (int i = 1; i <= n_vars; ++i) f += f.Change(i);
	return f;
}

/*****
 * zbdd_single_subset(n_vars, subset)
 *      Construct ZBDD representing the given single subset
 *      which must be a subset of variable numbers on SAPPOROBDD.
 *****/
ZBDD zbdd_single_subset(const std::vector<int>& S) {
    check_sapporo_vars(*std::max_element(S.begin(), S.end()));
    ZBDD f(1);
    for (int i : S) f = f.Change(i);
    return f;
}

/*****
 * zbdd_extraction(zbdd, targets)
 *      TODO: description
 *****/
ZBDD zbdd_extraction(const ZBDD& zbdd, const std::set<int>& targets) {
    std::function<ZBDD(const ZBDD&)> rec = [&](const ZBDD& f) {
        if (f == 0 or f == 1) return f;
        int i = f.Top();
        ZBDD f0 = f.OffSet(i), f1 = f.OnSet0(i);
        ZBDD g0 = rec(f0), g1 = rec(f1);
        if (targets.count(i) == 1) g1 = g1.Change(i);
        return g0 + g1;
    };
    return rec(zbdd);
}

/*****
 * unfold_zbdd(n_vars, zbdd, sorted)
 *      Unfold a given ZBDD over n_vars variables.
 *      Each vector in return value represents a subset.
 *      If sorted = true, subsets are sorted in lexicographical order.
 *****/
std::vector<std::vector<int>> unfold_zbdd(
    int n_vars,
    const ZBDD& zbdd,
    bool sorted = false
) {
    assert(zbdd.Top() <= n_vars);
    
    std::vector<std::vector<int>> answer_set;
    
    std::function<void(const ZBDD&, std::vector<int>&)> dfs =
    [&](const ZBDD& f, std::vector<int>& ans) {
        if (f == 0) return;
        if (f == 1) {
            answer_set.push_back(ans);
            return;
        }
        int i = f.Top();
        ZBDD f0 = f.OffSet(i), f1 = f.OnSet0(i);
        dfs(f0, ans);
        ans.push_back(n_vars - i);
        dfs(f1, ans);
        ans.pop_back();
    };

    std::vector<int> tmp;
    dfs(zbdd, tmp);
    if (sorted) std::sort(answer_set.begin(), answer_set.end());
    return answer_set;
}
    
} // namespace sapporo_tdzdd_apps

#endif
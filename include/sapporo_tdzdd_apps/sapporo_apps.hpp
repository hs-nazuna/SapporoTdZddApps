#ifndef SAPPORO_TDZDD_APPS_SAPPORO_FUNCS_HPP
#define SAPPORO_TDZDD_APPS_SAPPORO_FUNCS_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
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
ZBDD zbdd_single_subset(const std::vector<int>& subset) {
    check_sapporo_vars(*std::max_element(subset.begin(), subset.end()));
    ZBDD f(1);
    for (int i : subset) f = f.Change(i);
    return f;
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

        int level = f.Top();
        ZBDD f0 = f.OffSet(level), f1 = f.OnSet0(level);
        dfs(f0, ans);
        ans.push_back(n_vars - level);
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
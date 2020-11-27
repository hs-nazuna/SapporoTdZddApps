#ifndef SAPPORO_TDZDD_APPS_SAPPORO_FUNCS_HPP
#define SAPPORO_TDZDD_APPS_SAPPORO_FUNCS_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
#include "for_sapporo/ext_operations.hpp"

namespace sapporo_tdzdd_apps {

ZBDD zbdd_power_set(int n) {
    check_sapporo_vars(n);
	ZBDD f(1);
	for (int i = 0; i < n; ++i) f += f.Change(BDD_VarOfLev(i + 1));
	return f;
}

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
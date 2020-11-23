#ifndef SAPPORO_TDZDD_APPS_NAIVE_ENUMERATION_HPP
#define SAPPORO_TDZDD_APPS_NAIVE_ENUMERATION_HPP

#include <algorithm>
#include "instance_reader.hpp"

std::vector<std::vector<int>> naive_linear_inequarities(
    const LinearInequalities& instance
) {
    std::vector<std::vector<int>> answer_set;
    for (int X = 0; X < (1 << instance.n_vars); ++X) {
        bool ok = true;
        for (int r = 0; r < instance.n_rows; ++r) {
            int sum = 0;
            for (int i = 0; i < instance.n_vars; ++i) {
                if (X >> i & 1) sum += instance.A[r][i];
            }
            if (instance.sign[r] == "<=") ok &= (sum <= instance.b[r]);
            if (instance.sign[r] == "=") ok &= (sum == instance.b[r]);
            if (instance.sign[r] == ">=") ok &= (sum >= instance.b[r]);
        }
        if (ok) {
            std::vector<int> ans;
            for (int i = 0; i < instance.n_vars; ++i) {
                if (X >> i & 1) ans.push_back(i);
            }
            answer_set.push_back(ans);
        }
    }
    std::sort(answer_set.begin(), answer_set.end());
    return answer_set;
}

#endif
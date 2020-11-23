#ifndef SAPPORO_TDZDD_APPS_LINEAR_SPECS_HPP
#define SAPPORO_TDZDD_APPS_LINEAR_SPECS_HPP

// cpp default headers
#include <vector>
#include <string>
#include <algorithm>

// additional headers
#include <tdzdd/DdSpec.hpp>

namespace sapporo_tdzdd_apps {

// Spec for linear inequalities Ax {<=,=,>=} b
class LinearIneqSpec : public tdzdd::PodArrayDdSpec<LinearIneqSpec, int, 2> {
private:
    const int n_vars;
    const int n_rows;
    const std::vector<std::vector<int>>& A;
    const std::vector<int>& b;
    const std::vector<std::string>& sign;

    std::vector<std::vector<int>> neg_sum;
    std::vector<std::vector<int>> pos_sum;

    void add_item(int* mate, int i) const {
        for (int r = 0; r < n_rows; ++r) mate[r] += A[r][i];
    }

    bool check_conditions(int* mate, int i) const {
        for (int r = 0; r < n_rows; ++r) {
            int lb = mate[r] + neg_sum[r][i];
            int ub = mate[r] + pos_sum[r][i];
            if ((sign[r] == "<=" or sign[r] == "=") and lb > b[r]) return false;
            if ((sign[r] == ">=" or sign[r] == "=") and ub < b[r]) return false;
        }
        return true;
    }

public:
    LinearIneqSpec(
        const std::vector<std::vector<int>>& A,
        const std::vector<int>& b,
        const std::vector<std::string>& sign
    ) : n_vars(A[0].size()), n_rows(A.size()), A(A), b(b), sign(sign) {
        setArraySize(n_rows);
        neg_sum.assign(n_rows, std::vector<int>(n_vars + 1, 0));
        pos_sum.assign(n_rows, std::vector<int>(n_vars + 1, 0));
        for (int r = 0; r < n_rows; ++r) {
            for (int i = n_vars - 1; i >= 0; --i) {
                neg_sum[r][i] = neg_sum[r][i + 1] + std::min(A[r][i], 0);
                pos_sum[r][i] = pos_sum[r][i + 1] + std::max(A[r][i], 0);
            }
        }
    }

    int getRoot(int* mate) const {
        for (int r = 0; r < n_rows; ++r) mate[r] = 0;
        if (!check_conditions(mate, 0)) return 0;
        return n_vars;
    }

    int getChild(int* mate, int level, bool take) const {
        int i = n_vars - level;
        if (take) add_item(mate, i);
        if (!check_conditions(mate, i + 1)) return 0;
        return (level > 1 ? level - 1 : -1);
    }
};

} // namespace sapporo_tdzdd_apps

#endif
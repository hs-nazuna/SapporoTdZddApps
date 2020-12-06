#ifndef SAPPORO_TDZDD_APPS_OPTIMIZATION_HPP
#define SAPPORO_TDZDD_APPS_OPTIMIZATION_HPP

#include <vector>
#include <string>
#include <limits>
#include <utility>
#include <tdzdd/DdStructure.hpp>
#include <tdzdd/dd/NodeTable.hpp>
#include "converter.hpp"
#include "for_sapporo/ext_operations.hpp"

namespace sapporo_tdzdd_apps {

class OptimizationBase {
protected:
    int n;
    tdzdd::DdStructure<2> dd;

public:
    OptimizationBase() {}

    void set_dd(const ZBDD& f) {
        dd = to_ddstructure(f);
        n = dd.topLevel();
        check_sapporo_vars(n);
    }

    void set_dd(const tdzdd::DdStructure<2>& f) {
        dd = f;
        n = dd.topLevel();
        check_sapporo_vars(n);
    }
};

template<typename T> class LinearOptimization : public OptimizationBase {
private:
    std::pair<T, ZBDD> bottom_up_dp(const std::vector<int>& cost, int dir) {
        auto func = [&](T a, T b) {
            if (dir > 0) return std::max(a, b);
            else return std::min(a, b);
        };
        
        const tdzdd::NodeTableHandler<2>& diagram = dd.getDiagram();
        std::vector<std::vector<T>> best(n + 1, std::vector<T>());
        std::vector<std::vector<ZBDD>> ans(n + 1, std::vector<ZBDD>());
        for (int i = 0; i <= n; ++i) {
            int w = (*diagram)[i].size();
            if (dir > 0) best[i].assign(w, std::numeric_limits<T>().min());
            if (dir < 0) best[i].assign(w, std::numeric_limits<T>().max());
            ans[i].assign(w, ZBDD(0));
        }
        best[0][1] = 0;
        ans[0][1] = ZBDD(1);
        
        for (int i = 1; i <= n; ++i) {
            int w = (*diagram)[i].size();
            for (int j = 0; j < w; ++j) {
                // update best value
                for (int b = 0; b < 2; ++b) {
                    tdzdd::NodeId node = diagram->child(i, j, b);
                    int r = node.row(), c = node.col();
                    if (r == 0 and c == 0) continue;
                    T val = best[r][c] + (b == 1 ? cost[n - i] : 0);
                    best[i][j] = func(best[i][j], val);
                }
                // restore optimal solutions
                for (int b = 0; b < 2; ++b) {
                    tdzdd::NodeId node = diagram->child(i, j, b);
                    int r = node.row(), c = node.col();
                    if (r == 0 and c == 0) continue;
                    T val = best[r][c] + (b == 1 ? cost[n - i] : 0);
                    if (best[i][j] == val) {
                        if (b == 0) ans[i][j] += ans[r][c];
                        if (b == 1) ans[i][j] += ans[r][c].Change(i);
                    }
                }
            }
        }
        
        return std::pair<T, ZBDD>(best[n][0], ans[n][0]);
    }

public:
    LinearOptimization() {}

    std::pair<T, ZBDD> optimize(
        const std::vector<int>& cost,
        std::string direction = "maximize"
    ) {
        return bottom_up_dp(cost, direction == "maximize" ? 1 : -1);
    }
};

} // namespace sapporo_tdzdd_apps

#endif
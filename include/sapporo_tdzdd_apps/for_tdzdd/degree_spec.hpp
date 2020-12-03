#ifndef SAPPORO_TDZDD_APPS_DEGREE_SPEC_HPP
#define SAPPORO_TDZDD_APPS_DEGREE_SPEC_HPP

#include <vector>
#include <algorithm>
#include <tdzdd/DdSpec.hpp>
#include "graph_data.hpp"

namespace sapporo_tdzdd_apps {

/*****
 * class DegreeSpec
 *      Enumerate all subgraphs satisfying degree constraints.
 *      A degree constraint for a vertex must be represented by
 *      a lower bound and an upper bound.
 *****/
class DegreeSpec : public tdzdd::PodArrayDdSpec<DegreeSpec, int, 2> {
private:
    const Graph& G;
    const std::vector<int>& lb;
    const std::vector<int>& ub;
    const bool with_vertex;

    const int TAKE_FLAG = 1 << 30;
    const int COMPLETE = (1 << 20) - 1;

    std::vector<std::vector<int>> adj;

    void add_degree(int* mate, int i) const {
        if ((mate[i] & COMPLETE) != COMPLETE) ++mate[i];
        mate[i] |= TAKE_FLAG;
    }

    bool check_conditions(int* mate, int i, int vi, int v) const {
        if (mate[vi] > ub[v]) return false;
        auto it = std::upper_bound(adj[v].begin(), adj[v].end(), i);
        int max_deg = mate[vi] + (adj[v].end() - it);
        if (max_deg < lb[v]) return false;
        if (lb[v] <= mate[vi] and max_deg <= ub[v]) mate[vi] |= COMPLETE;
        return true;
    }

public:
    DegreeSpec(
        const Graph& G,
        const std::vector<int>& lb,
        const std::vector<int>& ub,
        bool with_vertex=false
    ) : G(G), lb(lb), ub(ub), with_vertex(with_vertex)
    {
        int n = G.max_vertex_number() + 1;
        assert((int)lb.size() == n);
        assert((int)ub.size() == n);
        for (int v = 0; v < n; ++v) assert(lb[v] <= ub[v]);

        adj.assign(G.max_vertex_number(), std::vector<int>());
        for (int i : G.edge_vars()) {
            adj[G[i][0]].push_back(i);
            adj[G[i][1]].push_back(i);
        }
        
        setArraySize(G.max_frontier_size());
    }

    int getRoot(int* mate) const {
        int F = G.max_frontier_size();
        for (int i = 0; i < F; ++i) mate[i] = 0;
        return G.n_items();
    }

    int getChild(int* mate, int level, bool take) const {
        int i = G.n_items() - level;
        
        if (G.is_vertex(i)) {
            if (take and not with_vertex) return 0;
            // G[i][0] leaves frontier
            int vi = G.frontier_index(G[i][0]);
            if (with_vertex) {
                if (not take and (mate[vi] & TAKE_FLAG) != 0) return 0;
                if (take and (mate[vi] & TAKE_FLAG) == 0) return 0;
            }
            mate[vi] = 0;
        }
        else {
            int u = G[i][0], v = G[i][1];
            int ui = G.frontier_index(u), vi = G.frontier_index(v);

            if (take) {
                add_degree(mate, ui);
                add_degree(mate, vi);
            }

            if (!check_conditions(mate, i, ui, u)) return 0;
            if (!check_conditions(mate, i, vi, v)) return 0;
        }

        return (level > 1 ? level - 1 : -1);
    }
};

} // namespace sapporo_tdzdd_apps

#endif
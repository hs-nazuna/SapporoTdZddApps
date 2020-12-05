#ifndef SAPPORO_TDZDD_APPS_DEGREE_SPEC_HPP
#define SAPPORO_TDZDD_APPS_DEGREE_SPEC_HPP

#include <vector>
#include <set>
#include <algorithm>
#include <tdzdd/DdSpec.hpp>
#include "graph_data.hpp"

namespace sapporo_tdzdd_apps {

/*****
 * class RangeDegreeSpec
 *****/
class RangeDegreeSpec :
    public tdzdd::PodArrayDdSpec<RangeDegreeSpec, int, 2> {
private:
    const Graph& G;
    const int F;
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
        int deg = mate[vi] & COMPLETE;
        if (deg == COMPLETE) return true;
        if (deg > ub[v]) return false;
        auto it = std::upper_bound(adj[v].begin(), adj[v].end(), i);
        int max_deg = deg + (adj[v].end() - it);
        if (max_deg < lb[v]) return false;
        if (lb[v] <= deg and max_deg <= ub[v]) mate[vi] |= COMPLETE;
        return true;
    }

public:
    RangeDegreeSpec(
        const Graph& G,
        const std::vector<int>& lb,
        const std::vector<int>& ub,
        bool with_vertex=false
    ) : G(G), F(G.max_frontier_size()),
        lb(lb), ub(ub), with_vertex(with_vertex)
    {
        int n = G.max_vertex_number() + 1;
        assert((int)lb.size() == n);
        assert((int)ub.size() == n);
        for (int v = 0; v < n; ++v) assert(lb[v] <= ub[v]);

        adj.assign(n, std::vector<int>());
        int m = G.n_edges();
        for (int i = 0; i < m; ++i) {
            int ei = G.var_of_edge(i);
            adj[G[ei][0]].push_back(ei);
            adj[G[ei][1]].push_back(ei);
        }
        
        setArraySize(F);
    }

    int getRoot(int* mate) const {
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

/*****
 * class DegreeSpec
 *****/
class DegreeSpec : public tdzdd::PodArrayDdSpec<DegreeSpec, int, 2> {
private:
    const Graph& G;
    const int F;
    const std::vector<std::set<int>>& candidates;
    const bool with_vertex;

public:
    DegreeSpec(
        const Graph& G,
        const std::vector<std::set<int>>& candidates,
        bool with_vertex = false        
    ) : G(G), F(G.max_frontier_size()),
        candidates(candidates), with_vertex(with_vertex) 
    {
        int n = G.max_vertex_number() + 1;
        assert((int)candidates.size() == n);
        setArraySize(F);
    }

    int getRoot(int* mate) const {
        for (int i = 0; i < F; ++i) mate[i] = 0;
        return G.n_items();
    }

    int getChild(int* mate, int level, bool take) const {
        int i = G.n_items() - level;

        if (G.is_vertex(i)) {
            if (take and not with_vertex) return 0;
            // G[i][0] leaves frontier
            int v = G[i][0];
            int vi = G.frontier_index(v);
            if (with_vertex) {
                if (not take and mate[vi] > 0) return 0;
                if (take and mate[vi] == 0) return 0;
            }
            // check degree
            if (candidates[v].count(mate[vi]) == 0) return 0;
            mate[vi] = 0;
        }
        else if (take) {
            int u = G[i][0], v = G[i][1];
            int ui = G.frontier_index(u), vi = G.frontier_index(v);
            ++mate[ui];
            ++mate[vi];
            if (*candidates[u].rbegin() < mate[ui]) return 0;
            if (*candidates[v].rbegin() < mate[vi]) return 0;
        }

        return (level > 1 ? level - 1 : -1);
    }
};

/*****
 * class SteinerSpec
 *****/
class SteinerSpec : public tdzdd::PodArrayDdSpec<SteinerSpec, int, 2> {
private:
    const Graph& G;
    const int F;
    const std::set<int>& T;
    const bool with_vertex;

public:
    SteinerSpec(
        const Graph& G,
        const std::set<int>& T,
        bool with_vertex = false
    ) : G(G), F(G.max_frontier_size()), T(T), with_vertex(with_vertex)
    {
        setArraySize(F);
    }

    int getRoot(int* mate) const {
        for (int i = 0; i < F; ++i) mate[i] = 0;
        return G.n_items();
    }

    int getChild(int* mate, int level, bool take) const {
        int i = G.n_items() - level;

        if (G.is_vertex(i)) {
            if (take and not with_vertex) return 0;
            // G[i][0] leaves frontier
            int v = G[i][0];
            int vi = G.frontier_index(v);
            if (with_vertex) {
                if (not take and mate[vi] > 0) return 0;
                if (take and mate[vi] == 0) return 0;
            }
            // check terminal
            if (mate[vi] == 0 and T.count(v) == 1) return 0;
            mate[vi] = 0;
        }
        else if (take) {
            int u = G[i][0], v = G[i][1];
            int ui = G.frontier_index(u), vi = G.frontier_index(v);
            mate[ui] = mate[vi] = 1;
        }

        return (level > 1 ? level - 1 : -1);
    }
};

} // namespace sapporo_tdzdd_apps

#endif
#ifndef SAPPORO_TDZDD_APPS_COMPONENT_SPEC_HPP
#define SAPPORO_TDZDD_APPS_COMPONENT_SPEC_HPP

#include <algorithm>
#include <tdzdd/DdSpec.hpp>
#include "graph_data.hpp"

namespace sapporo_tdzdd_apps {

/*****
 * class ComponentSpecBase
 *****/
class ComponentSpecBase {
protected:
    const Graph& G;
    const int F;
    const bool non_cyclic;
    const bool with_vertex;
    
    const int INIT = -1;

    int entry(int* mate, int v) const {
        int i = G.frontier_index(v);
        if (mate[i] == INIT) mate[i] = *std::max_element(mate, mate + F) + 1;
        return i;
    }

    void translation(int* mate) const {
        int c = 0;
        std::vector<int> trans(F + 1, -1);
        for (int i = 0; i < F; ++i) {
            int mi = mate[i];
            if (mi == INIT) continue;
            if (trans[mi] == -1) trans[mi] = mate[i] = c++;
            else mate[i] = trans[mi];
        }
    }

    void connect(int* mate, int ui, int vi) const {
        int a = mate[ui], b = mate[vi];
        for (int i = 0; i < F; ++i) if (mate[i] == a) mate[i] = b;
        translation(mate);
    }

    bool is_independent(int* mate, int i) const {
        if (mate[i] == INIT) return false;
        for (int j = 0; j < F; ++j) {
            if (j == i) continue;
            if (mate[i] == mate[j]) return false;
        }
        return true;
    }

    bool find_other_component(int* mate, int c) const {
        for (int i = 0; i < F; ++i) {
            if (mate[i] == INIT) continue;
            if (mate[i] != c) return true; 
        }
        return false;
    }

public:
    ComponentSpecBase(
        const Graph& G,
        bool non_cyclic,
        bool with_vertex
    ) : G(G), F(G.max_frontier_size()),
        non_cyclic(non_cyclic), with_vertex(with_vertex) {}
};

/*****
 * class ConnectedSpec
 *****/
class ConnectedSpec
    : public tdzdd::PodArrayDdSpec<ConnectedSpec, int, 2>,
      public ComponentSpecBase {

public:
    ConnectedSpec(
        const Graph& G,
        bool non_cyclic = false,
        bool with_vertex = false
    ) : ComponentSpecBase(G, non_cyclic, with_vertex)
    {
        setArraySize(F);
    }

    int getRoot(int* mate) const {
        for (int i = 0; i < F; ++i) mate[i] = INIT;
        return G.n_items();
    }

    int getChild(int* mate, int level, bool take) const {
        int i = G.n_items() - level;

        if (G.is_vertex(i)) {
            if (take and not with_vertex) return 0;
            // G[i][0] leaves frontier
            int vi = G.frontier_index(G[i][0]);
            if (with_vertex) {
                if (not take and mate[vi] != INIT) return 0;
                if (take and mate[vi] == INIT) return 0;
            }
            // check component
            if (is_independent(mate, vi)) {
                if (find_other_component(mate, mate[vi])) return 0;
                return -1; // complete
            }
            mate[vi] = INIT;
        }
        else if (take) {
            int u = G[i][0], v = G[i][1];
            int ui = entry(mate, u), vi = entry(mate, v);
            if (non_cyclic and mate[ui] == mate[vi]) return 0; // cyclic
            connect(mate, ui, vi);
        }

        return (level > 1 ? level - 1 : 0);
    }
};
    
} // namespace sapporo_tdzdd_apps

#endif
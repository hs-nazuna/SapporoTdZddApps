#ifndef SAPPORO_TDZDD_APPS_GRAPH_DATA_HPP
#define SAPPORO_TDZDD_APPS_GRAPH_DATA_HPP

#include <istream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <cassert>

namespace sapporo_tdzdd_apps {

/*****
 * class Graph
 *      Maintain a graph data (allowing multiple edges).
 *      Assuming that vertex number is non negative,
 *      and it does not have to be 0-indexed and continuous.
 *      The graph can be treated as a directed graph such that
 *      an edge v0-v1 corresponds to an arc v0->v1.
 *      Here, a-b and b-a are distinguished.
 *      Note that the multiplicity of edges is treated as the directed version.
 *      So, for undirected graphs, it is to be desired that
 *      v0 < v1 holds for each edge v0-v1.
 * 
 * Graph()
 *      Construct empty graph.
 * 
 * void add_edge(int v0, int v1)
 *      Add an edge v0-v1.
 *      Note that this method takes O(log |V|) time.
 * 
 * int max_vertex_number() const
 *      Get the maximum vertex number.
 * 
 * int n_vertices() const
 *      Get the number of vertices.
 * 
 * int n_edges() const
 *      Get the number of edges.
 *  
 * void setup()
 *      Setup for subgraph enumeration.
 *      Note that this method takes O(|E| log |V|) time.
 * 
 * int n_items() const
 *      Get the number of items (vertices and edges).
 *      This function works after calling setup().
 * 
 * bool is_vertex(int i) const
 *      Check the i'th item is a vertex or not.
 *      This function works after calling setup().
 * 
 * int max_frontier_size() const
 *      Get the maximum frontier size.
 *      For subgraph enumeration.
 *      This function works after calling setup().
 * 
 * int frontier_index(int v) const
 *      Get the frontier index of v.
 *      For subgraph enumeration.
 *      This function works after calling setup().
 * 
 * const std::vector<int>& operator [](int i)
 *      Get i'th item.
 *      Vertex item is a singleton {vertex number}.
 *      Edge item is a vector of length 3
 *      {1st vertex, 2nd vertex, multiplicity on items {0, 1, ..., i}}.
 *      This function works after calling setup().
 * 
 * int var_of_vertex(int v) const
 *      Return the variable number of a vertex v.
 * 
 * int var_of_edge(int i) const
 *      Return the variable number of an edge i.
 * 
 * const std::vector<int>& vertex_vars() const
 *      Get a vector which maps a veterx to its variable number.
 * 
 * const std::vector<int>& edge_vars() const
 *      Get a vector which maps an edge to its variable number.
 *****/
class Graph {
private:
    std::set<int> vertex;
    std::vector<std::vector<int>> edge;

    std::vector<std::vector<int>> item;
    std::vector<int> v_to_item;
    std::vector<int> e_to_item;
    std::vector<int> f_index;
    int max_f_size;

public:
    Graph() : max_f_size(0) {}

    /***** for original graph *****/
    void add_edge(int v0, int v1) {
        assert(v0 >= 0 and v1 >=0 and v0 != v1);
        if (vertex.count(v0) == 0) vertex.insert(v0);
        if (vertex.count(v1) == 0) vertex.insert(v1);
        edge.push_back({v0, v1});
        max_f_size = 0;
    }

    int max_vertex_number() const {
        return *vertex.rbegin();
    }

    int n_vertices() const {
        return vertex.size();
    }

    int n_edges() const {
        return edge.size();
    }

    /***** for subgraph enumeration *****/
    void setup() {
        int n = *vertex.rbegin() + 1, m = n_edges();

        item.clear();
        v_to_item.assign(n, -1);
        e_to_item.assign(m, -1);
        f_index.assign(n, -1);
        max_f_size = 0;

        std::vector<int> edge_count(n, 0);
        std::map<std::vector<int>, int> multiplicity;
        for (int i = 0; i < m; ++i) {
            ++edge_count[edge[i][0]];
            ++edge_count[edge[i][1]];
            multiplicity[edge[i]] = 0;
        }

        std::vector<bool> visited(n, false);
        std::priority_queue<int, std::vector<int>, std::greater<int>> que;
        for (int i = 0; i < n; ++i) que.push(i);

        for (int i = 0; i < m; ++i){
            --edge_count[edge[i][0]];
            --edge_count[edge[i][1]];
            ++multiplicity[edge[i]];
            
            e_to_item[i] = item.size();
            item.push_back({edge[i][0], edge[i][1], multiplicity[edge[i]]});

            for (int j = 0; j < 2; ++j) {
                int v = edge[i][j];
                if (not visited[v]) {
                    visited[v] = true;
                    int idx = que.top(); que.pop();
                    f_index[v] = idx;
                    max_f_size = std::max(max_f_size, idx + 1);
                }
            }

            for (int j = 0; j < 2; ++j) {
                int v = edge[i][j];
                if (edge_count[v] == 0) {
                    v_to_item[v] = item.size();
                    item.push_back({v});
                    que.push(f_index[v]);
                }
            }
        }
    }

    int n_items() const {
        assert(max_f_size > 0);
        return item.size();
    }

    bool is_vertex(int i) const {
        assert(max_f_size > 0);
        assert(0 <= i and i < n_items());
        return item[i].size() == 1;
    }

    int max_frontier_size() const {
        assert(max_f_size > 0);
        return max_f_size;
    }

    int frontier_index(int v) const {
        assert(max_f_size > 0);
        assert(0 <= v and v <= *vertex.rbegin());
        assert(v_to_item[v] >= 0);
        return f_index[v];
    }

    const std::vector<int>& operator [](int i) const {
        assert(max_f_size > 0);
        assert(0 <= i and i < n_items());
        return item[i];
    }

    /***** for after subgraph enumeration *****/
    int var_of_vertex(int v) const {
        return v_to_item[v];
    }

    int var_of_edge(int i) const {
        return e_to_item[i];
    }

    const std::vector<int>& vertex_vars() const {
        return v_to_item;
    }

    const std::vector<int>& edge_vars() const {
        return e_to_item;
    }
}; // class Graph

} // namespace sapporo_tdzdd_apps

#endif
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
 * class GraphData
 *      Maintain a graph data (allowing multiple edges).
 *      Assuming that vertex number is non negative.
 *      The graph can be treated as a directed graph such that
 *      an edge v0-v1 corresponds to an arc v0->v1.
 *      Here, an edge a-b and an edge b-a are distinguished.
 * 
 * GraphData()
 *      Construct empty graph.
 * 
 * bool add_vertex(int v)
 *      Add a vertex v.
 *      If it already exists, return false. Otherwise, return true.
 * 
 * void add_edge(int v0, int v1)
 *      Add an edge v0-v1.
 *      v0 and v1 must be already added.
 * 
 * int n_vertices() const
 *      Get the number of vertices.
 * 
 * int n_edge() const
 *      Get the number of edges.
 * 
 * int n_items() const
 *      Get the number of items (vertices and edges).
 *      This function works after calling setup().
 * 
 * bool is_vertex(int i) const
 *      Check the i'th item is a vertex or not.
 *      This function works after calling setup().
 * 
 * int edge_multiplicity(int i) const
 *      Get the multiplicity of i'th item in {item 0, item 1, ..., item i}.
 *      Assuming that i'th item is an edge.
 *      This function works after calling setup().
 * 
 * void setup()
 *      Setup for subgraph enumeration.
 * 
 * int frontier_size() const
 *      Get frontier size for subgraph enumeration.
 *****/
class GraphData {
private:
    int v_offset;
    std::set<int> vertex;
    std::vector<std::vector<int>> edge;

    std::vector<std::vector<int>> graph_item;
    std::vector<int> vertex_index;
    std::vector<int> edge_index;
    std::vector<int> frontier_index;
    int f_size;

public:
    GraphData() : v_offset(-1) {}

    bool add_vertex(int v) {
        assert(v >= 0);
        if (vertex.count(v)) return false;
        vertex.insert(v);
        v_offset = -1;
        return true;
    }

    void add_edge(int v0, int v1) {
        assert(vertex.count(v0) and vertex.count(v1));
        edge.push_back({v0, v1});
        v_offset = -1;
    }

    int n_vertices() const {
        return vertex.size();
    }

    int n_edges() const {
        return edge.size();
    }

    int n_items() const {
        assert(v_offset >= 0);
        return graph_item.size();
    }

    bool is_vertex(int i) const {
        assert(v_offset >= 0);
        assert(0 <= i and i < n_items());
        return graph_item[i].size() == 1;
    }

    int edge_multiplicity(int i) const {
        assert(v_offset >= 0);
        assert(0 <= i and i < n_items());
        assert(not is_vertex(i));
        return graph_item[i][2];
    }

    void setup() {
        v_offset = *vertex.begin();
        int n = *vertex.rbegin() + 1, m = n_edges();

        graph_item.clear();
        vertex_index.assign(n, -1);
        edge_index.assign(m, -1);
        frontier_index.assign(n, -1);
        f_size = 0;

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
            
            graph_item.push_back(edge[i]);
            graph_item.back().push_back(multiplicity[edge[i]]);

            for (int j = 0; j < 2; ++j) {
                int v = edge[i][j];
                if (not visited[v]) {
                    visited[v] = true;
                    int idx = que.top(); que.pop();
                    frontier_index[v] = idx;
                    f_size = std::max(f_size, idx + 1);
                }
            }

            for (int j = 0; j < 2; ++j) {
                int v = edge[i][j];
                if (edge_count[v] == 0) {
                    graph_item.push_back({v});
                    que.push(frontier_index[v]);
                }
            }
        }
    }

    int frontier_size() const {
        assert(v_offset >= 0);
        return f_size;
    }
}; // class GraphData

} // namespace sapporo_tdzdd_apps

#endif
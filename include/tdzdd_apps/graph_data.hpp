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
 * int get_n_vertices() const
 *      Get the number of vertices.
 * 
 * int get_n_edge() const
 *      Get the number of edges.
 * 
 * const std::vector<int>& get_edge(int i) const
 *      Get i'th edge.
 * 
 * void setup()
 *      Setup for subgraph enumeration.
 *      Note that this method takes O(|E| log |V|) time.
 * 
 * int get_n_items() const
 *      Get the number of items (vertices and edges).
 *      This function works after calling setup().
 * 
 * bool is_vertex(int i) const
 *      Check the i'th item is a vertex or not.
 *      This function works after calling setup().
 * 
 * int get_frontier_size() const
 *      Get the maximum frontier size.
 *      For subgraph enumeration.
 *      This function works after calling setup().
 * 
 * int get_frontier_index(int v) const
 *      Get the frontier index of v.
 *      For subgraph enumeration.
 *      This function works after calling setup().
 * 
 * const std::vector<int>& get_item(int i)
 *      Get i'th item.
 *      Vertex item is a singleton {vertex number}.
 *      Edge item is a vector of length 3
 *      {1st vertex, 2nd vertex, multiplicity on items {0, 1, ..., i}}.
 *      This function works after calling setup().
 * 
 * int get_edge_multiplicity(int i) const
 *      Get the multiplicity of i'th item on {item 0, item 1, ..., item i}.
 *      Assuming that i'th item is an edge.
 *      This function works after calling setup().
 * 
 * int get_original_edge_index(int i) const
 *      Get the original index of i'th item which must be an edge.
 *      This function works after calling setup().
 * 
 * int get_item_index_of_vertex(int v) const
 *      Get the index of vertex v as an item.
 *      This function works after calling setup().
 * 
 * int get_item_index_of_edge(int i) const
 *      Get the index of i'th edge as an item.
 *      This function works after calling setup().
 *****/
class Graph {
private:
    std::set<int> vertex;
    std::vector<std::vector<int>> edge;

    std::vector<std::vector<int>> item;
    std::vector<int> vertex_to_item;
    std::vector<int> edge_to_item;
    std::vector<int> item_to_edge;
    std::vector<int> frontier_index;
    int frontier_size;

public:
    Graph() : frontier_size(0) {}

    /***** for original graph *****/
    void add_edge(int v0, int v1) {
        if (vertex.count(v0) == 0) vertex.insert(v0);
        if (vertex.count(v1) == 0) vertex.insert(v1);
        edge.push_back({v0, v1});
        frontier_size = 0;
    }

    int get_n_vertices() const {
        return vertex.size();
    }

    int get_n_edges() const {
        return edge.size();
    }

    const std::vector<int>& get_edge(int i) const {
        assert(0 <= i and i < get_n_edges());
        return edge[i];
    }

    /***** for subgraph enumeration *****/
    void setup() {
        int n = *vertex.rbegin() + 1, m = get_n_edges();

        item.clear();
        vertex_to_item.assign(n, -1);
        edge_to_item.assign(m, -1);
        item_to_edge.clear();
        frontier_index.assign(n, -1);
        frontier_size = 0;

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
            int mlt = ++multiplicity[edge[i]];
            
            edge_to_item[i] = item.size();
            item_to_edge.push_back(i);
            item.push_back({edge[i][0], edge[i][1], mlt});

            for (int j = 0; j < 2; ++j) {
                int v = edge[i][j];
                if (not visited[v]) {
                    visited[v] = true;
                    int idx = que.top(); que.pop();
                    frontier_index[v] = idx;
                    frontier_size = std::max(frontier_size, idx + 1);
                }
            }

            for (int j = 0; j < 2; ++j) {
                int v = edge[i][j];
                if (edge_count[v] == 0) {
                    vertex_to_item[v] = item.size();
                    item_to_edge.push_back(-1);
                    item.push_back({v});
                    que.push(frontier_index[v]);
                }
            }
        }
    }

    bool is_vertex(int i) const {
        assert(frontier_size > 0);
        assert(0 <= i and i < get_n_items());
        return item_to_edge[i] < 0;
    }

    int get_n_items() const {
        assert(frontier_size > 0);
        return item.size();
    }

    int get_frontier_size() const {
        assert(frontier_size > 0);
        return frontier_size;
    }

    int get_frontier_index(int v) const {
        assert(frontier_size > 0);
        assert(0 <= v and v <= *vertex.rbegin());
        assert(vertex_to_item[v] >= 0);
        return frontier_index[v];
    }

    const std::vector<int>& get_item(int i) const {
        assert(frontier_size > 0);
        assert(0 <= i and i < get_n_items());
        return item[i];
    }

    /***** for after subgraph enumeration *****/
    int get_edge_multiplicity(int i) const {
        assert(frontier_size > 0);
        assert(0 <= i and i < get_n_items());
        assert(not is_vertex(i));
        return item[i][2];
    }

    int get_original_edge_index(int i) const {
        assert(frontier_size > 0);
        assert(0 <= i and i < get_n_items());
        assert(not is_vertex(i));
        return item_to_edge[i];
    }

    int get_item_index_of_vertex(int v) const {
        assert(frontier_size > 0);
        assert(0 <= v and v <= *vertex.rbegin());
        assert(vertex_to_item[v] >= 0);
        return vertex_to_item[v];
    }

    int get_item_index_of_edge(int i) const {
        assert(frontier_size > 0);
        assert(0 <= i and i < get_n_edges());
        return edge_to_item[i];
    }
}; // class Graph

} // namespace sapporo_tdzdd_apps

#endif
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

#include "sapporo_tdzdd_apps/all_apps.hpp"
#include "instance_reader.hpp"
#include "naive_enumeration.hpp"
using namespace sapporo_tdzdd_apps;
using namespace tdzdd;

void dump_array(const vector<int>& v, std::ostream& os) {
    int n = v.size();
    for (int i = 0; i < n; ++i) os << v[i] << (i < n - 1 ? " " : "\n");
}

int main(int argc, char* argv[]) {
    bddinit(1000, 10000);

    MessageHandler::showMessages();

    if (argc > 1) {
        cout << "Test power set" << endl;
        ZBDD f = zbdd_power_set(3);

        cout << "----- TdZdd -----" << endl;
        DdStructure<2> dd = to_ddstructure(f);
        vector<vector<int>> tdzdd = unfold_ddstructure(3, dd, true);
        for (const vector<int>& ans : tdzdd) dump_array(ans, cout);

        cout << "----- ZBDD -----" << endl;
        vector<vector<int>> zbdd = unfold_zbdd(3, f, true);
        for (const vector<int>& ans : zbdd) dump_array(ans, cout);
        return 0;
    }

    string instance_type;
    cin >> instance_type;

    if (instance_type == "ineq") {
        cout << "Test inequalities" << endl;

        cout << "----- Instance -----" << endl;
        LinearInequalities ineq = read_linear_inequalities(cin);
        ineq.dump(cout);
        
        cout << "----- Naive Enumeration -----" << endl;
        vector<vector<int>> naive = naive_linear_inequarities(ineq);
        for (const vector<int>& ans : naive) dump_array(ans, cout);

        cout << "----- TdZdd -----" << endl;
        DdStructure<2> dd = tdzdd_linear_inequalities(ineq.A, ineq.sign, ineq.b);
        vector<vector<int>> tdzdd = unfold_ddstructure(ineq.n_vars, dd, true);
        for (const vector<int>& ans : tdzdd) dump_array(ans, cout);
        
        cout << "----- Check converter -----" << endl;
        ZBDD f = to_zbdd(dd);
        DdStructure<2> g = to_ddstructure(f);
        assert(dd == g);
        vector<vector<int>> zbdd = unfold_zbdd(ineq.n_vars, f, true);
        for (const vector<int>& ans : zbdd) dump_array(ans, cout);
    }

    if (instance_type == "graph") {
        cout << "Test graph data" << endl;

        Graph graph = read_graph(cin);
        int n_items = graph.n_items();
        cout << graph.n_vertices() << " " << graph.n_edges() << " " << n_items << endl;

        cout << "-----" << endl;
        for (int i = 0; i < n_items; ++i) {
            cout << "#" << i << (graph.is_vertex(i) ? " vertex" : " edge") << endl;
            vector<int> item_i = graph[i];
            dump_array(item_i, cout);
        }
        
        cout << "-----" << endl;
        cout << "frontier_size = " << graph.max_frontier_size() << endl;
        for (int i = 0; i < n_items; ++i) {
            if (graph.is_vertex(i)) {
                int v = graph[i][0];
                cout << v << " " << graph.frontier_index(v) << endl;
            }
        }
    }
}
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

#include "sapporo_tdzdd_apps/all_apps.hpp"
#include "instance_reader.hpp"
#include "naive_enumeration.hpp"
using namespace sapporo_tdzdd_apps;
using namespace tdzdd;

/***** sub functions *****/
void dump_array(const vector<int>& v, std::ostream& os) {
    int n = v.size();
    for (int i = 0; i < n; ++i) os << v[i] << (i < n - 1 ? " " : "\n");
}

Graph make_complete_graph(int n) {
    Graph G;
    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            G.add_edge(u, v);
        }
    }
    G.setup();
    return G;
}

Graph make_grid_graph(int n) {
    auto to_v = [&](int y, int x) { return y * n + x; };
    Graph G;
    for (int y = 0; y < n; ++y) for (int x = 0; x < n; ++x) {
        if (x < n - 1) G.add_edge(to_v(y, x), to_v(y, x + 1));
        if (y < n - 1) G.add_edge(to_v(y, x), to_v(y + 1, x));
    }
    G.setup();
    return G;
}

/***** tests *****/
void test_powerset() {
    cout << "Test power set" << endl;
    for (int n = 1; n <= 5; ++n) {
        cout << "n = " << n << endl;
        ZBDD f = zbdd_power_set(n);
        DdStructure<2> dd = to_ddstructure(f);
        vector<vector<int>> ans_tdzdd = unfold_ddstructure(n, dd, true);
        vector<vector<int>> ans_zbdd = unfold_zbdd(n, f, true);
        assert(ans_tdzdd == ans_zbdd);
        for (const vector<int>& ans : ans_zbdd) dump_array(ans, cout);
    }
}

void test_subset() {
    cout << "Test single subset ZBDD" << endl;
    ZBDD f = zbdd_single_subset({1, 2, 5, 8});
    vector<vector<int>> ans_zbdd = unfold_zbdd(10, f, true);
    for (const vector<int>& ans : ans_zbdd) dump_array(ans, cout);
}

void test_extraction() {
    cout << "Test zbdd_extraction" << endl;
    ZBDD f = zbdd_power_set(5);
    ZBDD g = zbdd_extraction(f, {1,3,5});
    vector<vector<int>> ans_zbdd = unfold_zbdd(5, g, true);
    for (const vector<int>& ans : ans_zbdd) dump_array(ans, cout);    
}

void test_tree_enumeration() {
    cout << "Test spanning tree enumeartion" << endl;

    cout << "----- Complete Graphs -----" << endl;
    for (int n = 3; n <= 6; ++n) {
        cout << "n = " << n << endl;
        Graph G = make_complete_graph(n);
        for (int wv = 0; wv < 2; ++wv) {
            DdStructure<2> dd = tdzdd_spanning_trees(G, wv);
            cout << dd.zddCardinality() << endl;
        }
        cout << "-----" << endl;
    }

    cout << "----- Grid Graphs ----" << endl;
    for (int n = 2; n <= 5; ++n) {
        cout << "n = " << n << endl;
        Graph G = make_grid_graph(n);
        for (int wv = 0; wv < 2; ++wv) {
            DdStructure<2> dd = tdzdd_spanning_trees(G, wv);
            cout << dd.zddCardinality() << endl;
        }
        cout << "-----" << endl;
    }
}

void test_path_enumeration() {
    cout << "Test s-t path enumeration" << endl;

    cout << "----- Complete Graphs -----" << endl;
    for (int n = 3; n <= 6; ++n) {
        cout << "n = " << n << endl;
        Graph G = make_complete_graph(n);
        for (int wv = 0; wv < 2; ++wv) {
            DdStructure<2> dd = tdzdd_st_paths(G, 0, n-1, wv);
            cout << dd.zddCardinality() << endl;
        }
        cout << "-----" << endl;
    }

    cout << "----- Grid Graphs ----" << endl;
    for (int n = 2; n <= 5; ++n) {
        cout << "n = " << n << endl;
        Graph G = make_grid_graph(n);
        for (int wv = 0; wv < 2; ++wv) {
            DdStructure<2> dd = tdzdd_st_paths(G, 0, n*n-1, wv);
            cout << dd.zddCardinality() << endl;
        }
        cout << "-----" << endl;
    }
}

void test_cycle_enumeration() {
    cout << "Test cycle enumeration" << endl;

    cout << "----- Complete Graphs -----" << endl;
    for (int n = 3; n <= 6; ++n) {
        cout << "n = " << n << endl;
        Graph G = make_complete_graph(n);
        for (int wv = 0; wv < 2; ++wv) {
            DdStructure<2> dd = tdzdd_cycles(G, wv);
            cout << dd.zddCardinality() << endl;
        }
        cout << "-----" << endl;
    }

    cout << "----- Grid Graphs ----" << endl;
    for (int n = 2; n <= 5; ++n) {
        cout << "n = " << n << endl;
        Graph G = make_grid_graph(n);
        for (int wv = 0; wv < 2; ++wv) {
            DdStructure<2> dd = tdzdd_cycles(G, wv);
            cout << dd.zddCardinality() << endl;
        }
        cout << "-----" << endl;
    }    
}

void test_linear_optimization() {
    vector<vector<int>> A = {{1, 2, 1, 2, 1, 2, 1}};
    vector<string> sign = {"<="};
    vector<int> b = {5};
    DdStructure<2> dd = tdzdd_linear_inequalities(A, sign, b);
    vector<int> cost = {2, 3, 1, 1, 2, 4, 1};
    LinearOptimization<int> opt;
    opt.set_dd(dd);
    auto res = opt.optimize(cost);
    cout << res.first << endl;
    vector<vector<int>> ans = unfold_zbdd(7, res.second);
    for (vector<int> X : ans) dump_array(X, cout);
}

int main(int argc, char* argv[]) {
    bddinit(10000, 1000000);
    MessageHandler::showMessages();
    string test_type(argv[1]);

    if (test_type == "-power") test_powerset();
    if (test_type == "-subset") test_subset();
    if (test_type == "-extract") test_extraction();
    if (test_type == "-tree") test_tree_enumeration();
    if (test_type == "-path") test_path_enumeration();
    if (test_type == "-cycle") test_cycle_enumeration();
    if (test_type == "-linear") test_linear_optimization();
}
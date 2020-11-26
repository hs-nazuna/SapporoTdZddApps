#ifndef SAPPORO_TDZDD_APPS_INSTANCE_READER_HPP
#define SAPPORO_TDZDD_APPS_INSTANCE_READER_HPP

#include "../include/tdzdd_apps/graph_data.hpp"

#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include <iostream>
using namespace std;

struct LinearInequalities {
    int n_vars;
    int n_rows;
    vector<vector<int>> A;
    vector<string> sign;
    vector<int> b;

    LinearInequalities(int n_vars, int n_rows)
    : n_vars(n_vars), n_rows(n_rows),
      A(n_rows, vector<int>(n_vars)), sign(n_rows), b(n_rows) {}

    void dump(ostream& os) {
        os << n_vars << " vars" << endl;
        os << n_rows << " inequalities" << endl;
        for (int r = 0; r < n_rows; ++r) {
            os << "  ";
            for (int i = 0; i < n_vars; ++i) {
                os << A[r][i] << " x" << i << (i < n_vars - 1 ? " + " : " ") ;
            }
            os << sign[r] << " " << b[r] << endl;
        }
    }
};

LinearInequalities read_linear_inequalities(istream& in) {
    int n_vars, n_rows;
    in >> n_vars >> n_rows;
    LinearInequalities instance(n_vars, n_rows);
    for (int r = 0; r < n_rows; ++r) {
        for (int i = 0; i < n_vars; ++i) in >> instance.A[r][i];
        in >> instance.sign[r];
        in >> instance.b[r];
    }
    return instance;
}

sapporo_tdzdd_apps::Graph read_graph(istream& in) {
    int n, m;
    in >> n >> m;
    sapporo_tdzdd_apps::Graph graph;
    for (int i = 0; i < m; ++i) {
        int v0, v1;
        in >> v0 >> v1;
        graph.add_vertex(v0);
        graph.add_vertex(v1);
        graph.add_edge(v0, v1);
    }
    graph.setup();
    return graph;
}

#endif
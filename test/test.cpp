#include <iostream>
#include <string>
using namespace std;

#include "../include/sapporo_tdzdd_apps.hpp"
#include "instance_reader.hpp"
#include "naive_enumeration.hpp"
using namespace sapporo_tdzdd_apps;
using namespace tdzdd;

void dump_array(const vector<int>& v, std::ostream& os) {
    int n = v.size();
    for (int i = 0; i < n; ++i) os << v[i] << (i < n - 1 ? " " : "\n");
}

int main() {
    MessageHandler::showMessages();

    string instance_type;
    cin >> instance_type;

    if (instance_type == "ineq") {
        LinearInequalities ineq = read_linear_inequalities(cin);
        ineq.dump(cout);
        
        cout << "----- Naive Enumeration -----" << endl;
        vector<vector<int>> naive = naive_linear_inequarities(ineq);
        for (const vector<int>& ans : naive) dump_array(ans, cout);

        cout << "----- TdZdd -----" << endl;
        DdStructure<2> dd = tdzdd_linear_inequalities(ineq.A, ineq.b, ineq.sign);
        vector<vector<int>> tdzdd = unfold_zdd(ineq.n_vars, dd, true);
        for (const vector<int>& ans : tdzdd) dump_array(ans, cout);
    }
}
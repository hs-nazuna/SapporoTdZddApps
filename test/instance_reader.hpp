#ifndef SAPPORO_TDZDD_APPS_INSTANCE_READER_HPP
#define SAPPORO_TDZDD_APPS_INSTANCE_READER_HPP

#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include <iostream>

struct LinearInequalities {
    int n_vars;
    int n_rows;
    std::vector<std::vector<int>> A;
    std::vector<int> b;
    std::vector<std::string> sign;

    LinearInequalities(int n_vars, int n_rows)
    : n_vars(n_vars), n_rows(n_rows),
      A(n_rows, std::vector<int>(n_vars)), b(n_rows), sign(n_rows) {}

    void dump(std::ostream& os) {
        os << n_vars << " " << n_rows << std::endl;
        for (int r = 0; r < n_rows; ++r) {
            for (int i = 0; i < n_vars; ++i) {
                os << A[r][i] << " x" << i << (i < n_vars - 1 ? " + " : " ") ;
            }
            os << sign[r] << " " << b[r] << std::endl;
        }
    }
};

LinearInequalities read_linear_inequalities(std::istream& in) {
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

#endif
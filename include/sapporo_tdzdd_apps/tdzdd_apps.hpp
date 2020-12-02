#ifndef SAPPORO_TDZDD_APPS_TDZDD_FUNCS_HPP
#define SAPPORO_TDZDD_APPS_TDZDD_FUNCS_HPP

#include <tdzdd/DdStructure.hpp>
#include "for_tdzdd/graph_data.hpp"
#include "for_tdzdd/component_spec.hpp"
#include "for_tdzdd/degree_spec.hpp"
#include "for_tdzdd/linear_spec.hpp"

namespace sapporo_tdzdd_apps {

/*****
 * tdzdd_linear_inequalities(A, sign, b)
 *      Construct DdStructure representing all the 0-1 valid assignments
 *      each of which satisfies all the given linear inequalities (Ax sign b).
 *      Inequality sings can be different for each row.
 *****/
tdzdd::DdStructure<2> tdzdd_linear_inequalities(
    const std::vector<std::vector<int>>& A,
    const std::vector<std::string>& sign,
    const std::vector<int>& b
) {
    LinearIneqSpec spec(A, sign, b);
    tdzdd::DdStructure<2> dd(spec);
    dd.zddReduce();
    return dd;
}

/*****
 * tdzdd_degree_constraints(G, lb, ub, with_vertex=false)
 *      Construct DdStructure representing all the valid subgraphs of G
 *      each of which satisfies a given degree constraint
 *      lb_v <= deg_v <= ub_v for each vertex.
 *****/
tdzdd::DdStructure<2> tdzdd_degree_constraints(
    const Graph& G,
    const std::vector<int>& lb,
    const std::vector<int>& ub,
    bool with_vertex = false
) {
    DegreeSpec spec(G, lb, ub, with_vertex);
    tdzdd::DdStructure<2> dd(spec);
    dd.zddReduce();
    return dd;
}

/*****
 * tdzdd_steiner(G, T, with_vertex=false)
 *      Construct DdStructure representing all the valid subgraphs of G
 *      each of which has all the vertices in T.
 *****/
tdzdd::DdStructure<2> tdzdd_steiner(
    const Graph& G,
    const std::set<int> T,
    bool with_vertex = false
) {
    int n = G.n_vertices(), m = G.n_edges();
    std::vector<int> lb(n, 0), ub(n, m);
    for (int v : T) lb[v] = 1;
    return tdzdd_degree_constraints(G, lb, ub, with_vertex);
}

/*****
 * unfold_ddstructure(n_vars, dd, sorted)
 *      Unfold a given DdStructure over n_vars variables.
 *      Each vector in return value represents a subset.
 *      If sorted = true, subsets are sorted in lexicographical order.
 *****/
std::vector<std::vector<int>> unfold_ddstructure(
    int n_vars,
    const tdzdd::DdStructure<2>& dd,
    bool sorted = false
) {
    std::vector<std::vector<int>> answer_set;
    for (auto path = dd.begin(); path != dd.end(); ++path) {
        std::vector<int> ans;
        for (auto it = path->begin(); it != path->end(); ++it) {
            int i = n_vars - *it;
            ans.push_back(i);
        }
        std::reverse(ans.begin(), ans.end());
        answer_set.push_back(ans);
    }
    if (sorted) std::sort(answer_set.begin(), answer_set.end());
    return answer_set;
}

} // namespace sapporo_tdzdd_apps

#endif
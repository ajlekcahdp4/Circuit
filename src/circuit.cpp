#include "circuit.hpp"

namespace Circuit
{

auto Circuit::make_slae() const -> MatrixSLAE
{
    MatrixSLAE slae (number_of_edges() + number_of_nodes());
    add_first_Kirchhof_rule_equations(slae);
    add_potential_difference_equations(slae);
    return slae;
}

void Circuit::add_first_Kirchhof_rule_equations(MatrixSLAE& slae) const
{
    for (size_type i = 0; i < number_of_nodes() - 1; ++i)
        std::copy(incidence_matrix_[i].cbegin(), incidence_matrix_[i].cend(), slae[i].begin());
}

void Circuit::add_potential_difference_equations(MatrixSLAE& slae) const
{
    slae[number_of_nodes() - 1][number_of_edges()] = 1.0; // equation phi0 == 0
    for (size_type i = 0; i < number_of_edges(); ++i)
    {
        auto& row = slae[number_of_nodes() + i];
        const auto& edge = edges_[i];

        row[i]     = edge.resistance_;
        row.back() = edge.emf_;
        row[number_of_edges() + index(edge.node1_)] = -1.0;
        row[number_of_edges() + index(edge.node2_)] = 1.0;
    }
}

auto Circuit::solve_circuit() const -> Solution
{
    auto slae = make_slae();
    auto currents = slae.solve_slae();
    if (currents.size() == 0)
        return Solution{};

    Solution solution {};
    solution.reserve(number_of_edges());
    for (size_type i = 0; i < number_of_edges(); ++i)
        solution.push_back(std::pair<Edge, double>(edges_[i], currents[i]));
    return solution;
}
} // namespace Circuit
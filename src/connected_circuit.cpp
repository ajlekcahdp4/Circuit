#include "connected_circuit.hpp"

namespace Circuit
{
// Complexity: O((N + E)^2)
auto ConnectedCircuit::make_slae() const -> MatrixSLAE
{
    MatrixSLAE slae (number_of_edges() + number_of_nodes()); // (N + E)^2 iterations
    add_first_Kirchhof_rule_equations(slae);                 // N * E iterations
    add_potential_difference_equations(slae);                // N * E iterations
    return slae;
}

// Cоmplexity: O(N * E)
void ConnectedCircuit::add_first_Kirchhof_rule_equations(MatrixSLAE& slae) const
{
    for (size_type i = 0; i < number_of_nodes() - 1; ++i) // N iterations
        std::copy(incidence_matrix_[i].cbegin(), incidence_matrix_[i].cend(), slae[i].begin()); // E iterations
}

// Complexity: O(N * E)
void ConnectedCircuit::add_potential_difference_equations(MatrixSLAE& slae) const
{
    slae[number_of_nodes() - 1][number_of_edges()] = 1.0; // equation phi0 == 0, N * E iterations
    for (size_type i = 0; i < number_of_edges(); ++i) // N iterations
    {
        auto& row = slae[number_of_nodes() + i];
        const auto& edge = edges_[i];

        row[i]     = edge.resistance_;
        row.back() = edge.emf_;
        row[number_of_edges() + index(edge.node1_)] = -1.0;
        row[number_of_edges() + index(edge.node2_)] = 1.0;
    }
}

// Complexity: O((N + E)^3)
auto ConnectedCircuit::solve_circuit() const -> Solution
{
    const auto& slae = make_slae();    // (N + E)^2 iterations
    const auto& currents = slae.solve_slae(); // (N + E)^3 iterations
    if (currents.size() == 0)
        return Solution{};

    Solution solution {};
    solution.reserve(number_of_edges());
    for (size_type i = 0; i < number_of_edges(); ++i) // E iterations
        solution.push_back(std::pair<Edge, double>(edges_[i], currents[i]));
    return solution;
}
} // namespace Circuit
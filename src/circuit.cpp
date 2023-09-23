#include "circuit.hpp"

namespace Circuit
{

bool operator==(const Edge& e1, const Edge& e2)
{
    return e1.node1_ == e2.node1_ && e1.node2_ == e2.node2_;
}

auto ConnectedCircuit::make_slae() const -> MatrixSLAE
{
    MatrixSLAE slae (number_of_edges() + number_of_nodes());
    add_first_Kirchhof_rule_equations(slae);
    add_potential_difference_equations(slae);
    return slae;
}

void ConnectedCircuit::add_first_Kirchhof_rule_equations(MatrixSLAE& slae) const
{
    for (size_type i = 0; i < number_of_nodes() - 1; ++i)
        std::copy(incidence_matrix_[i].cbegin(), incidence_matrix_[i].cend(), slae[i].begin());
}

void ConnectedCircuit::add_potential_difference_equations(MatrixSLAE& slae) const
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

auto ConnectedCircuit::solve_circuit() const -> Solution
{
    const auto& slae = make_slae();
    auto currents = slae.solve_slae();
    if (currents.size() == 0)
        return Solution{};

    Solution solution {};
    solution.reserve(number_of_edges());
    for (size_type i = 0; i < number_of_edges(); ++i)
        solution.push_back(std::pair<Edge, double>(edges_[i], currents[i]));
    return solution;
}

auto Circuit::make_nodes() const -> Nodes
{
    Nodes nodes {};

    auto first = edges_.cbegin(), last = edges_.cend();
    
    for (;first != last; ++first)
    {
        const auto& pair1 = nodes.insert(Node{first->node1_, Container::Vector{std::make_pair(first->node2_, std::to_address(first))}});
        const auto& pair2 = nodes.insert(Node{first->node2_, Container::Vector{std::make_pair(first->node1_, std::to_address(first))}});

        if (!pair1.second)
            pair1.first->second.push_back(std::make_pair(first->node2_, std::to_address(first)));
        if (!pair2.second)
            pair2.first->second.push_back(std::make_pair(first->node1_, std::to_address(first)));
    }

    return nodes;
}

auto Circuit::solve_circuit() const -> Solution
{
    Solution solution {};

    for (const auto& cir: cirs_)
    {
        const auto& sub_solution = cir.solve_circuit();
        solution.insert(sub_solution.cbegin(), sub_solution.cend());
    }

    return solution;
}
} // namespace Circuit
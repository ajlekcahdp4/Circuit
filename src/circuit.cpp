#include "circuit.hpp"

namespace Circuit
{
// Complexity: O(E)
auto Circuit::make_nodes() const -> Nodes
{
    Nodes nodes {};

    auto first = edges_.cbegin(), last = edges_.cend();
    
    for (;first != last; ++first) // O(E) iterations
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

// Complexity: O(max(C * (MN + ME)^3, ME * logE))
auto Circuit::solve_circuit() const -> Solution
{
    Solution solution {};

    for (const auto& cir: cirs_) // O(C) iterations
    {
        const auto& sub_solution = cir.solve_circuit(); // O((MN + ME)^3) iterations
        solution.insert(sub_solution.cbegin(), sub_solution.cend()); // O(ME * log(E)) iterations
    }

    return solution;
}
} // namespace Circuit
#include "circuit.hpp"

namespace Circuit
{
// Complexity: O(С * (MN + ME)^3)
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
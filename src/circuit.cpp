#include "circuit.hpp"

namespace Circuit
{
// Complexity: O(С * (MN + ME)^3)
auto Circuit::solve_circuit() const -> Solution
{
    Solution solution (number_of_edges_);

    for (const auto& cir: cirs_) // C iterations
    {
        const auto& sub_solution = cir.solve_circuit(); // (MN + ME)^3 iterations
        for (const auto& edge_cur: sub_solution) // ME iterations
            solution[edge_cur.first.ind_] = edge_cur;
    }

    return solution;
}
} // namespace Circuit
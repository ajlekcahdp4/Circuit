#include "circuit.hpp"
#include "input_output.hpp"

int main()
{
    auto edges = Circuit::InputOutput::input();
    Circuit::Circuit circuit (edges.cbegin(), edges.cend());
    auto solution = circuit.solve_circuit();
    Circuit::InputOutput::output(solution.cbegin(), solution.cend());
    return 0;
}
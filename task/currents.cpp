#include "circuit.hpp"
#include "input_output.hpp"

int main()
{
    try {
    auto edges = Circuit::InputOutput::input();
    Circuit::Circuit circuit (edges.cbegin(), edges.cend());
    auto solution = circuit.solve_circuit();
    Circuit::InputOutput::output(solution.cbegin(), solution.cend());
    } catch(std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
    return 0;
}
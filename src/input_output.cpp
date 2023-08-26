#include "input_output.hpp"
#include <cctype>

namespace Circuit
{
namespace InputOutput
{    
Edge scan_edge()
{
    unsigned node1 = 0, node2 = 0;
    double res = 0.0, emf = 0.0;
    char c = '\0';

    std::cin >> node1;

    while (!std::isdigit((std::cin >> c, c)) && !std::cin.eof()) {}
    std::cin.unget();
    std::cin >> node2;

    while (!std::isdigit((std::cin >> c, c)) && !std::cin.eof()) {}
    std::cin.unget();
    std::cin >> res;

    while (!std::isdigit((std::cin >> c, c)) && c != '-' && !std::cin.eof()) {}

    if (!std::cin.eof())
    {
        std::cin.unget();
        std::cin >> emf;
        while (!std::isdigit((std::cin >> c, c)) && !std::cin.eof()) {}
        if (std::isdigit(c))
            std::cin.unget();
    }

    return Edge(node1, node2, res, emf);
}

Container::Vector<Edge> input()
{
    Container::Vector<Edge> edges {};
    while (!std::cin.eof())
        edges.push_back(scan_edge());
    return edges;
}

void output(SolutionIt first, SolutionIt last)
{
    while (first != last)
    {
        std::cout << first->first.node1_ + 1 << " -- " << first->first.node2_ + 1 << ": ";
        std::cout << first->second << " A\n";
        ++first;
    }
    std::cout.flush();
}

} // namespace InputOutput
} // namespace Circuit
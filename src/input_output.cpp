#include "input_output.hpp"
#include <cctype>
#include <string>

namespace Circuit
{
namespace InputOutput
{    
Edge scan_edge(const std::string& str)
{
    Edge edge {};
    
    auto itr = str.cbegin();

    itr += std::sscanf(std::to_address(itr), "%u", &edge.node1_);
    --edge.node1_;
    
    for (;!std::isdigit(*itr) && itr != str.cend(); ++itr) {}

    if (itr == str.cend())
        throw std::logic_error{"invalid input of node2"};
    else
        --itr;

    itr += std::sscanf(std::to_address(itr), "%u", &edge.node2_);
    --edge.node2_;
    
    for (;!std::isdigit(*itr) && itr != str.cend(); ++itr) {}

    if (itr == str.cend())
        throw std::logic_error{"invalid input of resistance"};
    else
        --itr;
    
    itr += std::sscanf(std::to_address(itr), "%lf", &edge.resistance_);

    for (;!std::isdigit(*itr) && *itr != '-' && itr != str.cend(); ++itr) {}
    
    if (itr != str.cend())
        std::sscanf(std::to_address(--itr), "%lf", &edge.emf_);

    return edge;
}

Container::Vector<Edge> input()
{
    Container::Vector<Edge> edges {};
    std::string str {};
    while (std::getline(std::cin, str))
        edges.push_back(scan_edge(str));
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
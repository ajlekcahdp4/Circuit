#include "input_output.hpp"
#include <cctype>
#include <string>

namespace Circuit
{
namespace InputOutput
{    
Edge scan_edge(const std::string& str)
{
    unsigned node1 = 0, node2 = 0;
    double res = 0.0, emf = 0.0;
    
    std::size_t pos = 0;
    std::size_t i = 0;

    node1 = static_cast<unsigned>(std::stoi(str.c_str() + pos, &i));
    pos += i;

    for (;pos != str.size() && !std::isdigit(str[pos]); ++pos) {}
    if (pos == str.size())
        throw std::logic_error{"invalid input of node2"};
    else
        --pos;

    node2 = static_cast<unsigned>(std::stoi(str.c_str() + pos, &i));
    pos += i;
    
    for (;pos != str.size() && !std::isdigit(str[pos]); ++pos) {}
    if (pos == str.size())
        throw std::logic_error{"invalid input of resistance"};
    else
        --pos;
    
    res = std::stod(str.c_str() + pos, &i);
    pos += i;
    
    for (;pos != str.size() && !std::isdigit(str[pos]) && str[pos] != '-'; ++pos) {}
    if (pos != str.size())
    {
        --pos;
        emf = std::stod(str.c_str() + pos);
    }

    return Edge(node1, node2, res, emf);
}

typename Circuit::Edges input()
{
    typename Circuit::Edges edges {};
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
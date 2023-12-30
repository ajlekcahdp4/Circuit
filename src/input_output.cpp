#include "input_output.hpp"
#include <cctype>
#include <string>

namespace Circuit
{
namespace InputOutput
{   

using str_citr = typename std::string::const_iterator;

unsigned scan_unsigned(str_citr& itr)
{
    std::size_t pos = 0;
    auto val = static_cast<unsigned>(std::stoi(std::to_address(itr), &pos));
    itr += pos;
    return val;
}

double scan_double(str_citr& itr)
{
    std::size_t pos = 0;
    auto val = static_cast<double>(std::stod(std::to_address(itr), &pos));
    itr += pos;
    return val;
}

str_citr skip_to_unsigned(str_citr itr, str_citr end)
{
    for (;itr != end && !std::isdigit(*itr); ++itr) {}
    if (itr != end)
        --itr;
    return itr;
}

str_citr skip_to_signed(str_citr itr, str_citr end)
{
    for (;itr != end && !std::isdigit(*itr) && *itr != '-'; ++itr) {}
    if (itr != end)
        --itr;
    return itr;
}

IEdge scan_edge(const std::string& str)
{
    unsigned node1 = 0, node2 = 0;
    double res = 0.0, emf = 0.0;
    
    auto itr = str.cbegin();

    node1 = scan_unsigned(itr);

    itr = skip_to_unsigned(itr, str.cend());
    if (itr == str.cend())
        throw std::logic_error{"invalid input of node2"};
    node2 = scan_unsigned(itr);

    itr = skip_to_signed(itr, str.cend());
    if (itr == str.cend())
        throw std::logic_error{"invalid input of resistance"};    
    res = scan_double(itr);
    if (res < 0.0)
        throw std::logic_error{"you cannot input negative resistance"};

    itr = skip_to_signed(itr, str.cend());
    if (itr != str.cend())
        emf = scan_double(itr);

    return IEdge{node1, node2, res, emf};
}

Container::Vector<IEdge> input()
{
    Container::Vector<IEdge> edges {};
    std::string str {};
    while (std::getline(std::cin, str))
        edges.push_back(scan_edge(str));
    return edges;
}

void output(SolutionIt first, SolutionIt last)
{
    for (; first != last; ++first)
    {
        std::cout << first->first.node1_ << " -- " << first->first.node2_ << ": ";
        std::cout << first->second << " A\n";
    }
    std::cout.flush();
}

} // namespace InputOutput
} // namespace Circuit
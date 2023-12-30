#pragma once
#include "circuit.hpp"
#include <cassert>
#include <iterator>

namespace Circuit
{
namespace InputOutput
{
Container::Vector<IEdge> input();

using SolutionIt = typename Circuit::Solution::const_iterator;
void output(SolutionIt first, SolutionIt last);
} // namespace InputOutput
} // namespace Circuit

#pragma once
#include "circuit.hpp"
#include <cassert>
#include <iterator>

namespace Circuit
{
namespace InputOutput
{
Container::Vector<Edge> input();

using SolutionIt = typename Circuit::Solution::ConstIterator;
void output(SolutionIt first, SolutionIt last);
} // namespace InputOutput
} // namespace Circuit

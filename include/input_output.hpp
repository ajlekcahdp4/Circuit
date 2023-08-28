#pragma once
#include "circuit.hpp"
#include <cassert>
#include <iterator>

namespace Circuit
{
namespace InputOutput
{
typename Circuit::Edges input();

using SolutionIt = typename Circuit::Solution::ConstIterator;
void output(SolutionIt first, SolutionIt last);
} // namespace InputOutput
} // namespace Circuit

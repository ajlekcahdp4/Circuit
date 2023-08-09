#include <variant>
#include <vector>
#include <iostream>
#include "electrical_circuits.hpp"

namespace Circuit
{
namespace Lexer
{
struct Node {int num_;};
struct Resistance {double val_;};
struct Emf {double val_;};

using Lexem = std::variant<Node, Resistance, Emf>;

std::vector<Lexem> lexer(std::istream& input)
{
    std::vector<Lexem> lexems {};
    while (!input.eof())
    {
        int num = 0;
        std::string str {};
        input >> num >> str 
    }
}
} // namespace Lexer
} // namespace Circuit
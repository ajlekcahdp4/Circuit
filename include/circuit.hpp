#pragma once

#include <vector>
#include <pair>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <memory>

#include "matrix_arithmetic.hpp"

namespace Circuit
{
struct Edge
{
    unsigned node1_ = 0, node2_ = 0;
    double resistance_ = 0.0, emf_ = 0.0;
}; // struct Edge

class Circuit
{
    Container::Vector<Edge> edges_ = {};
    Matrix::MatrixContainer<bool> incidence_matrix_ = 
public:
    template<std::input_iterator InpIt>
    Circuit(It first, It last): edges_ (first, last) {}

}; // class Circuit
} // namespace Circuit
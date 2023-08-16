#pragma once

#include <tuple>
#include <iostream>
#include <algorithm>

#include "matrix_arithmetic.hpp"

namespace Circuit
{
struct Edge
{
    unsigned node1_ = 0, node2_ = 0;
    double resistance_ = 0.0, emf_ = 0.0;
    double current_ = 0.0;

    Edge(unsigned n1, unsigned n2, double res, double emf)
    :node1_ {n1}, node2_ {n2}, resistance_ {res}, emf_ {emf}
    {}
};

class Circuit
{
    using Edges = Container::Vector<Edge>;

    Edges edges_ = {};
    Matrix::MatrixContainer<bool> incidence_matrix_ = {};

    template<std::forward_iterator FwdIt>
    std::size_t calc_height(FwdIt first, FwdIt last)
    {
        unsigned max = 0;
        while (first != last)
        {
            if (first->node2_ > max)
                max = first->node2_;
            ++first;
        }
        return static_cast<std::size_t>(max);
    }       

public:
    explicit Circuit(Edges&& edges)
    :edges_ (std::move(edges)), incidence_matrix_ (calc_height(edges.cbegin(), edges.cend()), edges.size())
    {
        auto first = edges_.cbegin();
        auto last  = edges_.cend();
        for (std::size_t i = 0; first != last; i++, first++)
        {
            incidence_matrix_[first->node1_][i] = true;
            incidence_matrix_[first->node2_][i] = true;
        }
    }

    explicit Circuit(const Edges& edges): Circuit(Edges(edges)) {}
    template<std::input_iterator InpIt>
    Circuit(InpIt first, InpIt last): Circuit(Edges(first, last)) {}

}; // class Circuit
} // namespace Circuit
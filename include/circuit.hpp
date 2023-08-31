#pragma once

#include <tuple>
#include <iostream>
#include <algorithm>
#include <cassert>

#include "matrix_arithmetic.hpp"
#include "matrix_slae.hpp"

namespace Circuit
{
struct Edge
{
    unsigned node1_ = 0, node2_ = 0;
    double resistance_ = 0.0, emf_ = 0.0;
    Edge() = default;
    Edge(unsigned n1, unsigned n2, double r, double e = 0.0)
    :node1_ {n1 - 1}, node2_ {n2 - 1}, resistance_ {r}, emf_ {e}
    {}
}; // struct Edge
struct connection
{
    char value_;
    connection(int val = 0): value_ {static_cast<char>(val)} {}
    constexpr operator char() const noexcept {return value_;} 
    constexpr operator double() const noexcept {return static_cast<double>(value_);}
}; // struct connection 

const connection flow_in  = 1;
const connection flow_out = -1;
const connection not_connected = 0;

class Circuit final
{
public:
    using Solution = Container::Vector<std::pair<Edge, double>>;
    using Edges = Container::Vector<Edge>;

private:
    using size_type = std::size_t;
    
    struct DblCmp
    {
        bool operator()(double d1, double d2) const
        {
            return std::abs(d1 - d2) <= (std::abs(d1) + std::abs(d2)) * 1e-8;
        }
    };
    using MatrixSLAE     = Matrix::MatrixSLAE<double, DblCmp>;
    using MatrixIterator = MatrixSLAE::Iterator;

    // size() is M - number of edges
    Edges edges_ = {};
    // height() of matrix is N - number of nodes
    // width()  of matrix is M - number of edges
    // If edge I flow in node  J than           mat[I][J] == flow_in (1)
    // If edge I flow out node J than           mat[I][J] == flow_out (-1) 
    // If edge I not connected with node J than mat[I][J] == not_connected (0)
    Matrix::MatrixContainer<connection> incidence_matrix_ = {};

    template<std::forward_iterator FwdIt>
    std::size_t calc_height(FwdIt first, FwdIt last) const 
    {
        unsigned max = 0;
        while (first != last)
        {
            if (first->node2_ > max)
                max = first->node2_;
            ++first;
        }
        return static_cast<size_type>(max + 1);
    }       

public:
    Circuit(Edges&& edges)
    :edges_ (std::move(edges)), incidence_matrix_ (calc_height(edges_.cbegin(), edges_.cend()), edges_.size())
    {
        auto first = edges_.cbegin();
        auto last  = edges_.cend();

        for (size_type i = 0; first != last; ++first, ++i)
        {
            incidence_matrix_[first->node1_][i] = flow_out;
            incidence_matrix_[first->node2_][i] = flow_in;
        }
    }

    template<std::input_iterator InpIt>
    Circuit(InpIt first, InpIt last): Circuit(Edges(first, last)) {}

    Circuit(std::initializer_list<Edge> ilist): Circuit(ilist.begin(), ilist.end()) {}

    size_type number_of_nodes() const {return incidence_matrix_.height();}
    size_type number_of_edges() const {return edges_.size();}

private:
    // add N - 1 equations in slae matrix
    void add_first_Kirchhof_rule_equations(MatrixSLAE& slae) const;
    
    // add M + 1 equations in sale matrix
    void add_potential_difference_equations(MatrixSLAE& slae) const;

    MatrixSLAE make_slae() const;

public:
    Solution solve_circuit() const;
}; // class Circuit
} // namespace Circuit
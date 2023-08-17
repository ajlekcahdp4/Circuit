#pragma once

#include <tuple>
#include <iostream>
#include <algorithm>

#include "matrix_arithmetic.hpp"
#include "matrix_slae.hpp"
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
}; // struct Edge
struct connection
{
    char value_ = 0;
    constexpr operator char() const noexcept {return value_;} 
    constexpr operator double() const noexcept {return static_cast<double>(value_);}
}; // struct connection 

constexpr connection input  = 1;
constexpr connection output = -1;
constexpr connection not_connected = 0;

class Circuit
{
    using size_type = std::size_t;
    using Edges = Container::Vector<Edge>;

    struct DblCmp
    {
        bool operator()(double d1, double d2) const
        {
            return std::abs(d1 - d2) <= (std::abs(d1) + std::abs(d2)) * 1e-6;
        }
    };
    using MatrixSLAE = Matrix::MatrixSLAE<double, DblCmp>;

    Edges edges_ = {};
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
        return static_cast<size_type>(max);
    }       

public:
    explicit Circuit(Edges&& edges)
    :edges_ (std::move(edges)), incidence_matrix_ (calc_height(edges.cbegin(), edges.cend()), edges.size())
    {
        auto first = edges_.cbegin();
        auto last  = edges_.cend();
        for (std::size_t i = 0; first != last; i++, first++)
        {
            incidence_matrix_[first->node1_][i] = output;
            incidence_matrix_[first->node2_][i] = input;
        }
    }

private:
    void add_first_Kirchhof_rule_equations(MatrixSLAE& slae) const
    {
        for (size_type i = 0; i < incidence_matrix_.height() - 1; i++)
            std::copy(incidence_matrix_[i].cbegin(), incidence_matrix_[i].cend(), slae[i].begin());
    }

    void add_second_Kirchhof_rule_equations(MatrixSLAE& slae) const {}
    

    explicit Circuit(const Edges& edges): Circuit(Edges(edges)) {}
    template<std::input_iterator InpIt>
    Circuit(InpIt first, InpIt last): Circuit(Edges(first, last)) {}

    void solve_circuit() const
    {
        MatrixSLAE slae (edges_.size());

        add_first_Kirchhof_rule_equations(slae);
        add_second_Kirchhof_rule_equations(slae);
    
    }
}; // class Circuit
} // namespace Circuit
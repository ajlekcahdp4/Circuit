#pragma once

#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <set>

#include "matrix_arithmetic.hpp"
#include "matrix_slae.hpp"
#include "edge.hpp"

namespace Circuit
{
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

class ConnectedCircuit final
{
public:
    using EdgeCur   = std::pair<Edge, double>;
    using Solution  = Container::Vector<EdgeCur>;
    using Edges     = Container::Vector<Edge>;
    using size_type = typename Edges::size_type;

private:
    struct DblCmp
    {
        bool operator()(double d1, double d2) const
        {
            return std::abs(d1 - d2) <= (std::abs(d1) + std::abs(d2) + 1) * 1e-8;
        }
    };
    using MatrixSLAE     = Matrix::MatrixSLAE<double, DblCmp>;
    using MatrixIterator = MatrixSLAE::iterator;
    using Map            = std::unordered_map<unsigned, size_type>;

    // N - number of nodes
    // E - number of edges

    Edges edges_ = {};
    // height() of matrix is N
    // width()  of matrix is E
    // If edge I flow in node  J than           mat[I][J] == flow_in (1)
    // If edge I flow out node J than           mat[I][J] == flow_out (-1) 
    // If edge I not connected with node J than mat[I][J] == not_connected (0)
    Matrix::MatrixContainer<connection> incidence_matrix_ = {};
    
    Map nodes_to_indexis_ = {};

    // Complexity: O(E)
    template<std::forward_iterator FwdIt>
    static size_type calc_height(FwdIt first, FwdIt last)
    {
        std::unordered_set<unsigned> set_nodes {};
        for (; first != last; ++first) // E iterations
            set_nodes.insert({first->node1_, first->node2_});
        return set_nodes.size();
    }   

    // Complexity: O(E)
    template<std::forward_iterator FwdIt>
    static Map fill_nodes_to_indexis(FwdIt first, FwdIt last)
    {
        Map nodes_to_indexis {};
        for (size_type i = 0; first != last; ++first) // E iterations
        {
            if (nodes_to_indexis.insert(Map::value_type{first->node1_, i}).second)
                ++i;
            if (nodes_to_indexis.insert(Map::value_type{first->node2_, i}).second)
                ++i;
        }
        return nodes_to_indexis;
    }

    // Complexity: O(1)
    size_type index(unsigned node) const
    {
        return nodes_to_indexis_.find(node)->second;
    }

    // Complexity: O(E)
    template<std::input_iterator InpIt>
    Edges make_edges_from_input_edges(InpIt first, InpIt last)
    {
        Edges edges {};
        for (unsigned i = 0; first != last; ++first, ++i) // E iterations
            edges.push_back(Edge(*first, i));
        return edges;
    }

public:
    // Complexity: O(E)
    explicit ConnectedCircuit(Edges&& edges)
    :edges_ (std::move(edges)), incidence_matrix_ (calc_height(edges_.cbegin(), edges_.cend()), edges_.size()),
    nodes_to_indexis_ (fill_nodes_to_indexis(edges_.cbegin(), edges_.cend()))
    {
        auto first = edges_.cbegin();
        auto last  = edges_.cend();

        for (size_type i = 0; first != last; ++first, ++i) // E iterations
        {
            incidence_matrix_[index(first->node1_)][i] = flow_out;
            incidence_matrix_[index(first->node2_)][i] = flow_in;
        }
    }

    // Complexity: O(E)
    template<std::input_iterator InpIt>
    ConnectedCircuit(InpIt first, InpIt last)
    requires (std::is_same<typename std::remove_cvref_t<typename std::iterator_traits<InpIt>::value_type>, Edge>::value)
    :ConnectedCircuit(Edges(first, last))
    {}

    // Complexity: O(E)
    ConnectedCircuit(std::initializer_list<Edge> ilist): ConnectedCircuit(ilist.begin(), ilist.end()) {}

    // Complexity: O(E)
    template<std::input_iterator InpIt>
    ConnectedCircuit(InpIt first, InpIt last)
    requires (std::is_same<typename std::remove_cvref_t<typename std::iterator_traits<InpIt>::value_type>,
    InputOutput::InputEdge>::value)
    :ConnectedCircuit(make_edges_from_input_edges(first, last))
    {}

    // Complexity: O(E)
    ConnectedCircuit(std::initializer_list<InputOutput::InputEdge> ilist): ConnectedCircuit(ilist.begin(), ilist.end()) {}
    
    size_type number_of_nodes() const {return incidence_matrix_.height();}
    size_type number_of_edges() const {return edges_.size();}

private:
    // Complexity: O(N * E)
    // add N - 1 equations in slae matrix
    void add_first_Kirchhof_rule_equations(MatrixSLAE& slae) const;
    
    // Complexity: O(N * E)
    // add E + 1 equations in sale matrix
    void add_potential_difference_equations(MatrixSLAE& slae) const;

    // Complexity: O((N + E)^2)
    MatrixSLAE make_slae() const;

public:
    // Complexity: O((N + E)^3)
    Solution solve_circuit() const;
}; // class ConnectedCircuit
} // namespace Circuit
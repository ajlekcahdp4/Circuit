#pragma once

#include <tuple>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <set>

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
    :node1_ {n1}, node2_ {n2}, resistance_ {r}, emf_ {e}
    {}
}; // struct Edge

bool operator==(const Edge& e1, const Edge& e2);
} // namespace Circuit
template<> 
struct std::hash<Circuit::Edge>
{
    std::size_t operator()(const Circuit::Edge& edge) const noexcept
    {
        return edge.node1_ + edge.node2_;
    }
};

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
    using EdgeCur = std::pair<Edge, double>;
    using Solution = Container::Vector<EdgeCur>;
    using Edges = Container::Vector<Edge>;
    using size_type = std::size_t;

private:
    struct DblCmp
    {
        bool operator()(double d1, double d2) const
        {
            return std::abs(d1 - d2) <= (std::abs(d1) + std::abs(d2)) * 1e-8;
        }
    };
    using MatrixSLAE     = Matrix::MatrixSLAE<double, DblCmp>;
    using MatrixIterator = MatrixSLAE::Iterator;
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
    static std::size_t calc_height(FwdIt first, FwdIt last)
    {
        std::unordered_set<unsigned> set_nodes {};
        for (; first != last; ++first) // O(E) iterations
            set_nodes.insert({first->node1_, first->node2_});
        return set_nodes.size();
    }   

    // Complexity: O(E)
    template<std::forward_iterator FwdIt>
    static Map fill_nodes_to_indexis(FwdIt first, FwdIt last)
    {
        Map nodes_to_indexis {};
        for (size_type i = 0; first != last; ++first) // O(E) iterations
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

public:
    // Complexity: O(E)
    explicit ConnectedCircuit(Edges&& edges)
    :edges_ (std::move(edges)), incidence_matrix_ (calc_height(edges_.cbegin(), edges_.cend()), edges_.size()),
    nodes_to_indexis_ (fill_nodes_to_indexis(edges_.cbegin(), edges_.cend()))
    {
        auto first = edges_.cbegin();
        auto last  = edges_.cend();

        for (size_type i = 0; first != last; ++first, ++i) // O(E) iterations
        {
            incidence_matrix_[index(first->node1_)][i] = flow_out;
            incidence_matrix_[index(first->node2_)][i] = flow_in;
        }
    }

    // Complexity: O(E)
    template<std::input_iterator InpIt>
    ConnectedCircuit(InpIt first, InpIt last): ConnectedCircuit(Edges(first, last)) {}

    // Complexity: O(E)
    ConnectedCircuit(std::initializer_list<Edge> ilist): ConnectedCircuit(ilist.begin(), ilist.end()) {}

    size_type number_of_nodes() const {return incidence_matrix_.height();}
    size_type number_of_edges() const {return edges_.size();}

private:
    // Complexity: O(N * E)
    // add N - 1 equations in slae matrix
    void add_first_Kirchhof_rule_equations(MatrixSLAE& slae) const;
    
    // Complexity: O(N * E)
    // add M + 1 equations in sale matrix
    void add_potential_difference_equations(MatrixSLAE& slae) const;

    // Complexity: O((N + E)^2)
    MatrixSLAE make_slae() const;

public:
    // Complexity: O((N + E)^3)
    Solution solve_circuit() const;
}; // class ConnectedCircuit

class Circuit final
{
public:
    using size_type = std::size_t;
    using Edges = Container::Vector<Edge>;
    
    using EdgeCur = typename ConnectedCircuit::EdgeCur;
    struct EdgeCurLess
    {
        bool operator()(const EdgeCur& ec1, const EdgeCur& ec2) const noexcept
        {
            if (ec1.first.node1_ < ec2.first.node1_)
                return true;
            else if (ec1.first.node1_ > ec2.first.node1_)
                return false;
            else
                return ec1.first.node2_ < ec2.first.node2_;
        }
    };

    using Solution = std::set<EdgeCur, EdgeCurLess>;

private:
    // C - number of connected circuits in circuit (cirs_.size())
    // MN = max(N_1, N_2, ... N_C) - max number of nodes in connected circuit (N_i - cirs_[i].number_of_nodes())
    // ME = max(E_1, E_2, ... E_C) - max number of edges in connected circuit (E_i - cirs_[i].number_of_edges())
    // N - number of nodes (number_of_nodes_)
    // M - number of edges (edges_.size())
    Edges edges_ = {};
    Container::Vector<ConnectedCircuit> cirs_ = {};
    size_type number_of_nodes_ = 0;

    using Nodes = std::unordered_map<unsigned, Container::Vector<std::pair<unsigned, const Edge*>>>;
    using Node  = typename Nodes::value_type;

    // Complexity: O(E)
    Nodes make_nodes() const;

    // Complexity: O(1)
    static void add_node_in_connected_cir
    (Nodes& nodes, typename Nodes::iterator itr, Container::Vector<Node>& con_cir, size_type& nodes_placed)
    {
        con_cir.push_back(std::move(*itr));
        nodes.erase(itr);
        ++nodes_placed;
    }

    // Complexity: O(MN * ME)
    static std::pair<Container::Vector<Node>, size_type> make_connected_cir_as_nodes(Nodes& nodes)
    {
        size_type nodes_placed = 0;
        Container::Vector<Node> connected_cir {};
        add_node_in_connected_cir(nodes, nodes.begin(), connected_cir, nodes_placed);

        for (size_type i = 0; i < connected_cir.size(); ++i) // O(MN) iterations
            for (const auto& node: connected_cir[i].second) // O(ME) iterations
            {
                auto itr = nodes.find(node.first);
                if (itr != nodes.end())
                    add_node_in_connected_cir(nodes, itr, connected_cir, nodes_placed);
            }

        return {connected_cir, nodes_placed};
    }

    // Complexity: O(MN * ME)
    template<std::input_iterator InpIt> 
    static ConnectedCircuit make_connected_cir(InpIt first, InpIt last)
    {
        std::unordered_set<Edge> edges_set {};
        for (;first != last; ++first) // O(MN) iterations
            for (const auto& pair: first->second) // O(ME) iterations
                edges_set.insert(*pair.second);
        return ConnectedCircuit(edges_set.cbegin(), edges_set.cend()); // O(ME)
    }

public:
    // Complexity: O(C * MN * ME)
    explicit Circuit(Edges&& edges): edges_ (std::move(edges))
    {
        Nodes nodes = make_nodes(); // O(E) iterations
        number_of_nodes_ = nodes.size();

        size_type nodes_placed = 0;
        while (nodes_placed != number_of_nodes_) // O(C) iterations
        {
            const auto& pair = make_connected_cir_as_nodes(nodes); // O(MN * ME) iterations
            const auto& connected_cir = pair.first;
            nodes_placed += pair.second;
            cirs_.push_back(make_connected_cir(connected_cir.cbegin(), connected_cir.cend())); // O(MN * ME) iterations
        }
    }

    // Complexity: O(C * MN * ME)
    template<std::input_iterator InpIt>
    Circuit(InpIt first, InpIt last): Circuit(Edges(first, last)) {}
    // Complexity: O(C * MN * ME)
    Circuit(std::initializer_list<Edge> ilist): Circuit(ilist.begin(), ilist.end()) {}

    size_type number_of_edges() const {return edges_.size();}
    size_type number_of_nodes() const {return number_of_nodes_;}

    // Complexity: O(max(C * (MN + ME)^3, ME * logE)
    Solution solve_circuit() const;
}; // class Circuit
} // namespace Circuit
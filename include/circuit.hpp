#pragma once

#include <tuple>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <set>

#include "connected_circuit.hpp"

namespace Circuit
{
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
    // E - number of edges (edges_.size())
    // E/C <= ME <= E
    // N/C <= MC <= N
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

        return std::make_pair(std::move(connected_cir), std::move(nodes_placed));
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

    // Complexity: O(C * (MN + ME)^3)
    Solution solve_circuit() const;
}; // class Circuit
} // namespace Circuit
#pragma once

#include <vector>
#include <pair>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <memory>

#include "matrix_arithmetic.hpp"

namespace Circuit {

namespace detail 
{
class Edge;
using Node = std::pair<std::size_t, std::vector<Edge*>>;

class Edge
{
    std::size_t node_num1_ = 0, node_num2_ = 0;
    double resistance_ = 0.0, emf_ = 0.0;
    double current_ = 0.0;

public:
    Edge(std::size_t n1, std::size_t n2, double r, double e)
    :node_num1_ {n1}, node_num2_ {n2}, resistance_ {r}, emf_ {e}
    {}

    Edge(std::size_t n1, std::size_t n2, double r)
    :node_num1_ {n1}, node_num2_ {n2}, resistance_ {r}
    {}

    std::size_t node_num1() const {return node_num1_;}
    std::size_t node_num2() const {return node_num2_;}

    double resistance() const {return resistance_;}
    double emf() const {return emf_;}

    double& current() {return current_;}
    double current() const {return current_;}

    double voltage() const {return current_ * resistance_ + emf_;}

    bool is_valid() const {return node_num1_ != node_num2_;}
}; // class Edge
} // namespace detail

class Circuit
{
    using Node = detail::Node;
    using Edge = detail::Edge;

    using MatrixOfIncidence = Matrix::MatrixContainer<int>;

    std::unordered_map<std::size_t, Node> nodes_ = {};
    std::vector<std::unique_ptr<Edge>>    edges_ = {};

public:
    Circuit() = default;

    bool add_node(std::size_t node_num)
    {
        return nodes_.emplace(node_num, node, std::vector<Edge*>()).second;
    }

    bool add_edge(std::size_t n1, std::size_t n2, double resistance, double emf)
    {
        auto pedge = edges_.emplace_back(std::make_unique<Edge>(n1, n2, resistance, emf)).get();
        auto& itr1 = nodes_.find(n1);
        auto& itr2 = nodes_.find(n2);

        if (itr1 == nodes_.end() || itr2 == nodes_.end())
            return false;
        itr1->second.second.push_back(pedge);
        itr2->second.second.push_back(pedge);
        return true;
    }
}; // class Circuit
} // namespace Circuit
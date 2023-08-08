#include <vector>
#include <pair>
#include <memory>
#include <iostream>

namespace Circuit {

class Edge;
using Node = std::pair<int, std::vector<Edge*>>;

class Edge
{
    using node_ptr   = Node*;
    using c_node_ptr = const Node*;

    node_ptr node1_ = nullptr, node2_ = nullptr;
    double resistance_ = 0.0, emf_ = 0.0;
    double voltage_ = 0.0, current_ = 0.0;

public:
    Edge(node_ptr n1, node_ptr node2, double r, double e)
    :node1_ {n1}, node2_ {n2}, resistance_ {r}, emf_ {e}
    {}

    node_ptr   node1()       {return node1_;}
    c_node_ptr node1() const {return node1_;}
    node_ptr   node2()       {return node2_;}
    c_node_ptr node2() const {return node2_;}

    double resistance() const {return resistance_;}
    double emf() const {return emf_;}

    double& voltage() {return voltage_;}
    double& current() {return current_;}
    double voltage() const {return voltage_;}
    double current() const {return current_;}

    bool is_valid() const {return node1_ != node2_;}
}; // class Edge

class Circuit
{
    using node_ptr = Node*;

    std::vector<Node> nodes_;

public:
    Circuit(std::istream& input): nodes_ {}

}; // class Circuit

} // namespace Circuit
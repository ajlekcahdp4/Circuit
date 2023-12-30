#pragma once

#include <unordered_map>

namespace Circuit
{

namespace InputOutput
{
struct IEdge
{
    unsigned node1_ = 0, node2_ = 0;
    double resistance_ = 0.0, emf_ = 0.0;
}; // struct IOEdge
} // namespace InputOutput

struct Edge
{
    unsigned node1_ = 0, node2_ = 0;
    double resistance_ = 0.0, emf_ = 0.0;
    unsigned ind_ = 0;
    
    Edge() = default;
    Edge(unsigned n1, unsigned n2, double res, double emf, unsigned i)
    :node1_ {n1}, node2_ {n2}, resistance_ {res}, emf_ {emf}, ind_ {i}
    {}

    Edge(const InputOutput::IEdge& ie, unsigned ind = 0)
    :node1_ {ie.node1_}, node2_ {ie.node2_}, resistance_ {ie.resistance_}, emf_ {ie.emf_}, ind_ {ind}
    {}
}; // struct Edge

bool operator==(const Edge& e1, const Edge& e2);
} // namespace Circuit

template<> 
struct std::hash<Circuit::Edge>
{
    std::size_t operator()(const Circuit::Edge& edge) const noexcept
    {
        std::hash<unsigned> hash;
        return hash(edge.node1_) ^ hash(edge.node2_);
    }
};

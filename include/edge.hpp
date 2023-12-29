#pragma once

#include <unordered_map>

namespace Circuit
{
struct Edge
{
    unsigned node1_ = 0, node2_ = 0, ind_ = 0;
    double resistance_ = 0.0, emf_ = 0.0;
    Edge() = default;
    Edge(unsigned n1, unsigned n2, unsigned ind, double r, double e = 0.0)
    :node1_ {n1}, node2_ {n2}, ind_ {ind}, resistance_ {r}, emf_ {e}
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
        return hash(edge.node1_) ^ hash(edge.node2_) ^ hash(edge.ind_);
    }
};

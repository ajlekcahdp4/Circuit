#include "edge.hpp"

namespace Circuit
{
bool operator==(const Edge& e1, const Edge& e2)
{
    return e1.node1_ == e2.node1_ && e1.node2_ == e2.node2_;
}
} // namespace Circuit
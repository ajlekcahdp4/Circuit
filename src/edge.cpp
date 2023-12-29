#include "edge.hpp"

namespace Circuit
{
bool operator==(const Edge& e1, const Edge& e2)
{
    return e1.ind_ == e2.ind_;
}
} // namespace Circuit
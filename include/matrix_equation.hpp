#pragma once

#include "matrix_arithmetic.hpp"

namespace Matrix
{

template<typename T, class Cmp, class Abs = detail::DefaultAbs<T>>
class MatrixEquation : public MatrixArithmetic<T, true, Cmp, Abs>
{
    using base = MatrixArithmetic<T, true, Cmp, Abs>;
    using size_type = std::size_t;
    using value_type = T;

public:
    MatrixEquation(size_type h, size_type w): base(h, w) {}

    MatrixEquation(size_type h, size_type w, typename base::const_reference val)
    :base(h, w, val)
    {}

    template<std::input_iterator it>
    MatrixEquation(size_type h, size_type w, it begin, it end)
    :base(h, w, begin, end)
    {}

    MatrixEquation(value_type val = value_type{})
    :base(val)
    {}

    MatrixEquation(std::initializer_list<value_type> onedim_list)
    :base(onedim_list)
    {}

    MatrixEquation(std::initializer_list<std::initializer_list<value_type>> twodim_list)
    :base(twodim_list)
    {}

}; // class MatrixEquation

} // namespace Matrix
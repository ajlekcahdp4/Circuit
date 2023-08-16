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

    Container::Vector<value_type> solve_slae() const
    {
        if (this->width() != this->height() + 1)
            throw std::invalid_argument{"This Matrix isn't slae"};

        MatrixEquation cpy (*this);

        if (Cmp{}(cpy.make_upper_triangular_square(cpy.height()), value_type{}))
            return Container::Vector<value_type>{};
        cpy.make_eye_square_from_upper_triangular_square(cpy.height());

        Container::Vector<value_type> solution {};
        solution.reserve(cpy.height());
        for (const auto& row: cpy)
            solution.push_back(row.back());

        return solution;
    }
}; // class MatrixEquation

} // namespace Matrix
#pragma once

#include "matrix_arithmetic.hpp"

namespace Matrix
{

template<typename T, class Cmp, class Abs = detail::DefaultAbs<T>>
class MatrixSLAE : public MatrixArithmetic<T, true, Cmp, Abs>
{
    using base       = MatrixArithmetic<T, true, Cmp, Abs>;
    using size_type  = std::size_t;
    using value_type = T;

public:
    MatrixSLAE(size_type sz): base(sz, sz + 1) {}

    MatrixSLAE(size_type sz, typename base::const_reference val)
    :base(sz, sz + 1, val)
    {}

    template<std::input_iterator it>
    MatrixSLAE(size_type sz, it begin, it end)
    :base(sz, sz + 1, begin, end)
    {}

    MatrixSLAE(std::initializer_list<std::initializer_list<value_type>> twodim_list)
    :base(twodim_list)
    {
        if (this->width() != this->height() + 1)
            throw std::length_error{"invalid size of MatrixSLAE in ctor"};
    }

    Container::Vector<value_type> solve_slae() const
    {
        if (this->width() != this->height() + 1)
            throw std::invalid_argument{"This Matrix isn't slae"};

        MatrixSLAE cpy (*this);

        if (this->cmp(cpy.make_upper_triangular_square(cpy.height()), value_type{}))
            return Container::Vector<value_type>{};
        cpy.make_eye_square_from_upper_triangular_square(cpy.height());

        Container::Vector<value_type> solution {};
        solution.reserve(cpy.height());
        for (const auto& row: cpy)
            solution.push_back(row.back());

        return solution;
    }
}; // class MatrixSLAE
} // namespace Matrix
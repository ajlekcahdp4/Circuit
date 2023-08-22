#pragma once

#include "matrix_arithmetic.hpp"

namespace Matrix
{

template<typename T, class Cmp, class Abs = detail::DefaultAbs<T>>
class MatrixSLAE : public MatrixArithmetic<T, true, Cmp, Abs>
{
    using base = MatrixArithmetic<T, true, Cmp, Abs>;

    using typename base::size_type;
    using typename base::value_type;
    using typename base::reference;
    using typename base::const_reference;
    using typename base::pointer;
    using typename base::const_pointer;

    using typename base::Row;

    using typename base::RowIterator;
    using typename base::RowConstIterator;
    using typename base::Iterator;
    using typename base::ConstIterator;
    using typename base::ReverseIterator;
    using typename base::ConstReverseIterator;

public:
    MatrixSLAE(size_type sz): base(sz, sz + 1) {}

    MatrixSLAE(size_type sz, typename base::const_reference val)
    :base(sz, sz + 1, val)
    {}

    template<std::input_iterator InpIt>
    MatrixSLAE(size_type sz, InpIt begin, InpIt end)
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
        for (auto& row: cpy)
            solution.push_back(std::move(row.back()));

        return solution;
    }
}; // class MatrixSLAE
} // namespace Matrix
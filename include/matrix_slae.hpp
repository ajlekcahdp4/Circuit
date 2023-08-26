#pragma once

#include "matrix_arithmetic.hpp"
namespace Matrix
{

template<typename T, class Cmp, class Abs = detail::DefaultAbs<T>>
class MatrixSLAE : public MatrixArithmetic<T, true, Cmp, Abs>
{
public:
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

    MatrixSLAE(size_type sz): base(sz, sz + 1) {}

    template<std::input_iterator InpIt>
    MatrixSLAE(size_type sz, InpIt begin, InpIt end)
    :base(sz, sz + 1, begin, end)
    {}

    bool is_matrix_of_slae() const {return (this->width() == this->height() + 1);}

    MatrixSLAE(std::initializer_list<std::initializer_list<value_type>> twodim_list)
    :MatrixSLAE(twodim_list.size())
    {
        auto itr = this->begin();
        for (auto& row: twodim_list)
        {
            std::copy(row.begin(), row.end(), itr->begin());
            ++itr;
        }
    }

    Container::Vector<value_type> solve_slae() const
    {
        if (!is_matrix_of_slae())
            throw std::invalid_argument{"This Matrix isn't slae"};

        MatrixSLAE cpy (*this);

        cpy.make_upper_triangular_square(cpy.height());
        if (this->cmp(cpy.determinant_for_upper_triangular(cpy.height()), value_type{}))
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
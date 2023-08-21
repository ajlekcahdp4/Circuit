#include <gtest/gtest.h>

#include "matrix_slae.hpp"
#include "circuit.hpp"

struct DblCmp
{
    bool operator()(double d1, double d2) const
    {
        return std::abs(d1 - d2) <= (std::abs(d1) + std::abs(d2)) * 1e-5;
    }
};

TEST(Matrix, solve_slae)
{
    Matrix::MatrixSLAE<double, DblCmp> slae1 {{1, 1, 2}, {2, 1, 5}};
    const auto& solution1 = slae1.solve_slae();
    EXPECT_EQ(solution1[0], 3);
    EXPECT_EQ(solution1[1], -1);

    Matrix::MatrixSLAE<double, DblCmp> slae2
    {{1,   1,  1,  0,  0,  0, 0},
     {1,   0,  0, -1, -1,  0, 0},
     {0,   1,  0,  1,  0, -1, 0},
     {0,  10, -2,  0,  0,  5, 12},
     {4,   0, -2,  0, 22,  0, 12},
     {4, -10,  0, 60,  0,  0, 0}};

    const auto& solution2 = slae2.solve_slae();
    for (auto elem: solution2)
        std::cout << elem << std::endl;

    DblCmp dbl_cmp {};
    EXPECT_TRUE(dbl_cmp(solution2[0],  0.442958));
    EXPECT_TRUE(dbl_cmp(solution2[1],  0.631499 ));
    EXPECT_TRUE(dbl_cmp(solution2[2], -1.07446));
    EXPECT_TRUE(dbl_cmp(solution2[3],  0.0757193));
    EXPECT_TRUE(dbl_cmp(solution2[4],  0.367239 ));
    EXPECT_TRUE(dbl_cmp(solution2[5],  0.707219));
}

TEST(Circuit, solve_circuit)
{
    DblCmp dbl_cmp {};
    
    Circuit::Circuit cir1 (
        typename Circuit::Circuit::Edges({
        {1, 2, 4.0},
        {1, 3, 10.0},
        {1, 4, 2.0, -12.0},
        {2, 3, 60.0},
        {2, 4, 22.0},
        {3, 4, 5.0}
    }));

    const auto& solution1 = cir1.solve_circuit();
    EXPECT_TRUE(dbl_cmp(solution1[0],  0.442958));
    EXPECT_TRUE(dbl_cmp(solution1[1],  0.631499 ));
    EXPECT_TRUE(dbl_cmp(solution1[2], -1.07446));
    EXPECT_TRUE(dbl_cmp(solution1[3],  0.0757193));
    EXPECT_TRUE(dbl_cmp(solution1[4],  0.367239 ));
    EXPECT_TRUE(dbl_cmp(solution1[5],  0.707219));

    Circuit::Circuit cir2 (
        typename Circuit::Circuit::Edges({
        {1, 2, 1.0, 1.0},
        {1, 3, 0.0},
        {2, 3, 0.0}
    }));

    const auto& solution2 = cir2.solve_circuit();
    EXPECT_TRUE(dbl_cmp(solution2[0], 1.0));
    EXPECT_TRUE(dbl_cmp(solution2[1], -1.0));
    EXPECT_TRUE(dbl_cmp(solution2[2], 1.0));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
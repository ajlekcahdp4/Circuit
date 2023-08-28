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

using MatrixSLAE = Matrix::MatrixSLAE<double, DblCmp>;

bool dbl_cmp(double d1, double d2)
{
    return DblCmp{}(d1, d2);
}

TEST(MatrixSLAE, solve_slae)
{
    MatrixSLAE slae0 {{1, 1}};
    auto solution0 = slae0.solve_slae();
    EXPECT_EQ(solution0.size(), 1);
    EXPECT_EQ(solution0[0], 1);

    MatrixSLAE slae1 {{1, 1, 2}, {2, 1, 5}};
    auto solution1 = slae1.solve_slae();
    EXPECT_EQ(solution1[0], 3);
    EXPECT_EQ(solution1[1], -1);

    MatrixSLAE slae2
    {{1,   1,  1,  0,  0,  0, 0},
     {1,   0,  0, -1, -1,  0, 0},
     {0,   1,  0,  1,  0, -1, 0},
     {0,  10, -2,  0,  0,  5, 12},
     {4,   0, -2,  0, 22,  0, 12},
     {4, -10,  0, 60,  0,  0, 0}};
    auto solution2 = slae2.solve_slae();
    EXPECT_TRUE(dbl_cmp(solution2[0],  0.442958));
    EXPECT_TRUE(dbl_cmp(solution2[1],  0.631499 ));
    EXPECT_TRUE(dbl_cmp(solution2[2], -1.07446));
    EXPECT_TRUE(dbl_cmp(solution2[3],  0.0757193));
    EXPECT_TRUE(dbl_cmp(solution2[4],  0.367239 ));
    EXPECT_TRUE(dbl_cmp(solution2[5],  0.707219));

    MatrixSLAE slae3 {{1, 1, 2}, {1, 1, 2}};
    auto solution3 = slae3.solve_slae();
    EXPECT_EQ(solution3.size(), 0);

    MatrixSLAE slae4 {
        {1, 1, 1, 3},
        {1, 2, 1, 4},
        {1, 1, 1, 3}
    };
    auto solution4 = slae4.solve_slae();
    EXPECT_EQ(solution4.size(), 0);
}

TEST(Circuit, solve_circuitCommonCases)
{
    Circuit::Circuit cir1 {
        {1, 2, 4.0},
        {1, 3, 10.0},
        {1, 4, 2.0, -12.0},
        {2, 3, 60.0},
        {2, 4, 22.0},
        {3, 4, 5.0}
    };

    auto solution1 = cir1.solve_circuit();
    EXPECT_TRUE(dbl_cmp(solution1[0].second,  0.442958));
    EXPECT_TRUE(dbl_cmp(solution1[1].second,  0.631499 ));
    EXPECT_TRUE(dbl_cmp(solution1[2].second, -1.07446));
    EXPECT_TRUE(dbl_cmp(solution1[3].second,  0.0757193));
    EXPECT_TRUE(dbl_cmp(solution1[4].second,  0.367239 ));
    EXPECT_TRUE(dbl_cmp(solution1[5].second,  0.707219));

    Circuit::Circuit cir2 {
        {1, 2, 1.0, 1.0},
        {1, 3, 0.0},
        {2, 3, 0.0}
    };

    auto solution2 = cir2.solve_circuit();
    EXPECT_TRUE(dbl_cmp(solution2[0].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution2[1].second, -1.0));
    EXPECT_TRUE(dbl_cmp(solution2[2].second, 1.0));
}

TEST(Circuit, solve_circuitCornerCase)
{
    Circuit::Circuit cir1 {
        {1, 2, 2.0, 1.0}
    };

    auto solution1 = cir1.solve_circuit();
    EXPECT_TRUE(dbl_cmp(solution1[0].second, 0.0));

    Circuit::Circuit cir2 {
        {1, 2, 2.0, 2.0},
        {1, 3, 2.0, 1.0}
    };

    auto solution2 = cir2.solve_circuit();
    EXPECT_TRUE(dbl_cmp(solution2[0].second, 0.0));
    EXPECT_TRUE(dbl_cmp(solution2[1].second, 0.0));

    Circuit::Circuit cir3 {
        {1, 2, 1.0, 4.0},
        {1, 4, 1.0},
        {1, 5, 1.0, 10.0},
        {2, 3, 1.0},
        {3, 4, 1.0}
    };

    auto solution3 = cir3.solve_circuit();
    EXPECT_TRUE(dbl_cmp(solution3[0].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution3[1].second, -1.0));
    EXPECT_TRUE(dbl_cmp(solution3[2].second, 0.0));
    EXPECT_TRUE(dbl_cmp(solution3[3].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution3[4].second, 1.0));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

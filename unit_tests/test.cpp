#include <gtest/gtest.h>

#include "matrix_slae.hpp"
#include "circuit.hpp"

struct DblCmp
{
    bool operator()(double d1, double d2) const
    {
        return std::abs(d1 - d2) <= (std::abs(d1) + std::abs(d2) + 1) * 1e-5;
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
    const auto& solution0 = slae0.solve_slae();
    EXPECT_EQ(solution0.size(), 1);
    EXPECT_EQ(solution0[0], 1);

    MatrixSLAE slae1 {{1, 1, 2}, {2, 1, 5}};
    const auto& solution1 = slae1.solve_slae();
    EXPECT_EQ(solution1[0], 3);
    EXPECT_EQ(solution1[1], -1);

    MatrixSLAE slae2
    {{1,   1,  1,  0,  0,  0, 0},
     {1,   0,  0, -1, -1,  0, 0},
     {0,   1,  0,  1,  0, -1, 0},
     {0,  10, -2,  0,  0,  5, 12},
     {4,   0, -2,  0, 22,  0, 12},
     {4, -10,  0, 60,  0,  0, 0}};
    const auto& solution2 = slae2.solve_slae();
    EXPECT_TRUE(dbl_cmp(solution2[0],  0.442958));
    EXPECT_TRUE(dbl_cmp(solution2[1],  0.631499 ));
    EXPECT_TRUE(dbl_cmp(solution2[2], -1.07446));
    EXPECT_TRUE(dbl_cmp(solution2[3],  0.0757193));
    EXPECT_TRUE(dbl_cmp(solution2[4],  0.367239 ));
    EXPECT_TRUE(dbl_cmp(solution2[5],  0.707219));

    MatrixSLAE slae3 {{1, 1, 2}, {1, 1, 2}};
    const auto& solution3 = slae3.solve_slae();
    EXPECT_EQ(solution3.size(), 0);

    MatrixSLAE slae4 {
        {1, 1, 1, 3},
        {1, 2, 1, 4},
        {1, 1, 1, 3}
    };
    const auto& solution4 = slae4.solve_slae();
    EXPECT_EQ(solution4.size(), 0);
}

TEST(ConnectedCircuit, solve_circuitCommonCases)
{
    Circuit::ConnectedCircuit cir1 {
        {1, 2, 4.0},
        {1, 3, 10.0},
        {1, 4, 2.0, -12.0},
        {2, 3, 60.0},
        {2, 4, 22.0},
        {3, 4, 5.0}
    };
    const auto& solution1 = cir1.solve_circuit();
    EXPECT_EQ(solution1.size(), 6);
    if (!solution1.empty())
    {
        EXPECT_TRUE(dbl_cmp(solution1[0].second,  0.442958));
        EXPECT_TRUE(dbl_cmp(solution1[1].second,  0.631499 ));
        EXPECT_TRUE(dbl_cmp(solution1[2].second, -1.07446));
        EXPECT_TRUE(dbl_cmp(solution1[3].second,  0.0757193));
        EXPECT_TRUE(dbl_cmp(solution1[4].second,  0.367239 ));
        EXPECT_TRUE(dbl_cmp(solution1[5].second,  0.707219));
    }

    Circuit::ConnectedCircuit cir2 {
        {1, 2, 1.0, 1.0},
        {1, 3, 0.0},
        {2, 3, 0.0}
    };
    const auto& solution2 = cir2.solve_circuit();
    EXPECT_EQ(solution2.size(), 3);
    if (!solution2.empty())
    {
        EXPECT_TRUE(dbl_cmp(solution2[0].second, 1.0));
        EXPECT_TRUE(dbl_cmp(solution2[1].second, -1.0));
        EXPECT_TRUE(dbl_cmp(solution2[2].second, 1.0));
    }
}

TEST(ConnectedCircuit, solve_circuitMultiEdgeCase)
{
    Circuit::ConnectedCircuit cir1 {
        {1, 2, 1.0, 2.0},
        {1, 2, 1.0}
    };
    const auto& solution1 = cir1.solve_circuit();
    EXPECT_EQ(solution1.size(), 2);
    if (!solution1.empty())
    {
        EXPECT_TRUE(dbl_cmp(solution1[0].second, 1.0));
        EXPECT_TRUE(dbl_cmp(solution1[1].second, -1.0));
    }

    Circuit::ConnectedCircuit cir2 {
        {1, 2, 1.0, -10.0},
        {2, 3, 17.0, 135.0},
        {2, 4, 14.0, 33.0},
        {3, 5, 1.0, 2.0},
        {3, 5, 1.0}  
    };
    const auto& solution2 = cir2.solve_circuit();
    EXPECT_EQ(solution2.size(), 5);
    EXPECT_TRUE(dbl_cmp(solution2[0].second, 0.0));
    EXPECT_TRUE(dbl_cmp(solution2[1].second, 0.0));
    EXPECT_TRUE(dbl_cmp(solution2[2].second, 0.0));
    EXPECT_TRUE(dbl_cmp(solution2[3].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution2[4].second, -1.0));

    for (std::size_t i = 0; i < solution2.size(); ++i)
        std::cout << i << ": " << solution2[i].second << std::endl;
}

TEST(ConnectedCircuit, solve_circuitCornerCase)
{
    Circuit::ConnectedCircuit cir1 {
        {1, 2, 2.0, 1.0}
    };
    const auto& solution1 = cir1.solve_circuit();
    EXPECT_EQ(solution1.size(), 1);
    EXPECT_TRUE(dbl_cmp(solution1[0].second, 0.0));

    Circuit::ConnectedCircuit cir2 {
        {1, 2, 2.0, 2.0},
        {1, 3, 2.0, 1.0}
    };
    const auto& solution2 = cir2.solve_circuit();
    EXPECT_EQ(solution2.size(), 2);
    EXPECT_TRUE(dbl_cmp(solution2[0].second, 0.0));
    EXPECT_TRUE(dbl_cmp(solution2[1].second, 0.0));

    Circuit::ConnectedCircuit cir3 {
        {1, 2, 1.0, 4.0},
        {1, 4, 1.0},
        {1, 5, 1.0, 10.0},
        {2, 3, 1.0},
        {3, 4, 1.0}
    };
    const auto& solution3 = cir3.solve_circuit();
    EXPECT_EQ(solution3.size(), 5);
    EXPECT_TRUE(dbl_cmp(solution3[0].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution3[1].second, -1.0));
    EXPECT_TRUE(dbl_cmp(solution3[2].second, 0.0));
    EXPECT_TRUE(dbl_cmp(solution3[3].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution3[4].second, 1.0));
}

TEST(Circuit, solve_circuitConnectedCase)
{
    Circuit::Circuit cir1 {
        {1, 2, 4.0},
        {1, 3, 10.0},
        {1, 4, 2.0, -12.0},
        {2, 3, 60.0},
        {2, 4, 22.0},
        {3, 4, 5.0}
    };
    EXPECT_EQ(cir1.number_of_connected_circuits(), 1);
    const auto& solution1 = cir1.solve_circuit();
    EXPECT_EQ(solution1.size(), 6);
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
    EXPECT_EQ(cir2.number_of_connected_circuits(), 1);
    const auto& solution2 = cir2.solve_circuit();
    EXPECT_EQ(solution2.size(), 3);
    EXPECT_TRUE(dbl_cmp(solution2[0].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution2[1].second, -1.0));
    EXPECT_TRUE(dbl_cmp(solution2[2].second, 1.0));

    Circuit::Circuit cir3 {
        {1, 2, 2.0, 1.0}
    };
    EXPECT_EQ(cir3.number_of_connected_circuits(), 1);
    const auto& solution3 = cir3.solve_circuit();
    EXPECT_EQ(solution3.size(), 1);
    EXPECT_TRUE(dbl_cmp(solution3.front().second, 0.0));

    Circuit::Circuit cir4 {
        {1, 2, 2.0, 2.0},
        {1, 3, 2.0, 1.0}
    };
    EXPECT_EQ(cir4.number_of_connected_circuits(), 1);
    const auto& solution4 = cir4.solve_circuit();
    EXPECT_EQ(solution4.size(), 2);
    EXPECT_TRUE(dbl_cmp(solution4[0].second, 0.0));
    EXPECT_TRUE(dbl_cmp(solution4[1].second, 0.0));

    Circuit::Circuit cir5 {
        {1, 2, 1.0, 4.0},
        {1, 4, 1.0},
        {1, 5, 1.0, 10.0},
        {2, 3, 1.0},
        {3, 4, 1.0}
    };
    EXPECT_EQ(cir5.number_of_connected_circuits(), 1);
    const auto& solution5 = cir5.solve_circuit();
    EXPECT_EQ(solution5.size(), 5);
    EXPECT_TRUE(dbl_cmp(solution5[0].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution5[1].second, -1.0));
    EXPECT_TRUE(dbl_cmp(solution5[2].second, 0.0));
    EXPECT_TRUE(dbl_cmp(solution5[3].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution5[4].second, 1.0));
}

TEST(Circuit, solve_circuitDisconnectedCase)
{
    Circuit::Circuit cir1 {
        {1, 2, 1.0},
        {1, 3, 1.0},
        {2, 3, 1.0, 3.0},
        {4, 5, 1.0},
        {4, 6, 1.0},
        {5, 6, 1.0, 3.0}
    };
    EXPECT_EQ(cir1.number_of_connected_circuits(), 2);
    EXPECT_EQ(cir1.number_of_edges(), 6);
    EXPECT_EQ(cir1.number_of_nodes(), 6);
    const auto& solution1 = cir1.solve_circuit();
    EXPECT_EQ(solution1.size(), 6);
    EXPECT_TRUE(dbl_cmp(solution1[0].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution1[1].second, -1.0));
    EXPECT_TRUE(dbl_cmp(solution1[2].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution1[3].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution1[4].second, -1.0));
    EXPECT_TRUE(dbl_cmp(solution1[5].second, 1.0));

    Circuit::Circuit cir2 {
        {1, 4, 1.0, 5.0},
        {1, 5, 1.0},
        {2, 4, 1.0},
        {2, 7, 1.0},
        {3, 6, 1.0, 5.0},
        {3, 9, 1.0},
        {5, 7, 1.0},
        {6, 10, 1.0},
        {8, 9, 1.0},
        {8, 10, 1.0},
        {11, 12, 1.0, 3.0},
        {11, 13, 1.0},
        {12, 13, 1.0}
    };
    EXPECT_EQ(cir2.number_of_connected_circuits(), 3);
    EXPECT_EQ(cir2.number_of_nodes(), 13);
    EXPECT_EQ(cir2.number_of_edges(), 13);
    const auto& solution2 = cir2.solve_circuit();
    EXPECT_EQ(solution2.size(), 13);
    EXPECT_TRUE(dbl_cmp(solution2[0].second, 1.0)); // 1 -- 4
    EXPECT_TRUE(dbl_cmp(solution2[1].second, -1.0)); // 1 -- 5
    EXPECT_TRUE(dbl_cmp(solution2[2].second, -1.0)); // 2 -- 4
    EXPECT_TRUE(dbl_cmp(solution2[3].second, 1.0)); // 2 -- 7
    EXPECT_TRUE(dbl_cmp(solution2[4].second, 1.0)); // 3 -- 6
    EXPECT_TRUE(dbl_cmp(solution2[5].second, -1.0)); // 3 -- 9
    EXPECT_TRUE(dbl_cmp(solution2[6].second, -1.0)); // 5 -- 7
    EXPECT_TRUE(dbl_cmp(solution2[7].second, 1.0)); // 6 -- 10
    EXPECT_TRUE(dbl_cmp(solution2[8].second, 1.0)); // 8 -- 9
    EXPECT_TRUE(dbl_cmp(solution2[9].second, -1.0)); // 8 -- 10
    EXPECT_TRUE(dbl_cmp(solution2[10].second, 1.0)); // 11 -- 12
    EXPECT_TRUE(dbl_cmp(solution2[11].second, -1.0)); // 11 -- 13
    EXPECT_TRUE(dbl_cmp(solution2[12].second, 1.0)); // 12 -- 13
}

TEST(Circuit, solve_circuitMultiEdgeCase)
{
    Circuit::Circuit cir1 {
        {1, 2, 1.0, 2.0},
        {1, 2, 1.0}
    };
    EXPECT_EQ(cir1.number_of_connected_circuits(), 1);
    const auto& solution1 = cir1.solve_circuit();
    EXPECT_EQ(solution1.size(), 2);
    EXPECT_TRUE(dbl_cmp(solution1[0].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution1[1].second, -1.0));

    Circuit::Circuit cir2 {
        {1, 2, 1.0, 2.0},
        {1, 2, 1.0},
        {3, 4, 1.0, 2.0},
        {3, 4, 1.0}
    };
    EXPECT_EQ(cir2.number_of_connected_circuits(), 2);
    const auto& solution2 = cir2.solve_circuit();
    EXPECT_EQ(solution2.size(), 4);
    EXPECT_TRUE(dbl_cmp(solution2[0].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution2[1].second, -1.0));
    EXPECT_TRUE(dbl_cmp(solution2[2].second, 1.0));
    EXPECT_TRUE(dbl_cmp(solution2[3].second, -1.0));

    Circuit::Circuit cir3 {
        {1, 2, 0.0, 10.0},
        {1, 2, 1.0},
        {1, 3, 3.0},
        {2, 3, 10.0, 20.0},
        {2, 3, 2.0}
    };
    EXPECT_EQ(cir3.number_of_connected_circuits(), 1);
    const auto& solution3 = cir3.solve_circuit();
    EXPECT_EQ(solution3.size(), 5);
    EXPECT_TRUE(dbl_cmp(solution3[0].second, 12.85714));
    EXPECT_TRUE(dbl_cmp(solution3[1].second, -10.0));
    EXPECT_TRUE(dbl_cmp(solution3[2].second, -2.85714));
    EXPECT_TRUE(dbl_cmp(solution3[3].second, 2.14286));
    EXPECT_TRUE(dbl_cmp(solution3[4].second, 0.714286));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

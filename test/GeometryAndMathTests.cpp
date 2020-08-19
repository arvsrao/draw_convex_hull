#include <gtest/gtest.h>
#include <include/RowColumnMatrix.h>
#include <include/SquareMatrix.h>

TEST(Matrix, creationTest) {
  std::array<double, 4> inital_array = {2.0, 0.0, 0.0, 1.0};
  auto a                             = RowColumnMatrix<2, 2, double>(inital_array);

  EXPECT_EQ(a(0, 0), inital_array[0]);
  EXPECT_EQ(a(0, 1), inital_array[1]);
  EXPECT_EQ(a(1, 0), inital_array[2]);
  EXPECT_EQ(a(1, 1), inital_array[3]);

  auto minor = a.getSubMatrix(0, 0);
  EXPECT_EQ(minor(0, 0), inital_array[3]);
}

TEST(Matrix, determinantTest) {
  std::array<double, 4> arrayA = {2.0, 3.0, -5.0, 1.0};
  auto squareMatrixA           = SquareMatrix<2, double>(arrayA);
  EXPECT_EQ(squareMatrixA.det(), 17.0);

  std::array<double, 9> arrayB = {
      6.0, 1.0,  1.0,  //
      4.0, -2.0, 5.0,  //
      2.0, 8.0,  7.0   //
  };

  auto squareMatrixB = SquareMatrix<3, double>(arrayB);
  EXPECT_EQ(squareMatrixB.det(), -306.0);
}

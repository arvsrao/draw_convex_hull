//
// Created by Arvind Rao on 16.08.20.
//

#ifndef DRAW_CONVEX_HULL_SQUAREMATRIX_H
#define DRAW_CONVEX_HULL_SQUAREMATRIX_H

#include <RowColumnMatrix.h>

#include <array>

template <uint N, typename R>
class SquareMatrix : public RowColumnMatrix<N, N, R> {
 public:
  using MatrixType              = SquareMatrix<N, R>;
  using CoDimensionOneSubMatrix = SquareMatrix<N - 1, R>;

  explicit SquareMatrix(std::array<R, N * N> &mat);
  explicit SquareMatrix(RowColumnMatrix<N, N, R> &mat);
  explicit SquareMatrix(RowColumnMatrix<N, N, R> &&mat);

  CoDimensionOneSubMatrix getSubMatrix(int row, int col);
  R det();
};

template <typename R>
class SquareMatrix<2, R> : public RowColumnMatrix<2, 2, R> {
 public:
  explicit SquareMatrix(std::array<R, 4> &mat);
  SquareMatrix(RowColumnMatrix<2, 2, R> &mat);
  explicit SquareMatrix(RowColumnMatrix<2, 2, R> &&mat);

  SquareMatrix<1, R> getSubMatrix(int row, int col);

  R det();
};

template <typename R>
class SquareMatrix<1, R> : public RowColumnMatrix<1, 1, R> {
 public:
  SquareMatrix(std::array<R, 1> &mat);
  SquareMatrix(RowColumnMatrix<1, 1, R> &mat);
  SquareMatrix(RowColumnMatrix<1, 1, R> &&mat);

  R det();
};

#include <impl/SquareMatrix.cpp>

#endif  // DRAW_CONVEX_HULL_SQUAREMATRIX_H

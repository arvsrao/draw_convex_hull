#ifndef DRAW_CONVEX_HULL_ROWCOLUMNMATRIX_H
#define DRAW_CONVEX_HULL_ROWCOLUMNMATRIX_H

#include <array>

/**
 * Store Matrix elements row-wise in a one-dimensional array
 */
template <uint N, uint M, typename R>
class RowColumnMatrix {
 public:
  using RingType = R;
  using DataType = std::array<RingType, N * M>;

  RowColumnMatrix(DataType &mat);
  RingType operator()(int row, int col);

  RowColumnMatrix<N - 1, M - 1, RingType> getSubMatrix(int row, int col);

  DataType data;
};

template <typename R>
class RowColumnMatrix<1, 1, R> {
 public:
  using RingType = R;
  using DataType = std::array<RingType, 1>;

  RowColumnMatrix(DataType &mat);
  RingType operator()(int row, int col);

  DataType data;
};

#include <impl/RowColumnMatrix.cpp>

#endif  // DRAW_CONVEX_HULL_ROWCOLUMNMATRIX_H

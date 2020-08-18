#include <RowColumnMatrix.h>

#include <array>
#include <stdexcept>

template <uint N, uint M, typename R>
RowColumnMatrix<N, M, R>::RowColumnMatrix(std::array<R, N * M> &mat) : data(mat) {}

template <uint N, uint M, typename R>
R RowColumnMatrix<N, M, R>::operator()(int row, int col) {
  return data[row * N + col];
}

template <typename R>
RowColumnMatrix<1, 1, R>::RowColumnMatrix(RowColumnMatrix<1, 1, R>::DataType &mat) : data(mat) {}

template <typename R>
R RowColumnMatrix<1, 1, R>::operator()(int row, int col) {
  return data[0];
}

template <uint N, uint M, typename R>
RowColumnMatrix<N - 1, M - 1, R> RowColumnMatrix<N, M, R>::getSubMatrix(int row, int col) {
  if (N - 1 <= 0 || M - 1 <= 0) throw std::invalid_argument(" N and M must be greater than 1.");

  std::array<R, (N - 1) * (M - 1)> submatrix;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      if (i != row && j != col) {
        uint iprime                          = (i < row) ? i : i - 1;
        uint jprime                          = (j < col) ? j : j - 1;
        submatrix[iprime * (N - 1) + jprime] = operator()(i, j);
      }
    }
  }
  return RowColumnMatrix<N - 1, M - 1, RingType>(submatrix);
}

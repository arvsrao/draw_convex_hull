
template <uint N, typename R>
SquareMatrix<N, R>::SquareMatrix(std::array<R, N * N> &mat) : RowColumnMatrix<N, N, R>(mat) {}

template <typename R>
SquareMatrix<1, R>::SquareMatrix(std::array<R, 1> &mat) : RowColumnMatrix<1, 1, R>(mat) {}

template <typename R>
SquareMatrix<2, R>::SquareMatrix(std::array<R, 4> &mat) : RowColumnMatrix<2, 2, R>(mat) {}

template <uint N, typename R>
SquareMatrix<N, R>::SquareMatrix(RowColumnMatrix<N, N, R> &mat)
    : RowColumnMatrix<N, N, R>(mat.data) {}

template <typename R>
SquareMatrix<1, R>::SquareMatrix(RowColumnMatrix<1, 1, R> &mat)
    : RowColumnMatrix<1, 1, R>(mat.data) {}

template <typename R>
SquareMatrix<2, R>::SquareMatrix(RowColumnMatrix<2, 2, R> &mat)
    : RowColumnMatrix<2, 2, R>(mat.data) {}

template <uint N, typename R>
SquareMatrix<N, R>::SquareMatrix(RowColumnMatrix<N, N, R> &&mat)
    : RowColumnMatrix<N, N, R>(mat.data) {}

template <typename R>
SquareMatrix<1, R>::SquareMatrix(RowColumnMatrix<1, 1, R> &&mat)
    : RowColumnMatrix<1, 1, R>(mat.data) {}

template <typename R>
SquareMatrix<2, R>::SquareMatrix(RowColumnMatrix<2, 2, R> &&mat)
    : RowColumnMatrix<2, 2, R>(mat.data) {}

template <typename R>
R SquareMatrix<1, R>::det() {
  return this->data[0];
}

template <typename R>
R SquareMatrix<2, R>::det() {
  return this->data[0] * this->data[3] - this->data[1] * this->data[2];
}

template <uint N, typename R>
R SquareMatrix<N, R>::det() {
  R result = static_cast<R>(0);

  for (int j = 0; j < N; j++) {
    R minor_0j = CoDimensionOneSubMatrix(this->getSubMatrix(0, j)).det();
    R c        = (j % 2 == 0 ? 1.0 : -1.0) * this->operator()(0, j);
    result += c * minor_0j;
  }

  return result;
}

#include "s21_matrix_oop.h"

// Дефолтный конструктор
// Списки инициализации представляют перечисления инициализаторов
// для каждой из переменных и констант через двоеточие после
// списка параметров конструктора:
S21Matrix::S21Matrix() : rows_{}, cols_{}, matrix_{} {}

// конструктор с параметрами
S21Matrix::S21Matrix(int rows, int cols) {
  if (rows < 1 || cols < 1) {
    throw std::invalid_argument(
        "Количество столбцов и строк в матрице не может быть мнеьше 1");
  }
  rows_ = rows;
  cols_ = cols;
  InitMatrix();
}

// приватный метод: выделение памяти для матрицы и заполнения нулями
void S21Matrix::InitMatrix() {
  matrix_ = new double *[rows_]();
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_]();
  }
}

// Деструктор
S21Matrix::~S21Matrix() { ClearMatrix(); }

// Конструктор копирования
S21Matrix::S21Matrix(const S21Matrix &other)
    : rows_(other.rows_), cols_(other.cols_) {
  if (&other == this) {
    throw std::logic_error("нельзя копировать в себя");
  }
  CopyMatrix(other);
}

/**
 * @brief копирование матрицы
 * @param other объект-матрица которую хотим копировать
 */
void S21Matrix::CopyMatrix(const S21Matrix &other) {
  InitMatrix();
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

// Конструктор перемещения
S21Matrix::S21Matrix(S21Matrix &&other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  rows_ = std::move(other.rows_);
  cols_ = std::move(other.cols_);
  matrix_ = std::move(other.matrix_);
  other.ClearMatrix();
}

// Приватный метод:
// очистка памяти и установка количество строк и столбцов в нули
void S21Matrix::ClearMatrix() {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
  }
  matrix_ = nullptr;
  rows_ = 0;
  cols_ = 0;
}

// Геттер - получить количество строк
int S21Matrix::GetRows() const { return rows_; }

// Геттер - получить количество столбцов
int S21Matrix::GetCols() const { return cols_; }

// Приватный метод для копирования значений матрицы
void S21Matrix::FillMatrix(S21Matrix &newMatrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      newMatrix.matrix_[i][j] = matrix_[i][j];
    }
  }
}

/**
 * Сеттер для установления количество строк, при уменьшении
 * размера матрицы лишнее отрасыватеся
 */
void S21Matrix::SetRows(int rows) {
  S21Matrix newMatrix(rows, cols_);
  double edge = rows_;
  if (rows < rows_) edge = rows;
  FillMatrix(newMatrix, edge, cols_);
  *this = newMatrix;
}

/**
 * Сеттер для установления количество столбцов, при уменьшении
 * размера матрицы лишнее отрасыватеся
 */
void S21Matrix::SetCols(int cols) {
  S21Matrix newMatrix(rows_, cols);
  double edge = cols_;
  if (cols < cols_) edge = cols;
  FillMatrix(newMatrix, rows_, edge);
  *this = newMatrix;
}

// проверяет матрицы на равенство
bool S21Matrix::EqMatrix(const S21Matrix &other) const {
  bool flag = true;
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    flag = false;
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        if (fabs(matrix_[i][j] - other.matrix_[i][j]) >= 1.0e-07) {
          flag = false;
        }
      }
    }
  }
  return flag;
}

// сложение матриц, перед этим выполняется проверка на
// пазмерность двух матриц, если размеры не равны,
// то кидается исключение
void S21Matrix::SumMatrix(const S21Matrix &other) {
  CheckIfSizesAreEqual(other);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

// вычитание матриц, перед этим выполняется проверка на
// пазмерность двух матриц, если размеры не равны,
// то кидается исключение
void S21Matrix::SubMatrix(const S21Matrix &other) {
  CheckIfSizesAreEqual(other);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

// проверка матриц на одинаковый размер
void S21Matrix::CheckIfSizesAreEqual(const S21Matrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("количество строк и столбцов не равны");
  }
}

// умножение матрицы на число
void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

// Умножение двух матриц, с предварительной проверкой
// что б количество строк одной матрицы было равно количеству столбцов
// другой матрицы
void S21Matrix::MulMatrix(const S21Matrix &other) {
  if (cols_ != other.rows_) {
    throw std::invalid_argument("Invalid sizes of matrices for multiplying");
  }
  S21Matrix res(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      res(i, j) = 0;
      for (int k = 0; k < cols_; k++) {
        res(i, j) += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  *this = res;
}

// транспонирование матрицы
S21Matrix S21Matrix::Transpose() const {
  S21Matrix transposed(cols_, rows_);
  for (int i = 0; i < transposed.rows_; i++) {
    for (int j = 0; j < transposed.cols_; j++) {
      transposed.matrix_[i][j] = matrix_[j][i];
    }
  }
  return transposed;
}

// Вычисляет матрицу алгебраических дополнений
// текущей матрицы и возвращает ее
S21Matrix S21Matrix::CalcComplements() const {
  CheckIfSquare();  // если матрица не квадратная - кидает исключение
  if (rows_ == 1) {
    throw std::logic_error("Размерность должна быть больше 1");
  }
  S21Matrix result(rows_, cols_);
  ComplementsHelp(result);
  return result;
}

// Проверка на квадратность матрицы
void S21Matrix::CheckIfSquare() const {
  if (rows_ != cols_) {
    throw std::logic_error("Матрица не квадратная");
  }
}

// вспомогательная функция для нахождения матрицы кофакторов
void S21Matrix::ComplementsHelp(S21Matrix &complements) const {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      S21Matrix minor(rows_ - 1, cols_ - 1);
      FindMinor(minor, i, j);
      complements.matrix_[i][j] = pow(-1, (i + j)) * minor.Determinant();
      minor.ClearMatrix();
    }
  }
}

// Нахождение минора матрицы
void S21Matrix::FindMinor(S21Matrix &minor, int row, int col) const {
  int RowCounter = 0, ColCounter = 0;
  for (int i = 0; i < rows_; i++) {
    if (i != row) {
      ColCounter = 0;
      for (int j = 0; j < cols_; j++) {
        if (j != col) {
          minor.matrix_[RowCounter][ColCounter] = matrix_[i][j];
          ColCounter++;
        }
      }
      RowCounter++;
    }
  }
}

// Вычисляет и возвращает определитель текущей матрицы
double S21Matrix::Determinant() const {
  CheckIfSquare();
  return DetHelp();
}

// вспомогательный метод для нахождениея дтерминанта
double S21Matrix::DetHelp() const {
  double total = 0;
  if (rows_ == 1) {
    total = matrix_[0][0];
  } else {
    for (int j = 0; j < cols_; j++) {
      S21Matrix minor(rows_ - 1, cols_ - 1);
      FindMinor(minor, 0, j);
      total += matrix_[0][j] * pow(-1, j) * minor.DetHelp();
      minor.ClearMatrix();
    }
  }
  return total;
}

// Вычисляет и возвращает обратную матрицу
S21Matrix S21Matrix::InverseMatrix() {
  const double determinant = Determinant();
  if (fabs(determinant) <= 1.0e-7) {
    throw std::logic_error("Определитель не может быть 0");
  }
  S21Matrix inversed = CalcComplements();
  inversed = inversed.Transpose();
  inversed *= (1.0 / determinant);
  return inversed;
}

/*---------ПЕРЕЗАГРУЗКА ОПЕРАТОРОВ----------*/

S21Matrix S21Matrix::operator+(const S21Matrix &other) {
  S21Matrix result(*this);
  result.SumMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator-(const S21Matrix &other) {
  S21Matrix result(*this);
  result.SubMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix &other) {
  S21Matrix result(*this);
  result.MulMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(const double mul) {
  S21Matrix result(*this);
  result.MulNumber(mul);
  return result;
}

bool S21Matrix::operator==(const S21Matrix &other) const {
  return EqMatrix(other);
}

S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  if (this == &other) {
    return *this;
  }

  ClearMatrix();
  rows_ = other.rows_;
  cols_ = other.cols_;
  CopyMatrix(other);
  return *this;
}

S21Matrix &S21Matrix::operator=(S21Matrix &&other) {
  if (this == &other) {
    return *this;
  }
  ClearMatrix();
  rows_ = std::exchange(other.rows_, 0);
  cols_ = std::exchange(other.cols_, 0);
  matrix_ = std::exchange(other.matrix_, nullptr);
  return *this;
}

S21Matrix S21Matrix::operator+=(const S21Matrix &other) {
  SumMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator-=(const S21Matrix &other) {
  SubMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*=(const S21Matrix &other) {
  MulMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*=(const double mul) {
  MulNumber(mul);
  return *this;
}

double &S21Matrix::operator()(int row, int col) {
  CheckIfIndexExists(row, col);
  return matrix_[row][col];
}

double &S21Matrix::operator()(int row, int col) const {
  CheckIfIndexExists(row, col);
  return matrix_[row][col];
}

// вспомогательная функция для проверки, что индексы не за пределами матрицы
void S21Matrix::CheckIfIndexExists(int row, int col) const {
  if (row < 0) {
    throw std::out_of_range("Кол-во строк должно быть больше 0");
  } else if (col < 0) {
    throw std::out_of_range("Кол-во столбцов должно быть больше 0");
  } else if (row >= rows_) {
    throw std::out_of_range("Кол-во строк за пределами матрицы");
  } else if (col >= cols_) {
    throw std::out_of_range("Кол-во столбцов за пределами матрицы");
  }
}
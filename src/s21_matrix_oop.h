#ifndef S21_MATRIX_OOP_H
#define S21_MATRIX_OOP_H

#include <cmath>
#include <iostream>
#include <utility>

class S21Matrix {
 public:
  /*
  Правило пяти говорит о том, что в общем случае, если возникла необходимость
  самостоятельного определения одной из операций копирования, перемещения или
  разрушения объекта, то скорее всего для корректной работы нужно будет
  реализовать:
      - Деструктор
      - Конструктор копирования
      - Оператор присваивания копированием
      - Конструктор перемещения
      - Оператор присваивания перемещением
  */
  S21Matrix();                    // дефолтный конструктор
  S21Matrix(int rows, int cols);  // конструктор с параметрами
  ~S21Matrix();                   // деструктор
  S21Matrix(const S21Matrix& other);  // конструктор копирования
  S21Matrix(S21Matrix&& other);  // конструктор перемещения

  /* Геттеры и Сеттеры для доступа к приватным полям класса */
  int GetRows() const;
  int GetCols() const;
  void SetRows(int rows);
  void SetCols(int cols);

  /* Основные функции */
  bool EqMatrix(const S21Matrix& other) const;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix();

  /* перезагрузка операторов */
  S21Matrix operator+(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other);
  S21Matrix operator*(const double mul);
  bool operator==(const S21Matrix& other) const;
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other);
  S21Matrix operator+=(const S21Matrix& other);
  S21Matrix operator-=(const S21Matrix& other);
  S21Matrix operator*=(const S21Matrix& other);
  S21Matrix operator*=(const double mul);
  double& operator()(int row, int col);
  double& operator()(int row, int col) const;

 private:
  /* Приватаные поля */
  int rows_, cols_;
  double** matrix_;

  /* Приватные вспомогательные методы */
  void InitMatrix();
  void CopyMatrix(const S21Matrix& other);
  void ClearMatrix();
  void FillMatrix(S21Matrix& newMatrix, int rows, int cols);
  void CheckIfSizesAreEqual(const S21Matrix& other) const;
  void CheckIfSquare() const;
  void ComplementsHelp(S21Matrix& complements) const;
  void FindMinor(S21Matrix& minor, int row, int col) const;
  double DetHelp() const;
  void CheckIfIndexExists(int row, int col) const;
};

#endif  // S21_MATRIX_OOP_H
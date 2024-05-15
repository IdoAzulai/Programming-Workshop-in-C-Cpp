// Matrix.cpp
#include "Matrix.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>

#define THRESHOLD 0.1

/// Constructor
Matrix::Matrix (int rows_input, int cols_input) : rows(rows_input),
                                                  cols(cols_input),
                                                  data(nullptr)
{
  // Initialize matrix with zeros
  if (rows_input > 0 && cols_input > 0)
  {
    data = new float[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
      data[i]=0;
    }
  }
  else
  {
    throw std::exception();
  }
}

/// Default constructor
Matrix::Matrix () : Matrix (1, 1)
{}

/// Copy constructor
Matrix::Matrix(const Matrix &other) : rows(other.rows),
                                      cols(other.cols),
                                      data(new float[other.rows * other.cols])
{
  // Copy data from other matrix
  for (int i = 0; i < rows * cols; ++i)
  {
    data[i] = other.data[i];
  }
}

/// Destructor
Matrix::~Matrix ()
{
  // Free memory allocated for data
  delete[] data;
}

/// Get number of rows
int Matrix::get_rows() const
{
  return this->rows;
}

/// Get number of columns
int Matrix::get_cols () const
{
  return this->cols;
}

/// Transpose matrix
Matrix &Matrix::transpose ()
{
  // Copy elements to the transposed matrix
  Matrix transposed (cols, rows);

  for (int i = 0; i < cols; ++i)
  {
    for (int j = 0; j < rows; ++j)
    {
      transposed.data[i*rows + j] = data[j*cols + i];
    }
  }

  // Update data to point to transposed matrix
  *this = transposed;

  return *this; // Enable function concat
}

/// Turn Matrix into vector
Matrix& Matrix::vectorize() {
    Matrix vectorized(rows * cols, 1); // Create a new matrix with one column

    // Copy the data into the vectorized matrix
    for (int i = 0; i < rows * cols; i++) {
        vectorized[i] = data[i];
    }

    // Update dimensions and data of the original matrix
    *this = vectorized;

    return *this;
}

/// Prints matrix elements
void Matrix::plain_print () const
{
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      std::cout << data[i*cols + j] << " ";
    }
    std::cout << std::endl;
  }
}

/// Mults each element with itself
Matrix Matrix::dot (Matrix other)
{
  // Check if dimensions allow dot product (rows and cols are equal)
  if (rows != other.get_rows() || cols != other.get_cols())
  {
    throw std::exception();
  }

  // Create a new matrix for results
  Matrix result (rows, cols);
  for (int i = 0; i < rows * cols; ++i)
  {
      result.data[i] = data[i] * other[i];
  }
  return result;
}

/// Norm of matrix
float Matrix::norm () const
{
  float norm = 0.0;
  for (int i = 0; i < rows * cols; ++i)
  {
    norm += data[i] * data[i];
  }
  // Return square root of norm
  return std::sqrt (norm);
}

Matrix Matrix::rref()
{
    // Create a copy of the original matrix
    Matrix rref_matrix(*this);
    int lead = 0, row_count = rref_matrix.rows, col_count = rref_matrix.cols;
    for (int row = 0; row < row_count; ++row)
    {
        if (lead >= col_count)
        {
            break;
        }
        int i = row;
        while (rref_matrix.data[i * col_count + lead] == 0)
        {
            ++i;
            if (i == row_count) {
                i = row;
                ++lead;
                if (lead == col_count) {
                    return rref_matrix;
                }
            }
        }
        // Swap rows i and row
        for (int k = 0; k < col_count; ++k) {
            std::swap(rref_matrix.data[row * col_count + k],
                      rref_matrix.data[i * col_count + k]);
        }
        // Divide row by lead value
        float div = rref_matrix.data[row * col_count + lead];
        if (div != 0) {
            for (int j = 0; j < col_count; ++j) {
                rref_matrix.data[row * col_count + j] /= div;
            }
        }
        // Subtract other rows
        for (int j = 0; j < row_count; ++j) {
            if (j != row) {
                float factor = rref_matrix.data[j * col_count + lead];
                for (int k = 0; k < col_count; ++k) {
                    rref_matrix.data[j * col_count + k] -=
                            factor * rref_matrix.data[row * col_count + k];
                }
            }
        }
        ++lead;
    }
    return rref_matrix;
}

/// Returns the index of the largest number in the matrix
int Matrix::argmax () const
{
  int max_index = 0;
  float max_value = data[0];

  // Iterate through all elements to find max value and its index
  for (int i = 0; i < rows * cols; ++i)
  {
    if (data[i] > max_value)
    {
      max_value = data[i];
      max_index = i;
    }
  }

  return max_index;
}

/// Returns the sum of all elements in the matrix
float Matrix::sum () const
{
  float total = 0.0;

  // Iterate through all elements and sum their values
  for (int i = 0; i < rows * cols; ++i)
  {
    total += data[i];
  }

  return total;
}

/// Operator += (Matrix addition)
Matrix &Matrix::operator+= (const Matrix &other)
{
  if (rows != other.rows || cols != other.cols)
  {
    throw std::exception();
  }
  for (int i = 0; i < rows * cols; ++i)
  {
    data[i] += other.data[i];
  }

  return *this;
}

/// Operator + (Matrix addition)
Matrix Matrix::operator+ (const Matrix &other) const
{
  Matrix result (*this);
  result += other;
  return result;
}

/// Assignment operator (using copy-and-swap idiom)
Matrix &Matrix::operator=(Matrix other)
{
    if (this != &other) { // Self-assignment check
        Matrix temp(other); // Create a copy of 'other'

        // Swap the contents of 'temp' with 'this'
        std::swap(rows, temp.rows);
        std::swap(cols, temp.cols);
        std::swap(data, temp.data);
    }
    return *this;
}

/// Operator * (Matrix multiplication)
Matrix Matrix::operator* (Matrix other) const
{
  if (cols != other.get_rows())
  {
    throw std::exception();
  }
  else
  {
  Matrix result (rows, other.get_cols());

  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < other.get_cols(); ++j)
    {
      for (int k = 0; k < cols; ++k)
      {
        result.data[i*result.get_cols() + j] += data[i*cols + k]
                                      * other.data[k*other.get_cols() + j];
      }
    }
  }
  return result;
  }
}

/// Operator * (Scalar multiplication on the right)
Matrix Matrix::operator* (const float other) const
{
  Matrix result (*this);
  for (int i = 0; i < rows * cols; ++i)
  {
    result.data[i] = other * result.data[i];
  }
  return result;
}

/// Friend function definition for scalar multiplication on the left
Matrix operator* (float scalar, Matrix matrix)
{
  return Matrix (matrix * scalar);
}

/// Parenthesis indexing operator, (row, col) format.
float &Matrix::operator() (int row, int col)
{
  // Check for valid indexing
  if (row >= 0 && row < rows && col >= 0 && col < cols)
  {
    return data[row *cols + col];
  }
  else
  {
    throw std::exception();
  }
}

/// Parenthesis indexing operator for concatenating. (row, col) format.
float Matrix::operator() (int row, int col) const
{
  // Check for invalid indexing
  if (row >= 0 && row < rows && col >= 0 && col < cols)
  {
    return data[row *cols + col];
  }
  else
  {
    throw std::exception();
  }
}

/// Parenthesis indexing operator, [k] format.
float &Matrix::operator[] (int k)
{
  // Check for valid indexing
  if (k >= 0 && k < rows*cols)
  {
    return data[k];
  }
  else
  {
    throw std::exception();
  }
}

/// Parenthesis indexing operator for concatenating, [k] format.
float Matrix::operator[] (int k) const
{
  // Check for valid indexing
  if (k >= 0 && k < rows*cols)
  {
    return this->data[k];
  }
  else
  {
    throw std::exception();
  }
}


/// Prints the whole matrix with ** if val is >0.1, or "  " else
std::ostream &operator<< (std::ostream &out, const Matrix &A)
{
  for (int i = 0; i < A.rows; ++i)
  {
    for (int j = 0; j < A.cols; ++j)
    {
      if (A (i, j) > THRESHOLD)
      {
        out << "**"; // double asterisk
      }
      else
      {
        out << "  "; // double space
      }
    }
    // print newLine
    out << std::endl;
  }
  return out;
}

std::istream &operator>> (std::istream &in, Matrix &img_matrix)
{
  long size = img_matrix.cols * img_matrix.rows * sizeof (float);
  char *input = new char[size];
  in.read (input, size);
  if (in)
  {
    for (int i = 0; i < img_matrix.cols * img_matrix.rows; ++i)
    {
      std::memcpy(&img_matrix.data[i],
                  &input[i * sizeof(float)],
                  sizeof(float));
    }
  }
  else
  {
    delete[] input;
    throw std::exception();
  }

  return in;
}
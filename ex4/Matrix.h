// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>


// You don't have to use the struct. Can help you with MlpNetwork.h
struct matrix_dims {
    int rows, cols;
};

class Matrix {
private:
    int rows;
    int cols;
    float *data;

public:

    /// ------ Constructors ------
    // constructor
    Matrix(int rows, int cols);

    // Default constructor - matrix size 1x1, single element to 0.
    Matrix();

    // copy constructor
    Matrix(const Matrix &m);

    // destructor
    ~Matrix();


    /// ------ Methods & Functions ------
    int get_rows() const;

    int get_cols() const;

    // transpose matrix
    Matrix &transpose();

    // turns matrix to vector
    Matrix& vectorize();

    // prints matrix elements
    void plain_print() const;

    // returns dot product of Matrix
    Matrix dot(Matrix other);

    // returns the Frobenius norm of Matrix
    float norm() const;

    //Returns new Matrix that is the reduced row echelon form of the original.
    Matrix rref();

    // Returns index of the largest number in the matrix.
    int argmax() const;

    // Returns the sum of all the elements in the matrix
    float sum() const;

    /// ------ Operators ------
    // +=
    Matrix& operator+= (const Matrix &other);

    // +
    Matrix operator+ (const Matrix &other) const;

    // = (Assignment)
    Matrix &operator= (Matrix other);

    // * (Matrix multiplication)
    Matrix operator* (Matrix other) const;

    // * (Scalar multiplication on the right)
    Matrix operator* (float other) const;

    // * (Scalar multiplication on the left)
    friend Matrix operator* (float scalar, Matrix matrix);

    // () Parenthesis indexing
    float &operator() (int row, int col);

    // () Parenthesis indexing, const
    float operator() (int row, int col) const;

    // [] Brackets indexing
    float &operator[] (int k);

    // [] Brackets indexing, const
    float operator[] (int k) const;

    // << Output stream
    friend std::ostream &operator<< (std::ostream &out, const Matrix &A);

    // >> Input stream
    friend std::istream &operator>> (std::istream &in, Matrix &img_matrix);
};


#endif //MATRIX_H
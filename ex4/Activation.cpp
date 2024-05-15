#include "Activation.h"
#include <cmath>


Matrix activation::relu(Matrix biased_vector)
{
    Matrix output_vector(biased_vector);
    for (int i = 0; i < biased_vector.get_cols() *
                        biased_vector.get_rows(); ++i) {
        if (biased_vector[i] < 0)
        {
            output_vector[i] = 0;
        }
    }
    return output_vector;
}

Matrix activation::softmax(Matrix vector)
{
    Matrix output_vector(vector.get_rows(), vector.get_cols());

    for (int i = 0; i < vector.get_rows(); ++i) {
        for (int j = 0; j < vector.get_cols(); ++j) {
            output_vector(i,j) = std::exp(vector(i,j));
        }
    }
    float pre_vector = 1 / output_vector.sum();

    return pre_vector * output_vector;
}
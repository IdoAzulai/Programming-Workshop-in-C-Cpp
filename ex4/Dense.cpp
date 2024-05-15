#include "Dense.h"



/// ------ Constructors ------
Dense::Dense(Matrix weights_mat, Matrix bias_mat, active_func act):
weights(weights_mat), bias(bias_mat), active(act) {}

/// ------ Getters ------
Matrix Dense::get_weights() const
{
    return this->weights;
}

Matrix Dense::get_bias() const
{
    return this->bias;
}

active_func Dense::get_activation() const
{
    return this->active;
}

/// ------ Operators ------
Matrix Dense::operator() (Matrix m) const
{
  Matrix mat_after_layer = weights * m + bias;
  return active(mat_after_layer);
}

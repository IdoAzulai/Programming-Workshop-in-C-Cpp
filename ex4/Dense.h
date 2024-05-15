// Dense.h
#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

typedef Matrix(*active_func) (Matrix);

class Dense
{
 private:
  Matrix weights;
  Matrix bias;
  active_func active;

 public:

  /// ------ Constructors ------
  /**
   * Inits a new layer with given parameters.
   * @param weights weights matrix
   * @param bias bias matrix
   */
  Dense (Matrix weights_mat, Matrix bias_mat, active_func act);

  /// ------ Getters ------
  /// Returns the weights of this layer.
  Matrix get_weights () const;

  ///Returns the bias of this layer.
  Matrix get_bias () const;

  /// Returns the activation function of this layer.
  active_func get_activation () const;

  /// ------ Operators ------
  /// Applies the layer on the input and returns an output matrix.
  Matrix operator() (Matrix m) const;

};

#endif //DENSE_H
#include "MlpNetwork.h"

bool valid_dimension(Matrix *m)
{
  for (int i = 0; i < MLP_SIZE; ++i)
  {
    if (((m[i].get_rows()) != (weights_dims[i].rows)) ||
        ((m[i].get_cols()) != (weights_dims[i].cols)))
    {
      return false;
    }
  }
  return true;
}

MlpNetwork::MlpNetwork (Matrix *weights_mat, Matrix *biases_mat) :
layer1(Dense(weights_mat[FIRST], biases_mat[FIRST], activation::relu)),
layer2(Dense(weights_mat[SECOND], biases_mat[SECOND], activation::relu)),
layer3(Dense(weights_mat[THIRD], biases_mat[THIRD], activation::relu)),
layer4(Dense(weights_mat[FOURTH], biases_mat[FOURTH], activation::softmax))
{
  if(!valid_dimension(weights_mat) || valid_dimension(biases_mat))
  {
    throw std::exception();
  }
}

digit MlpNetwork::operator() (Matrix img) const
{
    Matrix r1, r2, r3, r4;
    r1 = activation::relu(layer1.get_weights() * img +
                                  layer1.get_bias());
    r2 = activation::relu(layer2.get_weights() * r1 +
                                  layer2.get_bias());
    r3 = activation::relu(layer3.get_weights() * r2 +
                                  layer3.get_bias());
    r4 = activation::softmax(layer4.get_weights() * r3 +
                                     layer4.get_bias());
    unsigned int val = r4.argmax();
    float prob = r4[val];
    digit dig = {val, prob};
    return dig;
}
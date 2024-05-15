// MlpNetwork.h
#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"

#define MLP_SIZE 4
#define FIRST 0
#define SECOND 1
#define THIRD 2
#define FOURTH 3

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit {
	unsigned int value;
	float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
									{64,  128},
									{20,  64},
									{10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
								 {64,  1},
								 {20,  1},
								 {10,  1}};

class MlpNetwork
{
private:
//  Dense layers[MLP_SIZE];
  Dense layer1;
  Dense layer2;
  Dense layer3;
  Dense layer4;
//    Matrix *weights[4];
//    Matrix *biases[4];

public:
    /// ------ Constructors ------
    /**
     * Constructs the network
     * @param weights array of matrices, size 4
     * @param biases array of matrices, size 4
     */
    MlpNetwork(Matrix *weights_matrix, Matrix *biases_matrix);

    /// ------ Operators ------
    /// Applies the entire network on input. Returns a digit struct type.
    digit operator() (Matrix img) const;
};

#endif // MLPNETWORK_H
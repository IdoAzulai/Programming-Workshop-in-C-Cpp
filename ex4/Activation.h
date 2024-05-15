// Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

// Insert Activation namespace here...
namespace activation
{
    /**
     *
     * @param biased_vector
     * @return
     */
    Matrix relu (Matrix biased_vector);

    Matrix softmax (Matrix vector);

}



#endif //ACTIVATION_H
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// If only one cycle in function i as index!
Matrix EMPTY = {0, 0, NULL};


STATUS matrix_alloc(Matrix* ret, const size_t rows, const size_t cols) {
    size_t array = rows * cols;
    if (rows != 0 && array / rows != cols)
        return ERR_OVERFLOW;
    // Double size is platform-dependent. Cannot use bitflip easily
    size_t size = array*sizeof(double);
    if (size / sizeof(double) != array)
        return ERR_OVERFLOW;
    ret->values = malloc(size);
    if (ret->values == NULL)
        return ERR_MALLOC;
    ret->rows = rows;
    ret->cols = cols;
    return OK;
}


STATUS matrix_identity(Matrix matrix) {
    if (matrix.rows != matrix.cols)
        return ERR_SIZE;
    if (matrix.values == NULL)
        return (matrix.rows == matrix.cols && matrix.cols == 0) ? OK : ERR_MALLOC;
    memset(matrix.values, 0, sizeof(double) * matrix.rows * matrix.cols); // It is safe if matrix initialized
    for (size_t i = 0; i < matrix.rows; ++i)
        matrix.values[i * matrix.cols + i] = 1.0;
    return OK;
}


STATUS matrix_fill_val(Matrix matrix, const double* value) {
    memcpy(matrix.values, value, matrix.rows * matrix.cols * sizeof(double));
    return OK;
}


STATUS matrix_clone(Matrix* ret, Matrix src) {
    STATUS status = matrix_alloc(ret, src.rows, src.cols);
    if (status != OK)
        return status;
    matrix_fill_val(*ret, src.values);
    return OK;
}


STATUS matrix_equals(int* res, const Matrix matA, const Matrix matB, const double accuracy) {
    if (matA.rows != matB.rows || matA.cols != matB.cols)
        return ERR_SIZE;
    for (size_t i = 0; i < matA.rows * matA.cols; ++i)
        if (fabs(matA.values[i] - matB.values[i]) > accuracy) {
            *res = 0;
            return OK;
        }
    *res = 1;
    return OK;
}


void matrix_free(Matrix* matrix) {
    if (matrix->values != NULL) {
        free(matrix->values);
        matrix->values = NULL;
    }
    matrix->rows = 0;
    matrix->cols = 0;
}


void matrix_print(const Matrix matrix) {
    for (size_t row = 0; row < matrix.rows; ++row) {
        for (size_t col = 0; col < matrix.cols; ++col)
            printf("%f ", matrix.values[row * matrix.cols + col]);
        printf("\n");
    }
}


STATUS matrix_add(Matrix matA, const Matrix matB) {
    if (matA.rows != matB.rows || matA.cols != matB.cols)
        return ERR_SIZE;
    for (size_t i = 0; i < matA.rows * matA.cols; ++i)
        matA.values[i] += matB.values[i];
    return OK;
}


STATUS matrix_subt(Matrix matA, const Matrix matB) {
    if (matA.rows != matB.rows || matA.cols != matB.cols)
        return ERR_SIZE;
    for (size_t i = 0; i < matA.rows * matA.cols; ++i)
        matA.values[i] -= matB.values[i];
    return OK;
}


STATUS matrix_mult(Matrix* ret, Matrix matA, Matrix matB) {
    if (matA.cols != matB.rows)
        return ERR_SIZE;
    STATUS status = matrix_alloc(ret, matA.rows, matB.cols);
    if (status != OK)
        return status;
    for (size_t rowA = 0; rowA < matA.rows; ++rowA)
        for (size_t colB = 0; colB < matB.cols; ++colB)
            for (size_t colA = 0; colA < matA.cols; ++colA)
                ret->values[rowA * matB.cols + colB] +=
                    matA.values[rowA * matA.cols + colA]
                    * matB.values[colA * matB.cols + colB];
    return OK;
}


STATUS matrix_mult_by_num(Matrix matrix, const double a) {
    for (size_t i = 0; i < matrix.rows * matrix.cols; ++i)
        matrix.values[i] *= a;
    return OK;
}

// TODO
STATUS matrix_change_rows(Matrix matrix, const size_t rowA, const size_t rowB) {
    if (rowA >= matrix.rows || rowB >= matrix.rows) {
        return ERR_SIZE;
    }
    for (size_t j = 0; j < matrix.cols; ++j) {
        double temp = matrix.values[rowA * matrix.cols + j];
        matrix.values[rowA * matrix.cols + j] = matrix.values[rowB * matrix.cols + j];
        matrix.values[rowB * matrix.cols + j] = temp;
    }
    return OK;
}


STATUS matrix_det(double* ret, Matrix matrix) {
    if (matrix.rows != matrix.cols)
        return ERR_SIZE;
    if (matrix.rows == 1) {
        *ret = matrix.values[0];
        return OK;
    }
    if (matrix.rows == 2) {
        *ret = matrix.values[0] * matrix.values[3] - matrix.values[1] * matrix.values[2];
        return OK;
    }
    double det = 0.0;
    Matrix submatrix;
    STATUS status = matrix_alloc(&submatrix, matrix.rows - 1, matrix.cols - 1);
    if (status != OK)
        return status;
    for (size_t excludedCol = 0; excludedCol < matrix.cols; ++excludedCol) {
        size_t subRow = 0;
        for (size_t row = 1; row < matrix.rows; ++row) {
            size_t subCol = 0;
            for (size_t col = 0; col < matrix.cols; ++col) {
                if (col != excludedCol) {
                    submatrix.values[subRow * (matrix.cols - 1) + subCol] = matrix.values[row * matrix.cols + col];
                    subCol++;
                }
            }
            subRow++;
        }
        double subDet;
        status = matrix_det(&subDet, submatrix);
        if (status != OK) {
            matrix_free(&submatrix);
            return status;
        }
        det += (excludedCol % 2 ? -1 : 1) * matrix.values[excludedCol] * subDet;
        matrix_free(&submatrix);
    }
    *ret = det;
    return OK;
}



// TODO only support simple power
STATUS matrix_pow(Matrix* ret, Matrix matrix, const int power) {
    if (power < 0) {
        return ERR_PWR;
    }
    if (power == 0) {
        Matrix result;
        STATUS status = matrix_alloc(&result, matrix.rows, matrix.cols);
        if (status != OK) {
            return status;
        }
        status = matrix_identity(result);
        if (status != OK) {
            matrix_free(&result);
            return status;
        }
        *ret = result;
        return OK;
    }
    Matrix result;
    STATUS status = matrix_clone(&result, matrix);
    if (status != OK) {
        return status;
    }
    for (int i = 1; i < power; ++i) {
        Matrix temp;
        status = matrix_mult(&temp, result, matrix);
        if (status != OK) {
            matrix_free(&result);
            return status;
        }
        matrix_free(&result);
        result = temp;
    }
    *ret = result;
    return OK;
}


STATUS matrix_check_max_diff(double* ret, const Matrix matA, const Matrix matB) {
    if (matA.rows != matB.rows || matA.cols != matB.cols)
        return ERR_SIZE;
    double max_diff = 0.0;
    for (size_t i = 0; i < matA.rows * matA.cols; ++i) {
        double diff = fabs(matA.values[i] - matB.values[i]);
        if (diff > max_diff)
            max_diff = diff;
    }
    *ret = max_diff;
    return OK;
}


#define EXPONENT_STEPS 100
#define EXPONENT_ACCURACY 1e-6

STATUS matrix_exp(Matrix* ret, const Matrix matrix) {
    if (matrix.rows != matrix.cols)
        return ERR_SIZE;
    Matrix result;
    STATUS status = matrix_alloc(&result, matrix.rows, matrix.cols);
    if (status != OK)
        return status;
    status = matrix_identity(result);
    if (status != OK) {
        matrix_free(&result);
        return status;
    }
    Matrix prev;
    status = matrix_alloc(&prev, matrix.rows, matrix.cols);
    if (status != OK) {
        matrix_free(&result);
        return status;
    }
    for (int m = 1; m <= EXPONENT_STEPS; ++m) {
        status = matrix_fill_val(prev, result.values);
        if (status != OK) {
            matrix_free(&result);
            return status;
        }
        status = matrix_mult_by_num(matrix, 1.0 / m);
        if (status != OK) {
            matrix_free(&result);
            matrix_free(&prev);
            return status;
        }
        Matrix n1_member;
        status = matrix_mult(&n1_member, prev, matrix);
        if (status != OK) {
            matrix_free(&result);
            matrix_free(&prev);
            return status;
        }
        status = matrix_add(result, n1_member);
        matrix_free(&n1_member);
        if (status != OK) {
            matrix_free(&result);
            matrix_free(&prev);
            return status;
        }
        double max_diff;
        status = matrix_check_max_diff(&max_diff, result, prev);
        if (status != OK) {
            matrix_free(&result);
            matrix_free(&prev);
            return status;
        }
        if (max_diff < EXPONENT_ACCURACY) {
            matrix_free(&prev);
            *ret = result;
            return OK;
        }
    }
    matrix_free(&prev);
    *ret = result;
    return OK;
}

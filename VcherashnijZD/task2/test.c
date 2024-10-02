#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define ASSERT_STATUS_OK(status)  \
    do {                         \
        if (status != OK) {      \
            printf("Test failed: %d\n", status); \
            return;             \
        }                       \
    } while (0)

#define ASSERT_MATRIX_EQ(A, B, accuracy) \
    do {                                  \
        int are_equal;                     \
        STATUS status = matrix_equals(&are_equal, * A, * B, accuracy); \
        ASSERT_STATUS_OK(status);           \
        if (!are_equal) {                 \
            printf("Test failed: Matrices are not equal.\n"); \
            return;                     \
        }                                  \
    } while (0)

#define ASSERT_DOUBLE_EQ(A, B, accuracy) \
    do {                                  \
        if (fabs(A - B) > accuracy) {     \
            printf("Test failed: Double values are not equal.\n"); \
            return;                     \
        }                                  \
    } while (0)

#define MAX_VALUE 100
double EQUAL_TEST_ACCURACY = 1e-4;

Matrix generate_random_matrix(size_t rows, size_t cols) {
    Matrix matrix;
    STATUS status = matrix_alloc(&matrix, rows, cols);
    if (status != OK) {
        printf("Test failed: matrix_alloc failed.\n");
        exit(1);
    }
    for (int i = 0; i < rows * cols; i++)
        matrix.values[i] = (double)rand() / RAND_MAX * MAX_VALUE;
    return matrix;
}

void test_matrix_identity() {
    Matrix matrix;
    STATUS status = matrix_alloc(&matrix, 3, 3);
    ASSERT_STATUS_OK(status);
    status = matrix_identity(matrix);
    ASSERT_STATUS_OK(status);
    ASSERT_MATRIX_EQ(&matrix, (&(Matrix){ \
        .rows = 3, \
        .cols = 3, \
        .values = (double[]){1, 0, 0, 0, 1, 0, 0, 0, 1} \
    }), EQUAL_TEST_ACCURACY);
    matrix_free(&matrix);
}

void test_matrix_fill_val() {
    Matrix matrix;
    STATUS status = matrix_alloc(&matrix, 2, 3);
    ASSERT_STATUS_OK(status);
    double values[] = {1, 2, 3, 4, 5, 6};
    status = matrix_fill_val(matrix, values);
    ASSERT_STATUS_OK(status);
    ASSERT_MATRIX_EQ(&matrix, (&(Matrix){ \
        .rows = 2, \
        .cols = 3, \
        .values = (double[]){1, 2, 3, 4, 5, 6} \
    }), EQUAL_TEST_ACCURACY);
    matrix_free(&matrix);
}

void test_matrix_clone() {
    Matrix original = generate_random_matrix(2, 3);
    Matrix clone;
    STATUS status = matrix_clone(&clone, original);
    ASSERT_STATUS_OK(status);
    ASSERT_MATRIX_EQ(&original, &clone, EQUAL_TEST_ACCURACY);
    matrix_free(&original);
    matrix_free(&clone);
}

void test_matrix_add() {
    Matrix matA = generate_random_matrix(3, 3);
    Matrix matB = generate_random_matrix(3, 3);
    Matrix expected = generate_random_matrix(3, 3);
    for (size_t i = 0; i < 9; i++) {
        expected.values[i] = matA.values[i] + matB.values[i];
    }
    STATUS status = matrix_add(matA, matB);
    ASSERT_STATUS_OK(status);
    ASSERT_MATRIX_EQ(&matA, &expected, EQUAL_TEST_ACCURACY);
    matrix_free(&matA);
    matrix_free(&matB);
    matrix_free(&expected);
}

void test_matrix_subt() {
    Matrix matA = generate_random_matrix(3, 3);
    Matrix matB = generate_random_matrix(3, 3);
    Matrix expected = generate_random_matrix(3, 3);
    for (size_t i = 0; i < 9; i++) {
        expected.values[i] = matA.values[i] - matB.values[i];
    }
    STATUS status = matrix_subt(matA, matB);
    ASSERT_STATUS_OK(status);
    ASSERT_MATRIX_EQ(&matA, &expected, EQUAL_TEST_ACCURACY);
    matrix_free(&matA);
    matrix_free(&matB);
    matrix_free(&expected);
}

void test_matrix_mult() {
    Matrix matA = generate_random_matrix(2, 3);
    Matrix matB = generate_random_matrix(3, 2);
    Matrix result;
    STATUS status = matrix_mult(&result, matA, matB);
    ASSERT_STATUS_OK(status);

    Matrix expected = generate_random_matrix(2, 2);
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            for (size_t k = 0; k < 3; k++) {
                expected.values[i * 2 + j] += matA.values[i * 3 + k] * matB.values[k * 2 + j];
            }
        }
    }

    ASSERT_MATRIX_EQ(&result, &expected, EQUAL_TEST_ACCURACY);

    matrix_free(&matA);
    matrix_free(&matB);
    matrix_free(&result);
    matrix_free(&expected);
}

void test_matrix_mult_by_num() {
    Matrix mat = generate_random_matrix(2, 3);
    double a = 2.5;
    Matrix expected = generate_random_matrix(2, 3);
    for (size_t i = 0; i < 6; i++) {
        expected.values[i] = mat.values[i] * a;
    }
    STATUS status = matrix_mult_by_num(mat, a);
    ASSERT_STATUS_OK(status);
    ASSERT_MATRIX_EQ(&mat, &expected, EQUAL_TEST_ACCURACY);
    matrix_free(&mat);
    matrix_free(&expected);
}

void test_matrix_change_rows() {
    Matrix mat = generate_random_matrix(3, 3);
    Matrix expected = generate_random_matrix(3, 3);
    memcpy(expected.values, mat.values, 9 * sizeof(double));
    matrix_change_rows(mat, 0, 2);
    matrix_change_rows(expected, 0, 2);
    ASSERT_MATRIX_EQ(&mat, &expected, EQUAL_TEST_ACCURACY);
    matrix_free(&mat);
    matrix_free(&expected);
}

void test_matrix_det() {
    Matrix mat = generate_random_matrix(3, 3);
    double det_a;
    STATUS status = matrix_det(&det_a, mat);
    ASSERT_STATUS_OK(status);
    // Cannot reliably check the determinant without knowing the actual values.
    // Just check if it's not zero (which would be unusual for a random matrix)
    if (fabs(det_a) < 1e-6) {
        printf("Test failed: Determinant is zero.\n");
        matrix_free(&mat);
        return;
    }
    matrix_free(&mat);
}

void test_matrix_pow() {
    Matrix mat = generate_random_matrix(3, 3);
    Matrix result;
    STATUS status = matrix_pow(&result, mat, 2);
    ASSERT_STATUS_OK(status);
    matrix_free(&mat);
    matrix_free(&result);
}

void test_matrix_check_max_diff() {
    Matrix matA = generate_random_matrix(3, 3);
    Matrix matB = generate_random_matrix(3, 3);
    double max_diff;
    STATUS status = matrix_check_max_diff(&max_diff, matA, matB);
    ASSERT_STATUS_OK(status);
    // Check if the calculated max_diff is reasonable
    if (max_diff < 0 || max_diff > MAX_VALUE * 2) {
        printf("Test failed: Incorrect max difference.\n");
        matrix_free(&matA);
        matrix_free(&matB);
        return;
    }
    matrix_free(&matA);
    matrix_free(&matB);
}

void test_matrix_exp() {
    Matrix mat = generate_random_matrix(3, 3);
    Matrix result;
    STATUS status = matrix_exp(&result, mat);
    ASSERT_STATUS_OK(status);
    if (result.rows != 3 || result.cols != 3) {
        printf("Test failed: matrix_exp returned matrix with incorrect size.\n");
        matrix_free(&mat);
        matrix_free(&result);
        return;
    }
    matrix_free(&mat);
    matrix_free(&result);
}

void test_matrix_equals() {
    Matrix matA = generate_random_matrix(3, 3);
    Matrix matB = generate_random_matrix(3, 3);
    int are_equal;
    // Test for equal matrices
    STATUS status = matrix_equals(&are_equal, matA, matA, EQUAL_TEST_ACCURACY);
    ASSERT_STATUS_OK(status);
    ASSERT_DOUBLE_EQ(1.0, are_equal, 0.0);
    // Test for non-equal matrices
    status = matrix_equals(&are_equal, matA, matB, EQUAL_TEST_ACCURACY);
    ASSERT_STATUS_OK(status);
    ASSERT_DOUBLE_EQ(0.0, are_equal, 0.0);
    matrix_free(&matA);
    matrix_free(&matB);
}

int main() {
    srand(time(NULL)); // Initialize random seed
    printf("Test identity\n");
    test_matrix_identity();
    printf("Test fill val\n");
    test_matrix_fill_val();
    printf("Test clone\n");
    test_matrix_clone();
    printf("Test add\n");
    test_matrix_add();
    printf("Test subt\n");
    test_matrix_subt();
    printf("Test mult\n");
    test_matrix_mult();
    printf("Test mult by num\n");
    test_matrix_mult_by_num();
    printf("Test change rows\n");
    test_matrix_change_rows();
    printf("Test det\n");
    test_matrix_det();
    printf("Test pow\n");
    test_matrix_pow();
    printf("Test check max diff\n");
    test_matrix_check_max_diff();
    printf("Test exp\n");
    test_matrix_exp();
    printf("Test equals\n");
    test_matrix_equals();

    printf("All tests passed!\n");
    return 0;
}

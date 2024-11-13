#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Matrix 
{
    size_t rows;
    size_t cols;
    double* data;
};
typedef struct Matrix Matrix;

enum MatrixExceptionLevel {ERROR, WARNING, INFO, DEBUG};

const Matrix MATRIX_NULL = {0, 0, NULL};


// Сообщение об ошибке
void matrix_exception(const enum MatrixExceptionLevel level, char *msg)
{
    if(level == ERROR) {
        printf("ERROR: %s", msg);
    }

    if(level == WARNING) {
        printf("WARNING: %s", msg);
    }
    
    if(level == DEBUG) {
        printf("WARNING: %s", msg);
    }
}


Matrix matrix_alloc(const size_t rows, const size_t cols) 
{
    Matrix M;
    
    if (rows == 0 || cols == 0) {
        matrix_exception(WARNING, "Матрица содержит 0 столбцов или строк");
        return (Matrix) {rows, cols, NULL};
    }
    
    
    M.data = (double*)malloc(rows * cols * sizeof(double));
    
    if (M.data == NULL) {
        matrix_exception(WARNING, "Сбой выделения памяти");
        return MATRIX_NULL;
    }
    
    M.rows = rows;
    M.cols = cols;
    return M;
}


void matrix_free(Matrix* M)  // Функция для освобождения памяти матрицы
{
    free(M->data);
    *M = MATRIX_NULL;
}


// Создание единичной матрицы
Matrix identity_matrix(size_t size) 
{
    Matrix identity = {size, size, (double*)malloc(size * size * sizeof(double))};
    if (identity.data == NULL) {
        matrix_exception(ERROR, "Не удалось выделить память для единичной матрицы.\n");
        exit(1);
    }

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            identity.data[i * size + j] = (i == j) ? 1.0 : 0.0;
        }
    }

    return identity;
}

void matrix_print(const Matrix M) // Функция для печати матрицы
{
    for (size_t row = 0; row < M.rows; row++) {
        for (size_t col = 0; col < M.cols; col++) {
            printf("%.2f ", M.data[row * M.cols + col]);
        }
        printf("\n");
    }
}

// C = A + B 
Matrix matrix_sum(const Matrix A, const Matrix B) // Сложение матриц
{
    if (A.rows != B.rows || A.cols != B.cols) {
        matrix_exception(WARNING, "Размеры матриц не подходят для сложения.\n");
        return MATRIX_NULL;
    }

    Matrix C = matrix_alloc(A.rows, A.cols);
    
    for (size_t idx = 0; idx < C.rows * C.cols; idx++) {
        C.data[idx] = A.data[idx] + B.data[idx];
    }
    return C;
}


// C = A - B 
Matrix matrix_subtract(const Matrix A, const Matrix B) // Вычитание матриц
{
    if (A.rows != B.rows || A.cols != B.cols) {
        matrix_exception(WARNING, "Размеры матриц не подходят для вычитания.\n");
        return MATRIX_NULL;
    }

    Matrix C = matrix_alloc(A.rows, A.cols);
    for (size_t idx = 0; idx < C.rows * C.cols; idx++) {
        C.data[idx] = A.data[idx] - B.data[idx];
    }
    return C;
}


// C = A * B
Matrix matrix_multiply(const Matrix A, const Matrix B) // Умножение матриц
{
    if (A.cols != B.rows) {
        matrix_exception(WARNING,"Число столбцов первой матрицы не равно числу строк второй матрицы.\n");
        return MATRIX_NULL;
    }

    Matrix C = matrix_alloc(A.rows, B.cols);
    
    for (size_t row = 0; row < C.rows; row++) {
        for (size_t col = 0; col < C.cols; col++) {
            C.data[row * B.cols + col] = 0;
            for (size_t idx = 0; idx < A.cols; idx++) {
                C.data[row * C.cols + col] += A.data[row * A.cols + idx] * B.data[idx * B.cols + col];
            }
        }
    }
    return C;
}


Matrix matrix_transpose(const Matrix A) // Транспонирование матрицы
{
    Matrix T = matrix_alloc(A.cols, A.rows);
    
    for (size_t row = 0; row < T.rows; row++) {
        for (size_t col = 0; col < T.cols; col++) {
            T.data[col * T.cols + row] = A.data[row * A.cols + col];
        }
    }
    return T;
}


// Возведение матрицы в степень
Matrix matrix_power(const Matrix A, int power) 
{
    if (A.rows != A.cols) {
        matrix_exception(WARNING, "Матрица должна быть квадратной для возведения в степень.\n");
        return MATRIX_NULL;
    }
    
    Matrix result = identity_matrix(A.rows); // Создаем единичную матрицу

    for (int n = 0; n < power; n++) {
        Matrix temp = matrix_multiply(result, A);
        matrix_free(&result);
        result = temp;
    }

    return result;
}

// C = A * k
Matrix matrix_by_scalar(const Matrix A, double scalar) // Умножение матрицы на число
{
    Matrix C = matrix_alloc(A.rows, A.cols);
    
    for (size_t idx = 0; idx < C.rows * C.cols; idx++) {
        C.data[idx] = A.data[idx] * scalar;
    }
    return C;
}


double matrix_determinant(const Matrix A) // Определитель матрицы (для 2x2 и 3x3)
{
    if (A.rows != A.cols || A.cols != A.rows) {
        matrix_exception(WARNING, "Матрица должна быть квадратной для транспонирования.\n");
        return NAN;
    }
    
    if (A.rows == 1 && A.cols == 1) {
        return A.data[0];
    } 
    
    if (A.rows == 2 && A.cols == 2){
        return A.data[0] * A.data[3] - A.data[1] * A.data[2];
    }
    
    if (A.rows == 3 && A.cols == 3) {
        return A.data[0] * (A.data[4] * A.data[8] - A.data[5] * A.data[7]) -
               A.data[1] * (A.data[3] * A.data[8] - A.data[5] * A.data[6]) +
               A.data[2] * (A.data[3] * A.data[7] - A.data[4] * A.data[6]);
    }
    return 0; 
}


/*Matrix matrix_exponential(const Matrix A) // Матричная экспонента (приближенно)
{
    if (A.rows != A.cols || A.cols != A.rows) {
        printf("Ошибка: Матрица должна быть квадратной для вычисления экспоненты.n");
        return matrix_alloc(0, 0); // Возвращаем пустую матрицу
    }

    Matrix result = matrix_alloc(A.rows, A.cols);
    for (size_t i = 0; i < result.rows; i++) {
        for (size_t col = 0; col < result.cols; col++) {
            result.data[i * result.cols + col] = (i == col) ? 1.0 : 0.0; // Единичная матрица
        }
    }

    Matrix current_power = matrix_alloc(A.rows, A.cols);
    
    // Примерный алгоритм для вычисления экспоненты
    double factorial = 1.0;

    for (int n = 1; n <= 1; n++) { // Порядок разложения
        factorial *= n;

        if (n == 1) {
            current_power = matrix_by_scalar(A, 1.0);
        } else {
            Matrix temp_power = matrix_multiply(current_power, A);
            matrix_free(&current_power);
            current_power = temp_power;
        }

        Matrix temp_result = matrix_sum(result, matrix_by_scalar(current_power, 1.0 / factorial));
        matrix_free(&result);
        result = temp_result;
        
        matrix_free(&current_power);
        current_power = matrix_alloc(A.rows, A.cols);
    }

    matrix_free(&current_power);

    return result;
}*/

int main() 
{
    Matrix A = matrix_alloc(3,3);
    Matrix B = matrix_alloc(3, 3);

    double data_A[9] = {3, 1, 7, 0, 5, 7, 2, 5, 8};
    double data_B[9] = {5, 0, 8, 1, 9, 6, 3, 2, 1};

    memcpy(A.data, data_A, 9 * sizeof(double));
    memcpy(B.data, data_B, 9 * sizeof(double));

    // Печать исходных матриц
    printf("Матрица A:\n");
    matrix_print(A);

    printf("Матрица B:\n");
    matrix_print(B);

    // Сложение
    Matrix C = matrix_sum(A, B);
    printf("Результат сложения:\n");
    matrix_print(C);

    // Вычитание
    Matrix D = matrix_subtract(A, B);
    printf("Результат вычитания:\n");
    matrix_print(D);

    // Умножение
    Matrix E = matrix_multiply(A, B);
    printf("Результат умножения:\n");
    matrix_print(E);

    // Транспонирование
    Matrix T = matrix_transpose(A);
    printf("Транспонированная матрица A:\n");
    matrix_print(T);

    // Возведение в степень
    int power = 3;
    Matrix F = matrix_power(A, power);
    printf("Матрица A в степени %d:\n", power);
    matrix_print(F);

    // Умножение на число
    double scalar = 5;
    Matrix G = matrix_by_scalar(A, scalar);
    printf("Матрица A умноженная на %2.f:\n", scalar);
    matrix_print(G);
    
    // Определитель
    printf("Определитель матрицы A: %2.f \n", matrix_determinant(A));
    
    /* Матричная экспонента
    Matrix exp_result = matrix_exponential(A);
    printf("Матричная экспонента от A:\n");
    matrix_print(exp_result);*/

    // Освобождение памяти
    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&C);
    matrix_free(&D);
    matrix_free(&E);
    matrix_free(&T);
    matrix_free(&F);
    matrix_free(&G);
   // matrix_free(&exp_result);

    return 0;
}
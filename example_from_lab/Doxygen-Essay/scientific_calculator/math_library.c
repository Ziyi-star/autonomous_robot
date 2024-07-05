/**
 * @file math_library.c
 * @author Marek Herde
 * @date 23.05.2019
 * @brief Mathematical functions for scientific computing.
 *
 * @details This library contains the corresponding implementations of the mathematical functions
 * defined in the header file math_library.h.
 */

#include "math_library.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Matrix create_matrix(int n_rows, int n_columns){
    struct Matrix matrix;
    matrix.n_rows = n_rows;
    matrix.n_columns = n_columns;
    matrix.values = malloc(matrix.n_rows * sizeof(int *));
    if (matrix.values == NULL) {
        printf("Could not allocate memory!");
    } else {
        for (int i = 0; i < matrix.n_rows; i++) {
            matrix.values[i] = malloc(matrix.n_columns * sizeof(int));
            if (matrix.values[i] == NULL) {
                printf("Could not allocate memory!");
            }
        }
    }

    return matrix;
}

struct Matrix read_matrix(){
    int n_rows, n_columns;
    printf("Type in number of rows: ");
    scanf("%d", &n_rows);
    printf("Type in number of columns: ");
    scanf("%d", &n_columns);
    struct Matrix matrix = create_matrix(n_rows, n_columns);
    for (int i = 0; i < matrix.n_rows; i++){
        for (int j = 0; j < matrix.n_columns; j++) {
            printf("Type in: matrix[%d][%d] = ", i, j);
            scanf("%f", &matrix.values[i][j]);
        }
    }
    return matrix;
}

void print_matrix(struct Matrix matrix){
    printf("\n");
    for (int row = 0; row < matrix.n_rows; row++)
    {
        for(int columns = 0; columns < matrix.n_columns; columns++)
        {
            printf("%f\t", matrix.values[row][columns]);
        }
        printf("\n\n");
    }
}

struct Matrix transpose_matrix(struct Matrix m) {
    struct Matrix m_transposed = create_matrix(m.n_columns, m.n_rows);
    for (int i = 0; i < m.n_rows; i++) {
        for (int j = 0; j < m.n_columns; j++) {
            m_transposed.values[j][i] = m.values[i][j];
        }
    }
    return m_transposed;
}

struct Matrix add_matrices(struct Matrix m_1, struct Matrix m_2) {
    struct Matrix m_sum = create_matrix(m_1.n_rows, m_1.n_columns);
    for (int i = 0; i < m_sum.n_rows; i++) {
        for (int j = 0; j < m_sum.n_columns; j++) {
            m_sum.values[i][j] = m_1.values[i][j] + m_2.values[i][j];
        }
    }
    return m_sum;
}

struct Matrix multiply_matrices(struct Matrix m_1, struct Matrix m_2) {
    struct Matrix m_product = create_matrix(m_1.n_rows, m_2.n_columns);
    for (int i = 0; i < m_product.n_rows; i++) {
        for (int j = 0; j < m_product.n_columns; j++) {
            float sum = 0;
            /*
             * Multiply row of first matrix to column of second matrix
             * and store sum of product of elements in sum.
             */
            for (int k = 0; k < m_1.n_columns; k++) {
                sum += m_1.values[i][k] * m_2.values[k][j];
            }

            m_product.values[i][j] = sum;
        }
    }
    return m_product;
}

float poly_discriminant(float a, float b, float c)
{
    float discriminant = b * b - 4 * a * c;
    return discriminant;
}

int factorial(int n){
    if (n == 0)
        return 1;
    else
        return n * factorial(n-1);
}

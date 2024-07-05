/**
 * @file math_library.h
 * @author Marek Herde
 * @date 23.05.2019
 * @brief Mathematical functions for scientific computing.
 *
 * @details This library contains several mathematical functions and a list of them is given below.
 *
 * - Transpose matrix.
 * - Add two matrices.
 * - Multiply two matrices.
 * - Compute roots of quadratic polynomial.
 * - Compute factorial.
 */
#ifndef SCIENTIFIC_CALCULATOR_MATH_LIBRARY_H
#define SCIENTIFIC_CALCULATOR_MATH_LIBRARY_H

/**
 * @struct Complex
 *
 * @brief Struct representing a complex number consisting of a real and an imaginary number.
 *
 * @var Complex::real
 * Member 'real' represents the real valued part of the complex number.
 * @var Complex::imag
 * Member 'imag' represents the imaginary valued part of the complex number.
 *
 * @see https://en.wikipedia.org/wiki/Complex_number (last access: 23.05.2019)
 */
struct Complex {
    float real;
    float imag;
};

/**
 * @struct Matrix
 *
 * @brief Struct representing a two dimensional matrix.
 *
 * @var Matrix::n_rows
 * Member 'n_rows' represents the number of rows of the matrix.
 * @var Matrix::n_columns
 * Member 'n_columns' represents the number of columns of the matrix.
 * @var Matrix::values
 * Member 'values' is a double pointer containing the values of the matrix.
 *
 * @see https://en.wikipedia.org/wiki/Matrix_(mathematics) (last access: 23.05.2019)
 */
struct Matrix {
    int n_rows;
    int n_columns;
    float **values;
};

/**
 * @brief Allocates and creates a matrix with given input dimensions.
 *
 * @param n_rows number of rows
 * @param n_columns number of columns
 *
 * @return  created matrix
 */
struct Matrix create_matrix(int n_rows, int n_columns);

/**
 * @brief Reads in a matrix entered by the user.
 *
 * @return entered matrix
 */
struct Matrix read_matrix();

/**
 * @brief Prints input matrix.
 *
 * @param m matrix to be printed
 */
void print_matrix(struct Matrix m);

/**
 * @brief This functions transposes a given input matrix.
 *
 * @details Denoting the input matrix by \f$M\f$, the transpose \f$M^T\f$ is returned.
 *
 * @param m matrix to be transposed
 *
 * @return transposed matrix
 */
struct Matrix transpose_matrix(struct Matrix m);

/**
 * @brief This functions adds two given input matrices.
 *
 * @details Denoting the matrix by resulting matrix by \f$M\f$ and the input matrices by \f$X\f$ and \f$Y\f$,
 * the addition is described by \f$M = X \cdot Y\f$.
 *
 * @param m_1 first matrix
 * @param m_2 second matrix
 *
 * @return sum of the matrices 'm_1' and 'm_2'
 */
struct Matrix add_matrices(struct Matrix m_1, struct Matrix m_2);

/**
 * @brief This functions computes the product of two given input matrices.
 *
 * @details Denoting the product matrix by \f$M \in \mathbb{R}^{n \times m}\f$ and the input matrices by
 * \f$X \in \mathbb{R}^{n \times t}\f$ and \f$Y \in \mathbb{R}^{t \times m}\f$,
 * the product is described by \f$M = X \cdot Y\f$.
 *
 * @param m_1 first matrix
 * @param m_2 second matrix
 *
 * @return product of the matrices 'm_1' and 'm_2'
 */
struct Matrix multiply_matrices(struct Matrix m_1, struct Matrix m_2);

/**
 * @brief Computes the discriminant of a polynomial function of degree two.
 *
 * @details A polynomial function of degree two is defined by \f$f(x) = ax^2 + bx + c\f$.
 * The corresponding discriminant is given by \f$b^2 - 4ac\f$.
 *
 * @param a coefficient of polynomial of second order
 * @param b coefficient of polynomial of first order
 * @param c coefficient of polynomial of zeroth order
 *
 * @return discriminant of polynomial function
 *
 * @see https://en.wikipedia.org/wiki/Discriminant
 */
float poly_discriminant(float a, float b, float c);

/**
 * @brief Computes the factorial of a positive integer.
 *
 * @param n being a positive integer whose factorial is to be computed
 * @return factorial of the parameter
 *
 * @see https://en.wikipedia.org/wiki/Factorial (last access: 23.05.2019)
 */
int factorial(int n);

#endif //SCIENTIFIC_CALCULATOR_MATH_LIBRARY_H

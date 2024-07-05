/**
 * @file calculator.c
 * @author Marek Herde
 * @date 23.05.2019
 * @brief Calculator for scientific computing.
 */
#include "math_library.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>


/**
 * @brief Main function for execution of scientific calculator.
 */
int main(){
    int choice;
    while(1){
        printf("\n********* Scientific Calculator *********");
        printf("\nChoose one of the operations by its index.\n\n");
        printf("1. Transpose matrix\n");
        printf("2. Add two matrices\n");
        printf("3. Multiply two matrices\n");
        printf("4. Compute roots of quadratic polynomial\n");
        printf("5. Compute factorial\n");
        printf("6. Exit\n\n");
        printf("Enter your choice: ");
        scanf("\n%d", &choice);

        if (choice == 1) {
            printf("\n'Transpose matrix' selected.\n\n");
            struct Matrix m = read_matrix();
            printf("Input matrix:\n");
            print_matrix(m);
            struct Matrix m_transposed = transpose_matrix(m);
            printf("Transposed matrix:\n");
            print_matrix(m_transposed);

        } else if (choice == 2) {
            printf("\n'Add two matrices' selected.\n\n");
            struct Matrix m_1 = read_matrix();
            printf("\nFirst matrix:\n");
            print_matrix(m_1);
            struct Matrix m_2 = read_matrix();
            printf("\nSecond matrix:\n");
            print_matrix(m_2);
            if (m_1.n_rows == m_2.n_rows && m_1.n_columns == m_2.n_columns) {
                printf("Matrix sum:\n");
                struct Matrix m_sum = add_matrices(m_1, m_2);
                print_matrix(m_sum);
            } else {
                printf("Dimensions of first matrix and second matrix are not equal.\n");
            }

        } else if (choice == 3) {
                printf("\n'Multiply two matrices' selected.\n\n");
                struct Matrix m_1 = read_matrix();
                printf("\nFirst matrix:\n");
                print_matrix(m_1);
                struct Matrix m_2 = read_matrix();
                printf("\nSecond matrix:\n");
                print_matrix(m_2);
                if (m_1.n_columns == m_2.n_rows) {
                    printf("Matrix product:\n");
                    struct Matrix m_product = multiply_matrices(m_1, m_2);
                    print_matrix(m_product);
                } else {
                    printf("Number of columns of first matrix and number of rows of second matrix are not equal.\n");
                }

        } else if (choice == 4) {
            printf("\n'Compute roots of quadratic polynomial' selected.\n\n");
            printf("Enter a, b and c of the polynomial f(x) = ax^2 + bx + c: ");
            float a, b, c, root1, root2;
            scanf("%f %f %f", &a, &b, &c);
            float discriminant = poly_discriminant(a, b, c);
            if (discriminant == 0) {
                printf("\nThe given polynomial has two real and equal roots\n");
                printf("Both roots of the polynomial are %f.\n", (-b) / (2 * a));
            } else if (discriminant > 0) {
                root1 = (-b + sqrt(discriminant)) / (2 * a);
                root2 = (-b - sqrt(discriminant)) / (2 * a);
                printf("\nThe given polynomial has two distinct real roots");
                printf("\nThe roots of the polynomial are %f and %f.\n", root1, root2);
            }
            else {
                struct Complex c;
                c.real = -b / (2.0 * a);
                c.imag = sqrt(abs((int) discriminant)) / (2 * a);
                printf("\nThe given polynomial has a complex conjugate root.");
                printf("\nThe roots are: %f +- %fi.\n", c.real, c.imag);
            }

        } else if (choice == 5) {
            printf("\n'Compute factorial' selected.\n\n");
            int n;
            printf("Type in integer n = ");
            scanf("%d", &n);
            printf("Factorial of n is n! = %d\n", factorial(n));

        } else if (choice == 6) {
                return EXIT_SUCCESS;

        } else {
            printf("\nYou entered an invalid choice, enter only 1 to 6.\n");
        }
    }
}


#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <stdbool.h>

typedef struct matrix {
    uint32_t rows;
    uint32_t cols;
    double* data;
} matrix;

matrix* matrix_create(uint32_t rows, uint32_t cols);

bool matrix_copy(matrix* A, matrix* B);

void matrix_free(matrix* A);

void matrix_fill(matrix* A, double x);

void matrix_fill_zeros(matrix* A);

bool matrix_fill_identity(matrix* A);

bool matrix_add(matrix* out, const matrix* A, const matrix* B);

bool matrix_subtract(matrix* out, const matrix* A, const matrix* B);

bool matrix_multiply_mm(matrix* out, const matrix* A, const matrix* B);

bool matrix_multiply_tm(matrix* out, const matrix* A, const matrix* B);

bool matrix_multiply_mt(matrix* out, const matrix* A, const matrix* B);

bool matrix_multiply_tt(matrix* out, const matrix* A, const matrix* B);

bool matrix_multiply(matrix* out, const matrix* A, const matrix* B, bool transposeA, bool transposeB);

double matrix_inner_product(const matrix* v1, const matrix* v2);

bool matrix_outer_product(matrix* out, const matrix* v1, const matrix* v2);

double matrix_norm(const matrix* A);

void matrix_multiply_scalar(matrix* A, double c);

#endif
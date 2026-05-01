#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct matrix {
    uint32_t rows;
    uint32_t cols;
    double* data;
} matrix;

matrix* matrix_create(uint32_t rows, uint32_t cols) {
    matrix* mat = (matrix *)malloc(sizeof(matrix));

    if (mat == NULL) {
        printf("No memory to allocate matrix struct\n");
        exit(-1);
    }

    mat->rows = rows;
    mat->cols = cols;
    mat->data = (double *)malloc(rows * cols * sizeof(double));

    if (mat->data == NULL) {
        printf("No memory to allocate matrix\n");
        free(mat->data);
        free(mat);
        exit(-1);
    }
    return mat;  
};

void matrix_free(matrix* A) {
    free(A->data);
    free(A);
}

void matrix_fill(matrix* A, double x) {
    uint64_t size = (uint64_t)A->rows * A->cols;

    for (uint64_t i = 0; i < size; i++) {
        A->data[i] = x;
    }
}

void matrix_fill_zeros(matrix* A) {
    memset(A->data, 0, sizeof(double) * (uint64_t)A->rows * A->cols);
}

bool matrix_add(matrix* out, const matrix* A, const matrix* B) { 
    if (A->cols != B->cols || A->rows != B->rows) { return false; }

    uint64_t size = (uint64_t)A->rows * A->cols;

    for (uint64_t i = 0; i < size; i++) {
        out->data[i] = A->data[i] + B->data[i];
    }
    return true;
}

bool matrix_subtract(matrix* out, const matrix* A, const matrix* B) { // A - B
    if (A->cols != B->cols || A->rows != B->rows) { return false; }

    uint64_t size = (uint64_t)A->rows * A->cols;

    for (uint64_t i = 0; i < size; i++) {
        out->data[i] = A->data[i] - B->data[i];
    }
    return true;
}

bool matrix_multiply_mm(matrix* out, const matrix* A, const matrix* B) {
    if (A->cols != B->rows) { return false; }

    for (uint64_t i = 0; i < A->rows; i++) {
        for (uint64_t j = 0; j < B->rows; j++) {   // B->rows == A->cols
            for (uint64_t k = 0; k < B->cols; k++) {
                out->data[k + B->cols*i] += A->data[j + A->cols*i] * B->data[B->cols*j + k];
            }
        }
    }
    return true;
}

bool matrix_multiply_tm(matrix* out, const matrix* A, const matrix* B) { // this is for A.T @ B
    if (A->rows != B->rows) { return false; }

    for (uint64_t j = 0; j < B->rows; j++) {   // A->rows == B->rows
        for (uint64_t i = 0; i < A->cols; i++) {   
            for (uint64_t k = 0; k < B->cols; k++) {
                out->data[k + B->cols*i] += A->data[i + A->cols*j] * B->data[B->cols*j + k];
            }
        }
    }
    return true;
}

bool matrix_multiply_mt(matrix* out, const matrix* A, const matrix* B) { // this is for A @ B.T
    if (A->cols != B->cols) { return false; }

    for (uint64_t i = 0; i < A->rows; i++) {
        for (uint64_t k = 0; k < B->rows; k++) { 
            for (uint64_t j = 0; j < B->cols; j++) { // A->cols == B->cols
                out->data[k + B->rows*i] += A->data[j + A->cols*i] * B->data[B->cols*k + j];
            }
        }
    }
    return true;
}

bool matrix_multiply_tt(matrix* out, const matrix* A, const matrix* B) { // this is for A.T @ B.T
    if (B->cols != A->rows) { return false; }

    for (uint64_t i = 0; i < A->cols; i++) {
        for (uint64_t j = 0; j < B->cols; j++) { 
            for (uint64_t k = 0; k < B->rows; k++) {
                out->data[k + B->rows*i] += A->data[i + A->cols*j] * B->data[B->cols*k + j];
            }
        }
    }
    return true;
}

bool matrix_multiply(matrix* out, const matrix* A, const matrix* B, bool transposeA, bool transposeB) {
    matrix_fill_zeros(out);
    if (transposeA == false && transposeB == false) { return matrix_multiply_mm(out, A, B); }
    else if (transposeA == true && transposeB == false) { return matrix_multiply_tm(out, A, B); }
    else if (transposeA == false && transposeB == true) { return matrix_multiply_mt(out, A, B); }
    else { return matrix_multiply_tt(out, A, B); }
}

double matrix_inner_product(const matrix* v1, const matrix* v2) {
    double out = 0.0;
    if (!(v1->rows == v2->rows && v1->cols == v2->cols && (v1->rows == 1 || v1->cols == 1))) {
        printf("Invalid inner product\n");
        exit(-1);
    }
    uint64_t size = (uint64_t)v1->rows * v1->cols;
    for (uint64_t i = 0; i < size; i++) {
        out += v1->data[i] * v2->data[i];
    }
    return out;
}

bool matrix_outer_product(matrix* out, const matrix* v1, const matrix* v2) {
    uint32_t min_dim_v1 = v1->rows > v1->cols ? v1->cols: v1->rows;
    uint32_t min_dim_v2 = v2->rows > v2->cols ? v2->cols: v2->rows;

    if (min_dim_v1 != 1 || min_dim_v2 != 1) {
        return false;
    }

    uint32_t max_dim_v1 = v1->rows < v1->cols ? v1->cols: v1->rows;
    uint32_t max_dim_v2 = v2->rows < v2->cols ? v2->cols: v2->rows;
    matrix_fill_zeros(out);
    for (uint32_t i = 0; i < max_dim_v1; i++) {
        for (uint32_t j = 0; j < max_dim_v2; j++) {
            out->data[max_dim_v2 * i + j] = v1->data[i] * v2->data[j];
        }
    }
    return true; 
}

double matrix_norm(const matrix* A) {
    /* for matrices, this will return the frobenius norm; for vectors, l2 norm */
    uint64_t size = (uint64_t)A->rows * A->cols;
    double out = 0.0;
    for (uint64_t i = 0; i < size; i++) {
        out += A->data[i] * A->data[i];
    }
    return sqrt(out);
}

void matrix_multiply_scalar(matrix* A, double c) {
    uint64_t size = (uint64_t)A->rows * A->cols;

    for (uint64_t i = 0; i < size; i++) {
        A->data[i] *= c;
    }
}

int main() {
    return 0;
}
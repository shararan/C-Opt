#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct matrix {
    u_int32_t rows;
    u_int32_t cols;
    float* data;
} matrix;

matrix* matrix_create(u_int32_t rows, u_int32_t cols) {
    matrix* mat = (matrix *)malloc(sizeof(matrix));

    mat->rows = rows;
    mat->cols = cols;
    mat->data = (float *)malloc(rows * cols * sizeof(float));

    return mat;  
};

void matrix_fill(matrix* A, float x) {
    u_int64_t size = (u_int64_t)A->rows * A->cols;

    for (u_int64_t i = 0; i < size; i++) {
        A->data[i] = x;
    }
}

matrix* matrix_add(const matrix* A, const matrix* B) {
    if (A->cols != B->cols || A->rows != B->rows) { return NULL; }

    u_int64_t size = (u_int64_t)A->rows * A->cols;
    matrix* out = matrix_create(A->rows, A->cols);

    for (u_int64_t i = 0; i < size; i++) {
        out->data[i] = A->data[i] + B->data[i];
    }

    return out;
}

matrix* matrix_multiply_mm(const matrix* A, const matrix* B) {
    if (A->cols != B->rows) { return NULL; }

    matrix* out = matrix_create(A->rows, B->cols);
    matrix_fill(out, 0.f);

    for (u_int64_t i = 0; i < A->rows; i++) {
        for (u_int64_t j = 0; j < B->rows; j++) {   // B->rows == A->cols
            for (u_int64_t k = 0; k < B->cols; k++) {
                out->data[k + B->cols*i] += A->data[j + A->cols*i] * B->data[B->cols*j + k];
            }
        }
    }
    return out;
}

matrix* matrix_multiply_tm(const matrix* A, const matrix* B) { // this is for A.T @ B
    if (A->rows != B->rows) { return NULL; }

    matrix* out = matrix_create(A->cols, B->cols);
    matrix_fill(out, 0.f);

    for (u_int64_t j = 0; j < B->rows; j++) {   // A->rows == B->rows
        for (u_int64_t i = 0; i < A->cols; i++) {   
            for (u_int64_t k = 0; k < B->cols; k++) {
                out->data[k + B->cols*i] += A->data[i + A->cols*j] * B->data[B->cols*j + k];
            }
        }
    }
    return out;
}

matrix* matrix_multiply_mt(const matrix* A, const matrix* B) { // this is for A @ B.T
    if (A->cols != B->cols) { return NULL; }

    matrix* out = matrix_create(A->rows, B->rows);
    matrix_fill(out, 0.f);

    for (u_int64_t i = 0; i < A->rows; i++) {
        for (u_int64_t k = 0; k < B->rows; k++) { 
            for (u_int64_t j = 0; j < B->cols; j++) { // A->cols == B->cols
                out->data[k + B->rows*i] += A->data[j + A->cols*i] * B->data[B->cols*k + j];
            }
        }
    }
    return out;
}

matrix* matrix_multiply_tt(const matrix* A, const matrix* B) { // this is for A.T @ B.T
    if (B->cols != A->rows) { return NULL; }

    matrix* out = matrix_create(A->cols, B->rows);
    matrix_fill(out, 0.f);

    for (u_int64_t i = 0; i < A->cols; i++) {
        for (u_int64_t j = 0; j < B->cols; j++) { 
            for (u_int64_t k = 0; k < B->rows; k++) {
                out->data[k + B->rows*i] += A->data[i + A->cols*j] * B->data[B->cols*k + j];
            }
        }
    }
    return out;
}

matrix* matrix_multiply(const matrix* A, const matrix* B, bool transposeA, bool transposeB) {
    if (transposeA == false && transposeB == false) { return matrix_multiply_mm(A, B); }
    else if (transposeA == true && transposeB == false) { return matrix_multiply_tm(A, B); }
    else if (transposeA == false && transposeB == true) { return matrix_multiply_mt(A, B); }
    else if (transposeA == true && transposeB == true) { return matrix_multiply_tt(A, B); }
    else { return NULL; }
}

matrix* matrix_multiply_scalar(const matrix* A, float c) {
    u_int64_t size = (u_int64_t)A->rows * A->cols;
    matrix* out = matrix_create(A->rows, A->cols);

    for (u_int64_t i = 0; i < size; i++) {
        out->data[i] = A->data[i] * c;
    }
    return out;

}

int main() {
    return 0;
}
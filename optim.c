#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <matrix.h>

typedef double obj_fn (matrix *x);
typedef void grad_fn (matrix* grad, matrix* x);

double fn(matrix *x) {
    double out = 0.0;
    uint64_t size = (uint64_t)x->rows * x->cols;
    for (uint64_t i = 0; i < size; i++) {
        out += x->data[i] * x->data[i];
    }
    return out;
}

void g_fn(matrix* out, matrix* x) {
    uint64_t size = (uint64_t)x->rows * x->cols;
    for (uint64_t i = 0; i < size; i++) {
        out->data[i] = 2 * x->data[i];
    }
}

double line_search(obj_fn* f, grad_fn* g, matrix* x, matrix* grad) {

}

int main() {
    return 0;
}
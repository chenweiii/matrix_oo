#include "matrix.h"
#include "stopwatch.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST_ROW 12
#define TEST_COL 12
#define SAMPLES_NUM 100

MatrixAlgo *matrix_providers[] = {
    &NaiveMatrixProvider,
    &SSEMatrixProvider,
    &prefetchSSEMatrixProvider,
    &AVXMatrixProvider,
    &prefetchAVXMatrixProvider,
};

int *random_matrix(int row, int col)
{
    int *src = (int *) malloc(row * col * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            *(src + i * col + j) = rand();
    return src;
}

int main()
{
    MatrixAlgo *algo = matrix_providers[3];
    MatrixAlgo *verify = matrix_providers[0];

    Matrix *dst, *m, *n, *fixed;
    watch_p ctx = Stopwatch.create();

    m = algo->create(TEST_ROW, TEST_COL);
    algo->assign(m, random_matrix(TEST_ROW, TEST_COL));
    n = algo->create(TEST_ROW, TEST_COL);
    algo->assign(n, random_matrix(TEST_ROW, TEST_COL));

    Stopwatch.start(ctx);
    algo->mul(&dst, m, n);
    double time = Stopwatch.read(ctx);
    Stopwatch.destroy(ctx);

    verify->mul(&fixed, m, n);

    if (algo->equal(dst, fixed)) {
        printf("time: %f ms\n", time);
        return 0;
    }
    return -1;
}

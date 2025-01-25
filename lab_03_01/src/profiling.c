#include "profiling.h"

static void int_sum(const void *res, const void *num1, const void *num2)
{
    *(int *)res = *((int *)num1) + *((int *)num2);
}

int time_measurement()
{
    matrix_t matrix1 = NULL, matrix2 = NULL;
    csr_matrix_t csr_matrix1 = NULL, csr_matrix2 = NULL;
    size_t sizes[3] = {10, 50, 100};
    int rc;

    struct timespec start, end;
    int64_t time_delta_ns_normal = 0;
    long long time_normal[11][3];
    int64_t time_delta_ns_csr = 0;
    long long time_csr[11][3];

    for (size_t i = 0; i < 11; i++)
        for (size_t j = 0; j < 3; j++)
        {
            rc = matrix_create(&matrix1, sizeof(int), sizes[j], sizes[j]);
            if (rc != EXIT_SUCCESS)
                goto failed;
            rc = matrix_create(&matrix2, sizeof(int), sizes[j], sizes[j]);
            if (rc != EXIT_SUCCESS)
                goto failed;

            rc = csr_matrix_create(&csr_matrix1, sizeof(int), sizes[j], sizes[j]); 
            if (rc != EXIT_SUCCESS)
                goto failed;           
            rc = csr_matrix_create(&csr_matrix2, sizeof(int), sizes[j], sizes[j]);
            if (rc != EXIT_SUCCESS)
                goto failed;

            time_delta_ns_normal = 0;
            time_delta_ns_csr = 0;

            for (size_t k = 0; k < REPEATS_NUM; k++)
            {
                matrixes_init(&matrix1, &csr_matrix1, (double)i / 10.0);
                if (rc != EXIT_SUCCESS)
                    goto failed;
                matrixes_init(&matrix2, &csr_matrix2, (double)i / 10.0);
                if (rc != EXIT_SUCCESS)
                    goto failed;

                clock_gettime(CLOCK_MONOTONIC_RAW, &start);
                matrix_sum(&matrix1, &matrix2, int_sum);
                clock_gettime(CLOCK_MONOTONIC_RAW, &end);
                time_delta_ns_normal += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

                clock_gettime(CLOCK_MONOTONIC_RAW, &start);
                csr_matrix_sum(&csr_matrix1, &csr_matrix2, int_sum);
                clock_gettime(CLOCK_MONOTONIC_RAW, &end);
                time_delta_ns_csr += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
            }

            time_normal[i][j] = time_delta_ns_normal / REPEATS_NUM;
            time_csr[i][j] = time_delta_ns_csr / REPEATS_NUM;
        }

    char *separator = "+-----------+--------------+--------------+-----------------+--------------------+";

    size_t matrix_mem_size;
    size_t csr_matrix_mem_size;

    for (size_t j = 0; j < 3; j++)
    {
        printf("Размер матрицы %zux%zu:\n", sizes[j], sizes[j]);
        printf("%s\n", separator);
        printf("| Процент   | Обычная      | Разреженная  | Размер матрицы, | Размер разреженной |\n");
        printf("| заполения | матрица, нс  | матрица, нс  | байт            | матрицы, байт      |\n");
        printf("%s\n", separator);
        for (size_t i = 0; i < 11; i++)
        {
            matrix_mem_size = sizeof(matrix_t) + 3 * sizeof(size_t) + sizes[j] * sizeof(void *) + sizes[j] * sizes[j] * sizeof(int);
            csr_matrix_mem_size = sizeof(csr_matrix_t) + sizeof(size_t) + (3 * sizeof(size_t) + sizeof(void *) + sizeof(int) * sizes[j] * sizes[j] * (i / 10.0)) 
            + (3 * sizeof(size_t) + sizeof(void *) + sizeof(size_t) * sizes[j] * sizes[j] * (i / 10.0)) + (3 * sizeof(size_t) + sizeof(void *) + sizeof(size_t) * (sizes[j]  + 1)); 
            printf("| %-9zu | %-12lld | %-12lld | %-15zu | %-18zu |\n", i * 10, time_normal[i][j], time_csr[i][j], matrix_mem_size, csr_matrix_mem_size);
            printf("%s\n", separator);
        }
    }

    failed:
    {
        matrix_destroy(&matrix1);
        matrix_destroy(&matrix2);
        csr_matrix_destroy(&csr_matrix1);
        csr_matrix_destroy(&csr_matrix1);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int matrixes_init(matrix_t *matrix, csr_matrix_t *csr_matrix, double density)
{
    size_t elem_num = (*matrix)->rows * (*matrix)->columns;
    size_t non_zero_elems_num = elem_num * density;

    srand(time(NULL));

    int *indexes = malloc(elem_num * sizeof(size_t));
    if (indexes == NULL)
        return EXIT_FAILURE;

    for (int i = 0; i < elem_num; i++)
        indexes[i] = i;

    for (int i = 0; i < elem_num; i++)
    {
        int j = i + rand() % (elem_num - i);
        int tmp = indexes[i];
        indexes[i] = indexes[j];
        indexes[j] = tmp;
    }

    size_t row, col;
    int tmp, rc; 
    for (size_t i = 0; i < non_zero_elems_num; i++)
    {
        row = indexes[i] / (*matrix)->columns;
        col = indexes[i] % (*matrix)->columns;
        tmp = rand() % 1000 + 1;
        *(int *)matrix_at(*matrix, row, col) = tmp;
        rc = csr_matrix_put(csr_matrix, &tmp, row, col);
        if (rc != CSR_MATRIX_OK)
        {
            free(indexes);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = non_zero_elems_num; i < elem_num; i++)
    {
        row = indexes[i] / (*matrix)->columns;
        col = indexes[i] % (*matrix)->columns;
        *(int *)matrix_at(*matrix, row, col) = 0;
    }

    free(indexes);

    return EXIT_SUCCESS;
}

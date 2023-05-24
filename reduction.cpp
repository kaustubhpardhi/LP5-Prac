#include <stdio.h>
#include <omp.h>

#define ARRAY_SIZE 1000
#define NUM_THREADS 8

int main() {
    int array[ARRAY_SIZE];
    int i, tid, chunk;
    int min_val, max_val, sum_val;
    double avg_val;

    // Initialize the array
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }

    // Set the chunk size for parallelization
    chunk = ARRAY_SIZE / NUM_THREADS;

    // Initialize the reduction variables
    min_val = array[0];
    max_val = array[0];
    sum_val = 0;

    // Parallel reduction for finding min, max, and sum
#pragma omp parallel private(i, tid) shared(array, min_val, max_val, sum_val) num_threads(NUM_THREADS)
    {
        tid = omp_get_thread_num();
        // Find local min, max, and sum for each thread
        int local_min = array[tid * chunk];
        int local_max = array[tid * chunk];
        int local_sum = 0;

        // Compute local min, max, and sum
        for (i = tid * chunk; i < (tid + 1) * chunk; i++) {
            if (array[i] < local_min) {
                local_min = array[i];
            }
            if (array[i] > local_max) {
                local_max = array[i];
            }
            local_sum += array[i];
        }

        // Perform reduction for min, max, and sum
#pragma omp critical
        {
            if (local_min < min_val) {
                min_val = local_min;
            }
            if (local_max > max_val) {
                max_val = local_max;
            }
            sum_val += local_sum;
        }
    }

    // Compute the average
    avg_val = (double)sum_val / ARRAY_SIZE;

    printf("Min: %d\n", min_val);
    printf("Max: %d\n", max_val);
    printf("Sum: %d\n", sum_val);
    printf("Average: %lf\n", avg_val);

    return 0;
}

// gcc -fopenmp parallel_reduction.c -o parallel_reduction


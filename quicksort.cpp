#include <iostream>
#include <omp.h>

// Function to swap two elements
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Function to partition the array
int partition(int* array, int low, int high) {
    int pivot = array[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
            swap(array[i], array[j]);
        }
    }

    swap(array[i + 1], array[high]);
    return i + 1;
}

// Quicksort algorithm
void quicksort(int* array, int low, int high) {
    if (low < high) {
        int pivot = partition(array, low, high);

#pragma omp parallel sections
        {
#pragma omp section
            {
                // Sort left subarray recursively
                quicksort(array, low, pivot - 1);
            }

#pragma omp section
            {
                // Sort right subarray recursively
                quicksort(array, pivot + 1, high);
            }
        }
    }
}

int main() {
    const int arraySize = 10;
    int array[arraySize] = { 9, 5, 1, 8, 3, 7, 4, 6, 2, 10 };

    // Print the original array
    std::cout << "Original Array: ";
    for (int i = 0; i < arraySize; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    // Sort the array
    quicksort(array, 0, arraySize - 1);

    // Print the sorted array
    std::cout << "Sorted Array: ";
    for (int i = 0; i < arraySize; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}


// g++ -fopenmp quicksort_openmp.cpp -o quicksort_openmp

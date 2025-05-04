#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>
#include <ctime>

using namespace std;

// Helper function to print the array (for testing purposes)
void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

// Sequential Bubble Sort
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Parallel Bubble Sort using OpenMP
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        #pragma omp parallel for
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Sequential Merge Sort
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;

    sequentialMergeSort(arr, left, mid);
    sequentialMergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Parallel Merge Sort using OpenMP
void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    // Parallelizing the recursive merge sort calls
    #pragma omp parallel sections
    {
        #pragma omp section
        parallelMergeSort(arr, left, mid);

        #pragma omp section
        parallelMergeSort(arr, mid + 1, right);
    }
    merge(arr, left, mid, right);
}

int main() {
    int n = 10000; // Array size (can be adjusted)
    vector<int> arr(n);

    // Fill array with random numbers
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;  // random numbers between 0 and 9999
    }

    vector<int> arrCopy = arr; // To keep the original array for both algorithms

    // --- Sequential Bubble Sort ---
    vector<int> seqArr = arr;  // Create copy for sequential sort
    double start = omp_get_wtime();
    sequentialBubbleSort(seqArr);
    double end = omp_get_wtime();
    cout << "Sequential Bubble Sort time: " << (end - start) << " seconds" << endl;

    // --- Parallel Bubble Sort ---
    vector<int> parArr = arr;  // Create copy for parallel sort
    start = omp_get_wtime();
    parallelBubbleSort(parArr);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort time: " << (end - start) << " seconds" << endl;

    // --- Sequential Merge Sort ---
    seqArr = arr;  // Reset the array to original for sequential merge sort
    start = omp_get_wtime();
    sequentialMergeSort(seqArr, 0, n - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort time: " << (end - start) << " seconds" << endl;

    // --- Parallel Merge Sort ---
    parArr = arr;  // Reset the array for parallel merge sort
    start = omp_get_wtime();
    parallelMergeSort(parArr, 0, n - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort time: " << (end - start) << " seconds" << endl;

    return 0;
}
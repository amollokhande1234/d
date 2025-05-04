#include <iostream>
#include <vector>
#include <omp.h>    // OpenMP header
#include <cstdlib>  // For rand()

using namespace std;

int main() {
    // Uncomment below if you want to manually set number of threads
    // omp_set_num_threads(4); // Example: use 4 threads

    int n = 1000000; // Size of array (10 lakh elements)
    vector<int> nums(n);

    // Fill the vector with random numbers
    for (int i = 0; i < n; i++) {
        nums[i] = rand() % 10000; // random numbers from 0 to 9999
    }

    int min_seq, max_seq, sum_seq;
    double avg_seq;
    int min_par, max_par, sum_par;
    double avg_par;
    double start, end;

    // --- Sequential Version ---
    min_seq = max_seq = nums[0];
    sum_seq = 0;
    start = omp_get_wtime();
    for (int i = 0; i < n; i++) {
        if (nums[i] < min_seq) min_seq = nums[i];
        if (nums[i] > max_seq) max_seq = nums[i];
        sum_seq += nums[i];
    }
    avg_seq = (double)sum_seq / n;
    end = omp_get_wtime();
    double time_seq = end - start;

    // --- Parallel Version ---
    min_par = max_par = nums[0];
    sum_par = 0;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(min: min_par) reduction(max: max_par) reduction(+: sum_par)
    for (int i = 0; i < n; i++) {
        if (nums[i] < min_par) min_par = nums[i];
        if (nums[i] > max_par) max_par = nums[i];
        sum_par += nums[i];
    }
    avg_par = (double)sum_par / n;
    end = omp_get_wtime();
    double time_par = end - start;

    // --- Output ---
    cout << "=== Sequential Results ===" << endl;
    cout << "Minimum  : " << min_seq << endl;
    cout << "Maximum  : " << max_seq << endl;
    cout << "Sum      : " << sum_seq << endl;
    cout << "Average  : " << avg_seq << endl;
    cout << "Time     : " << time_seq << " seconds" << endl;

    cout << "\n=== Parallel Results ===" << endl;
    cout << "Minimum  : " << min_par << endl;
    cout << "Maximum  : " << max_par << endl;
    cout << "Sum      : " << sum_par << endl;
    cout << "Average  : " << avg_par << endl;
    cout << "Time     : " << time_par << " seconds" << endl;

    // --- Speedup ---
    cout << "\nSpeedup (Sequential/Parallel) = " << (time_seq / time_par) << "x faster" << endl;

    return 0;
}
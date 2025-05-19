#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <numeric>
#include <iterator>
#include <fstream>
#include <chrono>

using namespace std;

vector<int> read_vector_from_file(const string& filename) {
    ifstream fin(filename);
    vector<int> v;
    int x;
    while (fin >> x)
        v.push_back(x);
    return v;
}

void write_to_file(const string& filename, const vector<int>& data) {
    ofstream fout(filename);
    for (int x : data) fout << x << " ";
    fout.close();
}

int main() {
    const string filename = "data_100000000.txt";

    vector<int> full_data = read_vector_from_file(filename);
    
    size_t mid = full_data.size() / 2;
    vector<int> A(full_data.begin(), full_data.begin() + mid);
    vector<int> B(full_data.begin() + mid, full_data.end());
    
    auto start_time = chrono::high_resolution_clock::now();
    sort(std::execution::par, A.begin(), A.end());
    sort(std::execution::par, B.begin(), B.end());

    vector<int> C(A.size() + B.size(), 0);

    const int blocks = 4;
    vector<pair<size_t, size_t>> ranges(blocks);
    for (int i = 0; i < blocks; ++i) {
        size_t a_start = i * A.size() / blocks;
        size_t a_end = (i + 1) * A.size() / blocks;
        if (a_start >= A.size()) a_start = A.size() - 1;
        if (a_end > A.size()) a_end = A.size();
        ranges[i] = { a_start, a_end };
    }

    std::for_each(std::execution::par, ranges.begin(), ranges.end(), [&](auto range) {
        size_t i_start = range.first;
        size_t i_end = range.second;
        if (i_start >= i_end) return;

        int a_val_1 = A[i_start];
        int a_val_2 = A[i_end - 1];
        size_t j_start = lower_bound(B.begin(), B.end(), a_val_1) - B.begin();
        size_t j_end = upper_bound(B.begin(), B.end(), a_val_2) - B.begin();

        vector<int> temp((i_end - i_start) + (j_end - j_start));
        merge(A.begin() + i_start, A.begin() + i_end,
            B.begin() + j_start, B.begin() + j_end,
            temp.begin());

        size_t c_offset = i_start + j_start;
        if (c_offset + temp.size() <= C.size()) {
            copy(temp.begin(), temp.end(), C.begin() + c_offset);
        }
        else if (c_offset < C.size()) {
            size_t max_copy = C.size() - c_offset;
            copy(temp.begin(), temp.begin() + max_copy, C.begin() + c_offset);
        }
        });

    auto end_time = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(end_time - start_time).count();

    write_to_file("data_100000000_sorted_parallel_stl.txt", C);
    cout << "Timp total de executie: " << elapsed << " secunde.\n";

    return 0;
}

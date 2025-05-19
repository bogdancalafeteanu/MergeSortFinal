#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>

using namespace std;

void loadArrayFromFile(const string& filename, int arr[], int size) {
    ifstream file(filename);
    if (!file) {
        cerr << "Eroare la deschiderea fisierului: " << filename << endl;
        return;
    }
    for (int i = 0; i < size; ++i) {
        file >> arr[i];
    }
    file.close();
}

void saveArrayToFile(const string& filename, int arr[], int size) {
    ofstream file(filename);
    if (!file) {
        cerr << "Eroare la crearea fisierului: " << filename << endl;
        return;
    }
    for (int i = 0; i < size; ++i) {
        file << arr[i] << " ";
    }
    file.close();
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int main() {
    int sizes[] = { 10000000,25000000,50000000,75000000,100000000 };
    string filenames[] = { "data_10000000.txt","data_25000000.txt" , "data_50000000.txt" , "data_75000000.txt","data_100000000.txt" };

    for (int f = 0; f < 5; f++) {
        int* arr = new int[sizes[f]];
        loadArrayFromFile(filenames[f], arr, sizes[f]);

        auto start = chrono::high_resolution_clock::now();
        mergeSort(arr, 0, sizes[f] - 1);
        auto stop = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = stop - start;

        cout << "Fisier: " << filenames[f] << " - Run time: " << fixed << duration.count() << " seconds" << endl;

        //string sortedFilename = filenames[f].substr(0, filenames[f].find_last_of(".")) + "_sorted.txt";
         //saveArrayToFile(sortedFilename, arr, sizes[f]);

        delete[] arr;
    }

    return 0;
}

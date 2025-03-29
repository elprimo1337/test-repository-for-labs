#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <locale> // Для setlocale

void bitwiseSortRecursive(std::vector<int>& arr, int bit) {
    if (bit < 0 || arr.size() <= 1) {
        return;
    }

    std::vector<int> temp = arr;
    int zeros_index = 0;
    int ones_index = arr.size() - 1;

    for (int i = 0; i < arr.size(); ++i) {
        if ((temp[i] & (1 << bit)) == 0) {
            arr[zeros_index++] = temp[i];
        }
        else {
            arr[ones_index--] = temp[i];
        }
    }

    // Разделяем массив на нули и единицы на основе текущего бита
    std::vector<int> zeros;
    std::vector<int> ones;
    for (int num : arr) {
        if ((num & (1 << bit)) == 0) {
            zeros.push_back(num);
        }
        else {
            ones.push_back(num);
        }
    }

    arr.clear();

    // Рекурсивно сортируем подмассив нулей и единиц, если они есть
    if (bit == 31) {
        bitwiseSortRecursive(ones, bit - 1);
        bitwiseSortRecursive(zeros, bit - 1);
        arr.insert(arr.end(), ones.begin(), ones.end());
        arr.insert(arr.end(), zeros.begin(), zeros.end());

    }
    else {
        bitwiseSortRecursive(zeros, bit - 1);
        bitwiseSortRecursive(ones, bit - 1);
        arr.insert(arr.end(), zeros.begin(), zeros.end());
        arr.insert(arr.end(), ones.begin(), ones.end());

    }
}

void bitwiseSort(std::vector<int>& arr) {
    bitwiseSortRecursive(arr, 31);
}

std::vector<int> readArrayFromFile(const std::string& filename) {
    std::vector<int> arr;
    std::ifstream file(filename);
    if (file.is_open()) {
        int val;
        while (file >> val) {
            arr.push_back(val);
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    return arr;
}

int main() {
    setlocale(LC_ALL, "ru"); // Или другой подходящий вариант
    std::string filename = "array_10000_-10_10.txt";
    std::vector<int> originalArray = readArrayFromFile(filename);

    if (originalArray.empty()) {
        std::cerr << "Error: Could not read array from file: " << filename << std::endl;
        return 1;
    }

    // Число прогонов для вычисления среднего времени
    const int numRuns = 3;
    double totalTime = 0.0;

    // Выполняем сортировку несколько раз и измеряем время
    for (int i = 0; i < numRuns; ++i) {
        // Создаем копию массива для сортировки (чтобы не менять исходный)
        std::vector<int> arrayToSort = originalArray;

        // Замеряем время
        auto start = std::chrono::high_resolution_clock::now();
        bitwiseSort(arrayToSort);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        totalTime += duration.count();
    }

    // Вычисляем среднее время
    double averageTime = totalTime / numRuns;

    std::cout << "Среднее время сортировки (рекурсивный вариант): "
        << std::fixed << std::setprecision(2) << averageTime << " мс" << std::endl;

    return 0;
}
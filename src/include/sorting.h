//
// Created by sphdx on 16.01.2025.
//

#ifndef SORTING_H
#define SORTING_H

#include <iostream>

template<typename T>
void printArray(const T* arr, const int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i];
    }
    std::cout << std::endl;
}

namespace sorting {
    enum class Order {
        ASC,
        DESC
    };

    template<typename T>
    void insertionSort(T* arr, const int arraySize, const Order order = Order::ASC) {
        for (int i = 1; i < arraySize; ++i) {
            const T key = arr[i];
            int j = i - 1;

            if (order == Order::ASC) {
                while (j >= 0 && arr[j] > key) {
                    arr[j + 1] = arr[j];
                    --j;
                }
            } else {
                while (j >= 0 && arr[j] < key) {
                    arr[j + 1] = arr[j];
                    --j;
                }
            }
            arr[j + 1] = key;
        }
    }

    // increment generates Sedjvick dist for shell sort
    // for inc[40] it works about 200 nanoseconds
    inline int increment(long inc[], const long size) {
        int p1 = 1, p2 = 1, p3 = 1;
        int s = -1;

        do {
            if (++s % 2) {
                inc[s] = 8 * p1 - 6 * p2 + 1;
            } else {
                inc[s] = 9 * p1 - 9 * p3 + 1;
                p2 *= 2;
                p3 *= 2;
            }
            p1 *= 2;
        } while (3 * inc[s] < size);

        return s > 0 ? --s : 0;
    }

    template<typename T>
    void shellSort(T* arr, const int arraySize, const Order order = Order::ASC) {
        long j;
        long seq[arraySize];

        int s = increment(seq, arraySize);

        while (s >= 0) {
            const long inc = seq[s--];

            for (long i = inc; i < arraySize; i++) {
                T temp = arr[i];
                if (order == Order::ASC) {
                    for (j = i - inc; j >= 0 && arr[j] > temp; j -= inc) {
                        arr[j + inc] = arr[j];
                    }
                } else {
                    for (j = i - inc; j >= 0 && arr[j] < temp; j -= inc) {
                        arr[j + inc] = arr[j];
                    }
                }
                arr[j + inc] = temp;
            }
        }
    }
}

#endif //SORTING_H

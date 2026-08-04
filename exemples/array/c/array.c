#include "array.h"

#include <stdio.h>
#include <stdlib.h>

#define INIT_CAPACITY 10

#define max(a,b) ((a > b) ? a : b)

array create_array(unsigned int size) {
    
    unsigned int capacity = max(INIT_CAPACITY, size);
    array l = {
        .data = malloc(l.capacity * sizeof(T)),
        .size = size,
        .capacity = capacity
    };
    return l;
}

array create_empty_array() {
    return create_array(0);
}

static void realloc_array(array* arr) {
    unsigned int new_capacity = 2 * arr->capacity;
    T* new_data = (T*) malloc(new_capacity * sizeof(T));
    for (unsigned int i = 0; i < arr->size; i++) {
        new_data[i] = arr->data[i];
    }
    free(arr->data);
    arr->data = new_data;
    arr->capacity = new_capacity;
}

// Shifts the elements of indices index_start .. size-1 one position to the right
// It is assumed that capacity >= size + 1
static void shift_right(array* arr, unsigned int index_start) {
    for (unsigned int i = arr->size; i > index_start; i--) {
        arr->data[i] = arr->data[i - 1];
    }
}

// Shifts the elements of indices index_start .. size-1 one position to the left
static void shift_left(array* arr, unsigned int index_start) {
    if (index_start == 0 || arr->size == 0)
        return;
    for (unsigned int i = index_start - 1; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }
}

void push_back(array* arr, T val) {

    if (arr->size >= arr->capacity)
        realloc_array(arr);

    arr->data[arr->size] = val;
    arr->size++;
}

T get(const array* arr, unsigned int index) {
    return arr->data[index];
}

void set(array* arr, unsigned int index, T val) {
    arr->data[index] = val;
}

void insert(array* arr, unsigned int index, T val) {

    if (index > arr->size)
        return;

    if (arr->size >= arr->capacity)
        realloc_array(arr);

    if (index == arr->size) {
        push_back(arr, val);
    }
    else {
        shift_right(arr, index);
        arr->data[index] = val;
        arr->size++;
    }
}

void delete_at(array* arr, unsigned int index) {
    shift_left(arr, index + 1);
    arr->size--;
}

void print_array(const array* arr) {

    printf("[ ");
    if (arr->size != 0) {
        for (unsigned int i = 0; i < arr->size - 1; i++) {
            printf("%d, ", arr->data[i]);
        }
        printf("%d ", arr->data[arr->size - 1]);
    }
    printf("]\n");
}

void destroy_array(array* arr) {
    free(arr->data);
    *arr = (array) { .data = NULL, .size = 0, .capacity = 0 };
}
#ifndef ARRAY_H
#define ARRAY_H

typedef int T;
typedef struct {
    T* data;
    unsigned int size;
    unsigned int capacity;
} array;

/////

array create_array(unsigned int size);

array create_empty_array();

T get(const array* list, unsigned int index);

void set(array* arr, unsigned int index, T val);

void push_back(array* arr, T val);

void insert(array* arr, unsigned int index, T val);

void delete_at(array* arr, unsigned int index);

void print_array(const array* arr);

void destroy_array(array* arr);

#endif
#ifndef LIST_H
#define LIST_H

typedef int T;

typedef struct cell {
    T value;
    struct cell* next;
} cell;

typedef struct {
    cell* head;
    int size;
} list;

/////

list create_empty_list();

T get(const list* l, int index);

void set(list* l, int index, T val);

void push_front(list* l, T val);

void push_back(list* l, T val);

void insert(list* l, int index, T val);

void delete_at(list* l, int index);

void print_list(const list* l);

void destroy_list(list* l);

list reverse(const list* l);

list map(T (*pf)(T), const list* l);

#endif
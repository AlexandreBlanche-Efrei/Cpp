#include <stdio.h>
#include <stdlib.h>

#include "list.h"

list create_empty_list() {
    return (list) { .head = NULL, .size = 0 };
}

static cell *create_cell(T val) {
    cell *newcell = (cell*) malloc(sizeof(cell));
    *newcell = (cell) { .value = val, .next = NULL };
    return newcell;
}

void push_front(list* l, T val) {
    
    cell* newcell = create_cell(val);
    newcell->next = l->head;
    l->head = newcell;
    l->size++;
}

static cell* get_cell(const list* l, int index) {
    cell* curr = l->head;
    if (curr == NULL || index < 0 || index > l->size)
        return NULL;

    int i = 0;
    while (!(i == index || curr == NULL)) {
        curr = curr->next;
        i++;
    }
    return curr;
}

static cell* get_last(const list* l) {
    return get_cell(l, l->size - 1);
}

void push_back(list* l, T val) {

    if (l->size == 0) {
        push_front(l, val);
        return;
    }
    cell* newcell = create_cell(val);
    cell* lastcell = get_last(l);
    lastcell->next = newcell;
    l->size++;
}

T get(const list* l, int index) {
    cell* cell = get_cell(l, index);
    return cell->value;
}

void set(list* l, int index, T val) {
    cell* cell = get_cell(l, index);
    cell->value = val;
}

void insert(list* l, int index, T val) {
    if (index == 0) {
        push_front(l, val);
        return;
    }

    cell* prev = get_cell(l, index - 1);
    cell* newcell = create_cell(val);
    newcell->next = prev->next;
    prev->next = newcell;
    l->size++;
}

void delete_at(list* l, int index) {
    if (index == 0) {
        cell* first = l->head;
        if (first == NULL)
            return;
        l->head = first->next;
        free(first);
        l->size--;
        return;
    }

    cell* prev = get_cell(l, index - 1);
    cell* cell = prev->next;
    if (prev == NULL || cell == NULL)
        return;
    prev->next = cell->next;
    free(cell);
    l->size--;
}

void print_list(const list* list) {
    
    printf("[ ");
    cell* curr = list->head;
    if (curr == NULL) {
        printf("]\n");
        return;
    }
    
    while(curr->next != NULL) {
        printf("%d, ", curr->value);
        curr = curr->next;
    }
    printf("%d ]\n", curr->value);
}

void destroy_list(list* l) {
    
    cell* curr = l->head;
    while (curr != NULL) {
        cell* next = curr->next;
        free(curr);
        curr = next;
    }
    l->head = NULL;
    l->size = 0;
}

list reverse(const list* l) {
    list revlist = create_empty_list();
    cell* curr = l->head;
    while (curr != NULL) {
        push_front(&revlist, curr->value);
        curr = curr->next;
    }
    return revlist;
}

list map(T (*pf)(T), const list* l) {
    list newlist = create_empty_list();
    
    cell* curr = l->head;
    while (curr != NULL) {
        push_front(&newlist, (*pf)(curr->value));
        curr = curr->next;
    }

    list revnew = reverse(&newlist);
    destroy_list(&newlist);
    return revnew;
}
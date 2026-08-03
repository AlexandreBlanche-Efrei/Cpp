#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int square(int x) {
    return x * x;
}

int main() {

    list l = create_empty_list();
    print_list(&l);

    push_back(&l, 5);
    push_back(&l, 2);
    print_list(&l);

    push_back(&l, -3);
    push_back(&l, 4);
    print_list(&l);

    delete_at(&l, 2);
    print_list(&l);

    insert(&l, 1, 0);
    print_list(&l);

    push_front(&l, 8);
    print_list(&l);

    set(&l, 3, 10);
    print_list(&l);

    const int index = 1;
    printf("list[%d] = %d\n", index, get(&l, index));
    print_list(&l);

    list rev = reverse(&l);
    print_list(&rev);

    destroy_list(&l);
    destroy_list(&rev);

    //////////////////////////////////

    list range = create_empty_list();
    for (int i = 10; i >= 0; i--)
        push_front(&range, i);
    print_list(&range);

    list squares = map(square, &range);
    print_list(&squares);
    
    destroy_list(&range);
    destroy_list(&squares);

    //////////////////////////////////

    // Danger :

    list ll = create_empty_list();
    push_back(&ll, 2);
    push_back(&ll, 3);

    // ll2 pointe sur les mêmes cellules que ll ("shallow copy")
    list ll2 = ll;

    printf("ll: ");
    print_list(&ll);

    printf("ll2: ");
    print_list(&ll2);

    destroy_list(&ll);

    printf("ll: ");
    print_list(&ll);

    // Cause une erreur de segmentation :
    // ll2.head est un "dangling pointer", qui pointe vers une zone mémoire désallouée

    // printf("ll2: ");
    // print_list(&ll2);

    return EXIT_SUCCESS;
}
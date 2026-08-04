#include <stdio.h>
#include <stdlib.h>

#include "array.h"

int main() {

    array arr = create_empty_array();
    print_array(&arr);

    push_back(&arr, 5);
    push_back(&arr, 2);
    print_array(&arr);

    push_back(&arr, -3);
    push_back(&arr, 4);
    print_array(&arr);

    delete_at(&arr, 2);
    print_array(&arr);

    insert(&arr, 1, 0);
    print_array(&arr);

    set(&arr, 3, 10);
    print_array(&arr);

    const int index = 1;
    printf("arr[%d] = %d\n", index, get(&arr, index));
    print_array(&arr);

    destroy_array(&arr);

    //////////////////////////////////

    // Danger :
    {
        int size = 10;
        array arr = create_array(size);
        for (int i = 0; i < size; i++) {
            set(&arr, i, i * i);
        }
        print_array(&arr);

        // arr sort du scope : fuite mémoire !
        
        // Il faut absolument penser à appeler le destructeur :
        // destroy_array(&arr);
    }

    return EXIT_SUCCESS;
}
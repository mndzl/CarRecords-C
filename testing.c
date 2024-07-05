#include <stdio.h>
#include <stdlib.h>

int main(){
    int* array;
    array = calloc(5, sizeof(int));

    array[2] = 12;

    printf("%i %i", array[0], array[2]);


    return 0;
}
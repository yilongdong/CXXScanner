#include "foo.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Hello();
    char *x = (char*)malloc(10 * sizeof(char*));
    free(x);
    int *array = new int[100];
    delete [] array;
    return array[argc];  // BOOM
}

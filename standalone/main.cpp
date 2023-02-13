#include "foo.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Hello();
    char *x = (char*)malloc(10 * sizeof(char*));
    free(x);
    return 0;
}

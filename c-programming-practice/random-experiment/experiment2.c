#include <stdio.h>

void loop_printing_function( void *ptr );

int main(int argc, char const *argv[])
{
    int a=3, b=5;
    loop_printing_function(&a);

    return 0;
}

void loop_printing_function( void *ptr )
{
    int num1, i, j;
    num1 = *((int *) ptr);
    // j = *num1;
    for (i = 0; i < 5; i++){
        printf("%d\n", num1++);
    }
}


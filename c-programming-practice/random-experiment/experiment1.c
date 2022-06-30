#include <stdio.h>

int main(int argc, char const *argv[])
{
    int val = 0;
    printf("This is %d\n", val);
    val = !val;
    printf("This is new %d\n", val);

    return 0;
}

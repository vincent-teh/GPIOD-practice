#include <stdio.h>

//declaring structure of students
struct students
{
    int id;
    int result;
};

void print_id_result_function( void *ptr);

int main(int argc, char const *argv[])
{
    //declaring a student variable name ali
    struct students ali = {1, 2};
    struct students *name1 = &ali;
    (void *) name1;
    print_id_result_function(name1);
    return 0;
}

//dereferancing void* to struct
void print_id_result_function( void *ptr ){
    struct students name;   //declare struct variable student name first
    name = *(struct students *) ptr;    //*(struct thread_struct*)variable;;the 1st * dereferancing the variable, (..) cast from void* to struct*
    printf("%d, %d\n", name.id, name.result);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *char_ptr;     // A char pointer
    int *int_ptr;       // An integer pointer
    int mem_size;

    if (argc < 2)       // If there aren't command-line arguments,
        mem_size = 50;  // use 50 as the default value for memory size.
    else
        mem_size = atoi(argv[1]);  // Convert the first command-line argument to an integer.

        
    // The following line is just for demonstration purposes
    printf("\t[*] allocating %d bytes of memory on the heap for char_ptr\n", mem_size);
    char_ptr = (char *) malloc(mem_size); // Allocating heap memory

    if (char_ptr == NULL) { // Error checking, in case malloc() fails
        fprintf(stderr, "Error: could not allocate heap memory.\n");
        exit(-1);
    }

    strcpy(char_ptr, "This memory is located on the heap.");
    printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr); // %s expect a char * (so a pointer)  | %p expect a pointer

    printf("\t[*] allocating 12 bytes of memory on the heap for int_ptr\n");
    int_ptr = (int *) malloc(12); // Allocated heap memory again

    if (int_ptr == NULL) { // Error checking, in case malloc() fails
        fprintf(stderr, "Error: could not allocate heap memory.\n");
        exit(-1);
    }

    *int_ptr = 31337; // Put the value of 31337 where int_ptr is pointing.
    printf("int_ptr (%p) --> %d\n", int_ptr, *int_ptr);  // %d don't expect a pointer -> * to dereference the pointer

    printf("\t[-] freeing char_ptr's heap memory...\n");
    free(char_ptr); // Freeing heap memory

    printf("\t[+] allocating another 15 bytes for char_ptr\n");
    char_ptr = (char *) malloc(15); // Allocating more heap memory

    if (char_ptr == NULL) { // Error checking, in case malloc() fails
        fprintf(stderr, "Error: could not allocate heap memory.\n");
        exit(-1);
    }

    strcpy(char_ptr, "new memory");
    printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr);

    printf("\t[-] freeing int_ptr's heap memory...\n");
    free(int_ptr); // Freeing heap memory

    printf("\t[-] freeing char_ptr's heap memory...\n");
    free(char_ptr); // Freeing the other block of heap memory
}


//To avoid repetition in the allocation part, we can use a function like this:
void *errorcheck_malloc(unsigned int size){
    void *ptr;
    ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Error: could not allocate heap memory.\n");
        exit(-1);
    }
    return ptr;
}
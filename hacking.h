/*

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A function to display an error message and exit the program
void fatal(const char *message) {
    char error_message[100];

    strcpy(error_message, "[!!] Fatal error ");
    strncat(error_message, message, 83);
    perror(error_message);  // Print the error message
    exit(-1);
}

// An error-checked malloc() wrapper function
void *ec_malloc(unsigned int size) {
    void *ptr;
    ptr = malloc(size);
    if (ptr == NULL) {
        fatal("in ec_malloc() on memory allocation");
    }
    return ptr;  // Return the allocated memory pointer
}

// Dumps raw memory in hex byte and printable split format
void dump(const unsigned char *data_buffer, const unsigned int length) {
    unsigned int i, j;
    unsigned char byte;

    for (i = 0; i < length; i++) {
        byte = data_buffer[i];                // Display byte in hex
        printf("%02X ", byte);

        if (((i % 16) == 15) || (i == length - 1)) {
            for (j = 0; j < 15 - (i % 16); j++)
                printf("   ");                // fill space if line < 16 bytes

            printf("  ");

            for (j = (i - (i % 16)); j <= i; j++) {
                byte = data_buffer[j];
                if (byte > 31 && byte < 127)   // printable ASCII range
                    printf("%c", byte);
                else
                    printf(".");
            }
            printf("\n");                     // new line every 16 bytes
        }
    }
}
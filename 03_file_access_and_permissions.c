/*
There are 2 primary ways to access files in C:
 1. File descriptor (low-level I/O, more direct)
 2. filestream (high-level I/O, easier to program with

A file descriptor is a number that uniquely identifies an open file, and it's used to reference 
it (with and integer value). "IT'S PASSED LIKE A POINTER TO THE FILE!!!"  

- - - - -
4 common function with files (returns -1 on error):
 1. open()  - Opens a file and returns a file descriptor.
 2. read()  - Reads data from a file using its file descriptor.
 3. write() - Writes data to a file using its file descriptor.
 4. close() - Closes a file using its file descriptor.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

void usage(char *prog_name, char *filename) {
    printf("Usage: %s <data to add to %s>\n", prog_name, filename);
    exit(0);
}

void fatal(char *);                         // A function for fatal errors
void *ec_malloc(unsigned int);              // An error-checked malloc() wrapper

int main(int argc, char *argv[]) {
    int fd;                                 // file descriptor
    char *buffer, *datafile;

    buffer = (char *) ec_malloc(100);
    datafile = (char *) ec_malloc(20);
    strcpy(datafile, "/tmp/notes");

    if (argc < 2)                           // If there aren't command-line arguments,
        usage(argv[0], datafile);           // display usage message and exit.

    strcpy(buffer, argv[1]);                // Copy into buffer.

    printf("[DEBUG] buffer   @ %p : '%s'\n", buffer, buffer);
    printf("[DEBUG] datafile @ %p : '%s'\n", datafile, datafile);

    strncat(buffer, "\n", 1);               // Add a newline on the end.

    // Opening file
    fd = open(datafile, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1)
        fatal("in main() while opening file");
    printf("[DEBUG] file descriptor is %d\n", fd);

    // Writing data
    if (write(fd, buffer, strlen(buffer)) == -1)
        fatal("in main() while writing buffer to file");

    // Closing file
    if (close(fd) == -1)
        fatal("in main() while closing file");

    printf("Note has been saved.\n");

    free(buffer);
    free(datafile);
    return 0;
}

// A function to display an error message and then exit
void fatal(char *message) {
    char error_message[100];

    strcpy(error_message, "[!!] Fatal Error ");
    strncat(error_message, message, 83);
    perror(error_message);
    exit(-1);
}

// An error-checked malloc() wrapper function
void *ec_malloc(unsigned int size) {
    void *ptr;
    ptr = malloc(size);
    if (ptr == NULL)
        fatal("in ec_malloc() on memory allocation");
    return ptr;
}

/*
• strlen() - Returns the length of a string (not including the null terminator).
• perror() - Prints a description for the last error that occurred.
• strncat() - Appends a string to another string, with a specified maximum length.

The files fcnt.h and sys/stat.h had to be included, since they define the flags used in the open() fn:
    • fcnt.h defines the file access modes (O_RDONLY, O_WRONLY, O_RDWR) and 
                     the file creation flags (O_CREAT, O_EXCL, O_TRUNC, O_APPEND), combined with '|' the or operator.

    • sys/stat.h defines the file permission bits (S_IRUSR, S_IWUSR, S_IRGRP, S_IWGRP, S_IROTH, S_IWOTH)
        (I - followed by the permission bits, R for read, W for write, X for execute - followed by the user, group, and other(anyone) permissions).
        (I - R(Read) - USR (User) -> IRUSR)

    FLAGS ARE TYPE: ---->  INT  <----

Curiosity:
  chmod command structure (rwx are bitwise, ex: 7= 2^2 + 2^1 + 2^0 (=rwx) , 5= 2^2 + 2^0 (=rx)):
    "chmod [who][operation][permission] filename"
            rwx     rwx        rwx
            |||     |||        |||
            |||     |||        ||+--> others
            |||     ||+-----> group
            ||+-------> user
        
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
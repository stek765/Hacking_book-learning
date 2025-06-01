/*
Buffer Overflow vulnerabilities have been around since the dawn of programming.

They are a type of security vulnerability that occurs when a program writes more data to a buffer (ex: stack),
than it can hold, causing the program to overwrite adjacent memory locations.

This can lead to unexpected behavior, crashes, or even allow an attacker to execute arbitrary code.
- - - - - - - - - - - - - - - - - - 
*/

// This program is designed to demonstrate a simple stack-based buffer overflow by showing how overflowing 
// one buffer (buffer_two) can:
// 	 1.	Corrupt another buffer (buffer_one) that is adjacent in memory.
//   2.	Modify a separate variable (value) that lies further up in the stack frame.
// - - - - - - - - - - - - -

// compile with: gcc -fno-stack-protector -o file file.c
// run: ./file input_string

#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    int value = 5;
    char buffer_one[8], buffer_two[8]; // Two buffers of 8 bytes each

    strcpy(buffer_one, "one"); // Copying string "one" into buffer_one
    strcpy(buffer_two, "two"); // Copying string "two" into buffer_two

    // using two before one because of the stack frame (since MAIN is a function so it has a stack frame)
    printf("[BEFORE] buffer_two is at %p and contains \'%s\'\n", buffer_two, buffer_two);
    printf("[BEFORE] buffer_one is at %p and contains \'%s\'\n", buffer_one, buffer_one);
    printf("[BEFORE] value is at %p and is \'%d\'\n", &value, value);


    printf("\n[STRCPY] copying %lu bytes into buffer_two\n\n", strlen(argv[1]));
    strcpy(buffer_two, argv[1]); // Copying the command line first argument into buffer_two

    printf("[AFTER] buffer_two is at %p and contains \'%s\'\n", buffer_two, buffer_two);
    printf("[AFTER] buffer_one is at %p and contains \'%s\'\n", buffer_one, buffer_one);
    printf("[AFTER] value is at %p and is \'%d\'\n", &value, value);

    return 0;
}

/* 
if you run this program with a string longer than 8 bytes, it will overwrite the value variable.
using a string longer than 8 bytes (e.g., "AAAAAAAAAAAAAAAA") will cause a buffer overflow.
After the strcpy(buffer_two, argv[1]), the memory layout will look like this:

| buffer_two[0-7] = 'AAAAAAA\0'     |
| buffer_one[0-7] = 'AAAAAAAA'      | ← ⚠️ Corrupted!
| value (int) = 0x41414141 ('AAAA') | ← ⚠️ Corrupted!
| return address = ???              | ← ⚠️ Possible crash!
*/
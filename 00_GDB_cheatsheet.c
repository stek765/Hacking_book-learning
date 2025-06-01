
/*
  GDB CHEATSHEET — Low-level Debugging Reference
 
  This file is meant to be a quick practical reference for:
  - Stack inspection
  - Register access
  - Memory inspection
  - Breakpoints and flow control
  - Variable and function analysis
 
  Use this when analyzing C programs, vulnerabilities, or during RE/CTFs.
 
  */

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Example data to inspect
    int  number = 1337;
    char buffer[16] = "HELLO";

    printf("Program started. PID: %d\n", getpid());

    // Pause here to attach debugger manually if needed
    getchar();

    printf("buffer: %s\n", buffer);
    printf("number: %d\n", number);

    return 0;
}

/* --------------------------------------------
   💻 GDB STARTUP
   --------------------------------------------
   gcc -g -o prog gdb_cheatsheet.c
   gdb ./prog
   run
   --------------------------------------------

   🛑 BREAKPOINTS
   --------------------------------------------
   break main                   # breakpoint at main
   break 12                     # breakpoint at line 12
   break func_name              # breakpoint at a function
   delete                       # delete all breakpoints
   --------------------------------------------

   🧭 FLOW CONTROL
   --------------------------------------------
   run ARG                      # start program with args
   next                         # next line (skip into funcs)
   step                         # step into function
   continue                     # continue after breakpoint
   finish                       # run until function returns
   kill                         # terminate program
   --------------------------------------------

   📦 VARIABLES & REGISTERS
   --------------------------------------------
   print var                    # print variable value
   set var = VALUE              # set variable
   info locals                  # show local variables
   info args                    # show function args
   info registers               # show CPU registers
   --------------------------------------------

   📚 STACK & MEMORY
   --------------------------------------------
   x/NFMT ADDR                  # examine memory
     N = number of units
     FMT = format (x=hex, d=dec, s=string, i=instruction, c=char, w=4byte)
     ADDR = memory address or $esp, &var, etc.

   Examples:
     x/16xw $esp                # 16 words from ESP
     x/32xb &buffer             # 32 bytes from buffer
     x/s &buffer                # show buffer as string
     x/4i $eip                  # show next 4 instructions

   --------------------------------------------

   📁 FUNCTIONS & SYMBOLS
   --------------------------------------------
   info functions               # list all functions
   info address var             # show address of symbol
   disas main                   # disassemble function
   layout asm                   # show assembly in TUI
   --------------------------------------------

   🧠 ADVANCED
   --------------------------------------------
   info frame                   # current stack frame
   backtrace                    # call stack (bt)
   set {int}&var = VALUE        # modify memory at address
   display var                  # always print var on step
   --------------------------------------------

   📌 WATCHPOINTS (break on memory write/read)
   --------------------------------------------
   watch var                   # break on write
   rwatch var                  # break on read
   awatch var                  # break on read or write
   --------------------------------------------
*/
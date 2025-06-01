/* A compiled program's memory is divided into 5 segments:

1. TEXT   
2. DATA  // global/static initialized variables
3. BSS   // global/static uninitialized variables
4. HEAP  
5. STACK  // (main is also a function, so it has a stack frame too)

the processor follows this loop:
   - Reads the instruction that EIP register points to.
   - Increments the EIP register to point to the next instruction.
   - Executes the instruction.
   - Repeats



- - - 
TEXT:
- - - 
The Text Segment contains the compiled code of the program. It is read-only and 
cannot be modified during runtime to prevent accidental/intentional changes to the code.
Any attempt to modify the text segment will result in a segmentation fault.

Another usefull thing is that this segment is shared between processes.
It should also be noted that it has a fixed size, which is determined at compile time.
---------------------------------------------------------------------------------------

- - - - - -
DATA & BSS:
- - - - - -

The Data and BSS segments are used to store global and static program variables.
- The Data segment contains initialized global and static variables.
- The BSS segment contains uninitialized global and static variables.

So the global and static variables persist despite the functional context since they are 
stored in one of these segments.
---------------------------------------------------------------------------------------

- - - - - -
HEAP:
- - - - - -

The Heap segment is used for dynamic memory allocation. 
It is read-write and can be modified during runtime.

It's not FIXED SIZE, it can grow and shrink as needed. (growth downwards towards the stack)
It's managed by alloc() and free() functions. (allocate memory, free it when done)
---------------------------------------------------------------------------------------

- - - - - -
STACK:
- - - - - -

The Stack segment is used for function calls and local variables.
It is read-write and can be modified during runtime.

The stack is a LIFO (Last In First Out) data structure, meaning that the last item added to the stack
is the first one to be removed.

The stack grows downwards towards the heap, and its size is limited by the operating system, 
even though it can grow and shrink as needed.

When a function is calle, all the passed parameters, the location EIP should return to afterwards, 
and all the local variable used in the function are stored in the stack, making it a "stack frame".

Inside the stack, there can be many stack frames, one for each function call.
When a function returns, its stack frame is removed from the stack, and the EIP register is set to the
location of the next instruction to execute.
---------------------------------------------------------------------------------------

*/

#include <stdio.h>

int global_var;    // BSS segment: uninitialized global variable
int global_inizialized_var = 5;  // DATA segment: initialized global variable

void function() {    // This is just a demo function
    int stack_var;   // Notice this variable has the same name as the one in main()
    
    printf("the function's stack_var is at address: 0x%08x\n", &stack_var);
}


int main(){
    int stack_var;   // Same name as the variable in function()
    static int static_initialized_var = 5;   // DATA segment: initialized static variable
    static int static_var;                   // BSS segment: uninitialized static variable
    int *heap_var_ptr;                       // HEAP segment: pointer to an integer

    heap_var_ptr = (int *) malloc(4);        // Allocate 4 bytes of memory on the heap

    // These variables are int he DATA segment (global_initialized_var  ,  static_initialized_var):
    printf("global_initialized_var is at address 0x%08x\n", &global_inizialized_var);
    printf("static_initialized_var is at address 0x%08x\n\n", &static_initialized_var);


    // These variables are in the BSS segment (global_var  ,  static_var):
    printf("global_var is at address 0x%08x\n", &global_var);
    printf("static_var is at address 0x%08x\n\n", &static_var);

    // This variable is in the HEAP segment  (allocated with malloc):  
    printf("heap_var is at address 0x%08x\n", heap_var_ptr);

    // These variables are in the STACK segment (stack_var):
    printf("stack_var is at address 0x%08x\n", &stack_var);
    function();

    free(heap_var_ptr);           // Free the allocated memory
    return 0;
}


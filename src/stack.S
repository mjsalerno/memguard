       .global _start

        .text
_start:
        pushq %rcx                      # Add 8 bytes to the stack
        popq %rcx                       # Remove 8 bytes to the stack
        sub $8, %rsp                    # Add 8 bytes to the stack
        add $8, %rsp                    # Add 8 bytes to the stack


        # exit(0)
        mov     $60, %rax               # system call 60 is exit
        xor     %rdi, %rdi              # we want return code 0
        syscall                         # invoke operating system to exit
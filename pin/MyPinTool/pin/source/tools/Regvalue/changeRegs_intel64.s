
.data

.extern gprval
.extern agprval
.extern stval
.extern astval
.extern xmmval
.extern axmmval
#ifdef CONTEXT_USING_AVX
.extern ymmval
.extern aymmval
#endif
.extern fpSaveArea

.text

# void ChangeRegsWrapper();
# This wrapper saves and restores the registers used by ChangeRegs.
# This is done in the wrapper since we want ChangeRegs to actually
# change the register values but not to affect the application itself.
# The tool may intercept ChangeRegs and replace it with its own function.
#
# Register usage:
# rax   - used (implicitly) by xsave
# rbx   - used for testing the gpr values
# rcx   - used for holding a pointer to the fp save area (used by fxsave)
# rdx   - used (implicitly) by xsave
# st0   - used (implicitly) for loading a value to the FPU stack
# st2   - used for testing the FPU values
# xmm0  - used for testing the sse values
# ymm1  - used for testing the avx values
.global ChangeRegsWrapper
.type ChangeRegsWrapper,  @function
ChangeRegsWrapper:
    # Save the necessary GPRs
    push    %rax
    push    %rbx
    push    %rcx
    push    %rdx

    # Allign the fpSaveArea
    lea     fpSaveArea, %rcx
    add     $0x40, %rcx
    and     $0xffffffffffffffc0, %rcx
    # Save the floating-point state
#ifdef CONTEXT_USING_AVX
    push    %rdx
    xor     %rdx, %rdx
    mov     $7, %rax
    xsave   (%rcx)
#else
    fxsave  (%rcx)
#endif

    # Now call ChangeRegs - do the actual test.
    # The tool may intercept this function and modify the register values itself.
    call    ChangeRegs

    # Placeholder for PIN_ExecuteAt
    call    ExecuteAt

    # Save the modified values to memory so the tool can ispect them.
    # This is relevant only when the tool modifies the values.
    call    SaveRegsToMem

    # Restore the floating-point state
#ifdef CONTEXT_USING_AVX
    mov     $7, %rax
    xrstor  (%rcx)
    pop     %rdx
#else
    fxrstor (%rcx)
#endif

    # Restore the GPRs
    pop     %rdx
    pop     %rcx
    pop     %rbx
    pop     %rax
    ret

# void ChangeRegs();
# For register usage see ChangeRegsWrapper above.
.global ChangeRegs
.type ChangeRegs,  @function
ChangeRegs:
    # TEST: load the new value to rbx
    mov     gprval, %rbx
    # prepare the test value at the top of the FPU stack
    fldt    stval
    # TEST: load the new value to st2
    fst     %st(2)
    # TEST: load the new value to xmm0
    movdqu  xmmval, %xmm0
#ifdef CONTEXT_USING_AVX
    # TEST: load the new value to ymm1
    vmovdqu ymmval, %ymm1
#endif
    ret

# void ExecuteAt();
.global ExecuteAt
.type ExecuteAt,  @function
ExecuteAt:
    ret

# void SaveRegsToMem();
# Save the necessary registers to memory.
# The tool will then compare the value stored in memory to the ones it expects to find.
# For register usage see ChangeRegsWrapper above.
.global SaveRegsToMem
.type SaveRegsToMem,  @function
SaveRegsToMem:
    # TEST: store the new value of rbx
    mov     %rbx, agprval
    # prepare the test value at the top of the FPU stack
    fld     %st(2)
    # TEST: store the new value of st2
    fstpt   astval
    # TEST: store the new value of xmm0
    movdqu  %xmm0, axmmval
#ifdef CONTEXT_USING_AVX
    # TEST: store the new value of ymm1
    vmovdqu %ymm1, aymmval
#endif
    ret

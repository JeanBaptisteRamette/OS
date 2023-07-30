bits 64


; Some interrupts push an error code to the stack
; We have to define two prologues before jumping to the common stub,
; one which does not push anything as it is done by the processor,
; and one which pushes a dummy error code 0

%macro ISR_ENTRY 1
InterruptServiceRoutineEntry%+%1:
    push qword 0
    push qword %1
    jmp IsrCommonStub
%endmacro


%macro ISR_ENTRY_EC 1
InterruptServiceRoutineEntry%+%1:
    push qword %1
    jmp IsrCommonStub
%endmacro

%macro PUSH_INT_CONTEXT 0
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rbp
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    push rax
%endmacro

%macro POP_INT_CONTEXT 0
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rbp
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
%endmacro


section .text

extern InterruptServiceRoutineDispatcher

IsrCommonStub:
    ; We do not need to clear IF, because interrupt gates do it automatically

    ; The SysV-x86_64 ABI requires cld to be cleared
    ; on function entry and function return.
    ;
    ; Note that if you disassemble some of your code, GCC doesn't necessarily generate
    ; cld instruction on function entry, this is because GCC can keep track
    ; of wether the DF is set or not when generating code. Here we explicitly clear
    ; the flag because we can't know for sure it it's set or not during an interrupt
    cld

    ; Push all registers so we can get their values contiguously via RSP
    PUSH_INT_CONTEXT

    ; Call the interrupt dispatcher function with and pass the registers state as argument
    mov rdi, rsp
    call InterruptServiceRoutineDispatcher

    POP_INT_CONTEXT

    ; pop interrupt vector and error code
    add rsp, 16

    ; return from interrupt, reset IF and DF to their original values
    ; as this instruction restores RFLAGS
    iretq


; Declare ISR entries for interrupts providing an error code
; Check "Table 6-1. Protected-Mode Exceptions and Interrupts (Contd.)" of Intel SDM
ISR_ENTRY_EC 8
ISR_ENTRY_EC 10
ISR_ENTRY_EC 11
ISR_ENTRY_EC 12
ISR_ENTRY_EC 13
ISR_ENTRY_EC 14
ISR_ENTRY_EC 17
ISR_ENTRY_EC 21

; Declare ISR entries for interrupts not providing an error code
ISR_ENTRY 0
ISR_ENTRY 1
ISR_ENTRY 2
ISR_ENTRY 3
ISR_ENTRY 4
ISR_ENTRY 5
ISR_ENTRY 6
ISR_ENTRY 7
ISR_ENTRY 9
ISR_ENTRY 15
ISR_ENTRY 16
ISR_ENTRY 18
ISR_ENTRY 19
ISR_ENTRY 20
ISR_ENTRY 22
ISR_ENTRY 23
ISR_ENTRY 24
ISR_ENTRY 25
ISR_ENTRY 26
ISR_ENTRY 27
ISR_ENTRY 28
ISR_ENTRY 29
ISR_ENTRY 30
ISR_ENTRY 31


section .data

global InterruptsHandlers
InterruptsHandlers:
%assign i 0
%rep 32
    dq InterruptServiceRoutineEntry%+i
%assign i i+1
%endrep

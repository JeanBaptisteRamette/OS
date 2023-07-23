bits 64

; https://wiki.osdev.org/System_V_ABI#x86-64
;
; Parameters to functions are passed in the registers rdi, rsi, rdx, rcx, r8, r9, and
; further values are passed on the stack in reverse order.
;
; void KeGdtInstallStub(
;                       GDT_REGISTER* pTableRegister<rdi>,
;                       WORD KernelCodeSegment<rsi>,
;                       WORD KernelDataSegment<rdx>
;      )

global KeGdtInstallStub
KeGdtInstallStub:
    ; Loads the GDT via the GDT_REGISTER struct
    lgdt [rdi]

    ; Update data segment selectors to
    ; index the new descriptors
    mov ds, dx
    mov ss, dx
    mov es, dx
    mov fs, dx
    mov gs, dx

    ; Update code segment selector, we need to issue a far return:
    ; "A return to a calling procedure located in a different segment
    ; than the current code segment"

    ; Use this syntax instead of mov rax, qword .trampoline
    ; because the later is not position independent
    lea rax, [rel .trampoline]

    ; Code segment selector and return address popped from the stack by retfq
    push rsi
    push rax

    ; When executing a far return, the processor pops the return
    ; instruction pointer from the top of the stack into the RIP register,
    ; then pops the segment selector from the top of the stack into the CS register.
    ; The processor then begins program execution in the new code segment at the new instruction pointer.
    retfq

.trampoline:
    ; Return to caller of KeGdtInstallStub
    ret

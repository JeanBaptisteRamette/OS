#ifndef OS_ISR_HPP
#define OS_ISR_HPP

#include <osl/string_view.hpp>
#include <osl/printf.hpp>
#include <osl/mindef.hpp>
#include <osl/array.hpp>


constexpr BYTE CPU_EXCEPTION_COUNT = 32;


//
// Table 6-1: Protected-Mode Exceptions and Interrupts
//
constexpr osl::array<osl::string_view, CPU_EXCEPTION_COUNT> ExceptionsDescriptions = {
        "Divide Error",
        "Debug Exception",
        "NMI Interrupt",
        "Breakpoint",
        "Overflow",
        "BOUND Range Exceeded",
        "Invalid Opcode",
        "Device Not Available",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Invalid TSS",
        "Segment Not Present",
        "Stack-Segment-Fault",
        "General Protection",
        "Page Fault",
        "Intel Reserved (15)",
        "x87 FPU Floating-Point Error",
        "Alignment Check",
        "Machine Check",
        "SIMD Floating-Point Exception",
        "Virtualization Exception",
        "Control Protection Exception",
        "Intel Reserved (22)",
        "Intel Reserved (23)",
        "Intel Reserved (24)",
        "Intel Reserved (25)",
        "Intel Reserved (26)",
        "Intel Reserved (27)",
        "Intel Reserved (28)",
        "Intel Reserved (29)",
        "Intel Reserved (30)",
        "Intel Reserved (31)",
};


struct INTERRUPT_CONTEXT
{
    QWORD rax;
    QWORD rbx;
    QWORD rcx;
    QWORD rdx;
    QWORD rsi;
    QWORD rdi;
    QWORD rbp;
    QWORD r8;
    QWORD r9;
    QWORD r10;
    QWORD r11;
    QWORD r12;
    QWORD r13;
    QWORD r14;
    QWORD r15;
    QWORD InterruptVector;
    QWORD InterruptErrCode;
};


//
// Defined in isr_stub.asm
//
extern "C" QWORD InterruptsHandlers[];


extern "C"
void InterruptServiceRoutineDispatcher(const INTERRUPT_CONTEXT* InterruptContext);


#endif //OS_ISR_HPP

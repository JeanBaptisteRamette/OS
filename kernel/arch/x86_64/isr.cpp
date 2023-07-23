#include <arch/x86_64/isr.hpp>
#include <std/printf.hpp>


bool InterruptIsCpuException(QWORD InterruptVector)
{
    return InterruptVector < 32;
}

void DbgLogCpuException(const INTERRUPT_CONTEXT* Exception)
{
    if (!Exception)
        return;

    const auto ExceptionVector      = Exception->InterruptVector;
    const auto ExceptionErrCode     = Exception->InterruptErrCode;
    const auto ExceptionDescription = ExceptionsDescriptions[ExceptionVector].data();

    printf("Received CPU Exception: Vector (%X)\n", ExceptionVector);
    printf("Exception Description:  %s\n", ExceptionDescription);

    const auto& c = *Exception;

    printf(
           "Exception Context Dump:\n"
           "RAX %016X RBX %016X RCX %016X RDX %016X\n"
           "RSI %016X RDI %016X RBP %016X R8  %016X\n"
           "R9  %016X R10 %016X R11 %016X R12 %016X\n"
           "R13 %016X R14 %016X R15 %016X ERR %016X\n",
           c.rax, c.rbx, c.rcx, c.rdx,
           c.rsi, c.rdi, c.rbp, c.r8,
           c.r9,  c.r10, c.r11, c.r12,
           c.r13, c.r14, c.r15, ExceptionErrCode
    );
}

extern "C"
void InterruptServiceRoutineDispatcher(const INTERRUPT_CONTEXT* InterruptContext)
{
    if (!InterruptContext)
        return;

    if (InterruptIsCpuException(InterruptContext->InterruptVector))
        DbgLogCpuException(InterruptContext);
    else
        printf("ISR Dispatcher (%d)\n", InterruptContext->InterruptVector);
}

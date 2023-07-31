#include <arch/x86_64/isr.hpp>
#include <osl/printf.hpp>


#define IRQ(IntNo) (IntNo - CPU_EXCEPTION_COUNT)

bool InterruptIsCpuException(QWORD InterruptVector)
{
    return InterruptVector < CPU_EXCEPTION_COUNT;
}

void DbgLogCpuException(const INTERRUPT_CONTEXT* Exception)
{
    if (!Exception)
        return;

    const auto ExceptionVector      = Exception->InterruptVector;
    const auto ExceptionErrCode     = Exception->InterruptErrCode;
    const auto ExceptionDescription = ExceptionsDescriptions[ExceptionVector].data();

    printf("Received CPU Exception: Vector (%lX)\n", ExceptionVector);
    printf("Exception Description:  %s\n", ExceptionDescription);

    const auto& c = *Exception;

    printf(
           "Exception Context Dump:\n"
           "RAX %016lX RBX %016lX RCX %016lX RDX %016lX\n"
           "RSI %016lX RDI %016lX RBP %016lX R8  %016lX\n"
           "R9  %016lX R10 %016lX R11 %016lX R12 %016lX\n"
           "R13 %016lX R14 %016lX R15 %016lX ERR %016lX\n",
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
        printf("IRQ Received (%lu) -> (%lu)\n", InterruptContext->InterruptVector, IRQ(InterruptContext->InterruptVector));
}

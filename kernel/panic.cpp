#include <arch/x86_64/cpu.hpp>
#include <osl/printf.hpp>
#include <panic.hpp>


[[noreturn]]
void KePanic(osl::string_view cause)
{
    printf("KePanic(): %s\n", cause.data());
    printf("-- ABORTING --\n");

    KeClearInterrupts();
    KeHaltForever();
}
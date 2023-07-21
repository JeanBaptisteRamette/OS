#ifndef OS_CPU_HPP
#define OS_CPU_HPP


void KeClearInterrupts();
void KeSetInterrupts();
bool KeInterruptsEnabled();

[[noreturn]]
void KeHalt();

#endif //OS_CPU_HPP

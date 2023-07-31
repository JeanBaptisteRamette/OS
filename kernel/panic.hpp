#ifndef OS_PANIC_HPP
#define OS_PANIC_HPP


#include <osl/string_view.hpp>


[[noreturn]]
void KePanic(osl::string_view cause);


#endif //OS_PANIC_HPP

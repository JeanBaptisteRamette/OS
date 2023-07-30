#ifndef OS_PMM_HPP
#define OS_PMM_HPP

#include <std/mindef.hpp>
#include <limine.h>

void KePmmInit();
QWORD KePmmRequest(QWORD Size);

#endif //OS_PMM_HPP

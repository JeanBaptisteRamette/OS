#ifndef OS_PMM_HPP
#define OS_PMM_HPP

#include <osl/mindef.hpp>
#include <limine.h>

void KePmmInit();
QWORD KePmmRequest(QWORD Size);
void KePmmRelease(QWORD Address, QWORD PageCount);

#endif //OS_PMM_HPP

#ifndef OS_PIC_HPP
#define OS_PIC_HPP


#include <osl/mindef.hpp>


// The x86_64 architecture has a built-in PCI chip, handling IRQs from external hardware devices
// http://www.brokenthorn.com/Resources/OSDevPic.html
//
// Because it is a legacy and obsolete controller compared to the modern APIC (Advanced PIC) we want to disable it.
// We still need to remap the IRQ vectors because it overlaps the processor exceptions vectors.

void KePicRemapIRQs();
void KePicDisable();


#endif //OS_PIC_HPP

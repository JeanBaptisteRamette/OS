#include <arch/x86_64/cpu.hpp>
#include <arch/x86_64/gdt.hpp>
#include <std/array.hpp>



#define LONGMODE     (1 << 9)
#define PRESENT      (1 << 7)
#define DPL0         (0)
#define CODE         (1 << 4)
#define DATA         (1 << 4)
#define EXECUTE_READ (0b1010)
#define READ_WRITE   (0b0010)


//
// Defined in gdt_stub.asm
//
extern "C" void KeGdtInstallStub(
        const GDT_REGISTER* pTableRegister,
        WORD KernelCodeSelector,
        WORD KernelDataSelector
    );


static osl::array<SEGMENT_DESCRIPTOR, 3> Descriptors {};


SEGMENT_DESCRIPTOR GdtEncodeDescriptor(DWORD Base, DWORD Limit, WORD Flags)
{
    //
    // LOBYTE(Flags) = T, S, DPL, P
    // HIBYTE(Flags) = AVL, L, D/B, G  (4 msb unused)
    //

    SEGMENT_DESCRIPTOR desc {};

    desc.LoLimit    = LOWORD(Limit);             // & 0x0000FFFF
    desc.LoBaseAddr = LOWORD(Base);              // & 0x0000FFFF
    desc.MiBaseAddr = LOBYTE(HIWORD(Base));      // & 0x00FF0000
    desc.HiBaseAddr = HIBYTE(HIWORD(Base));      // & 0xFF000000
    desc.AccessBits = LOBYTE(Flags);             // & 0x00FF
    desc.HiLimitAndFlags = static_cast<BYTE>((HIBYTE(Flags) << 4) | (LOBYTE(HIWORD(Limit)) & 0x0F));

    return desc;
}


void KeGdtSetup()
{
    // In long mode, the CPU ignores the entries' base and limit as it
    // uses a flat memory-model but it still uses the access flags

    //
    // The first segment descriptor is not used by the processor and should be null
    //
    Descriptors[SEG_NULL] = GdtEncodeDescriptor(0, 0, 0);


    //
    // Defines a 64-bit code segment for the kernel
    //
    Descriptors[SEG_K64C] = GdtEncodeDescriptor(
            0x00'00'00'00,      // Ignored base
            0x00'00'00'00,      // Ignored limit
            PRESENT | DPL0 | CODE | EXECUTE_READ | LONGMODE
    );

    //
    // Defines a 64-bit data segment for the kernel
    //
    Descriptors[SEG_K64D] = GdtEncodeDescriptor(
            0x00'00'00'00,      // Ignored base
            0x00'00'00'00,      // Ignored limit
            PRESENT | DPL0 | DATA | READ_WRITE
    );
}


void KeGdtLoad()
{
    //
    // Create the default GDT
    //
    KeGdtSetup();


    //
    // Create a GDT register
    //
    const auto Limit   = Descriptors.size() * sizeof(SEGMENT_DESCRIPTOR) - 1;
    const auto Address = (QWORD)Descriptors.data();

    const GDT_REGISTER GdtRegister = {
        Limit,
        Address
    };

    //
    // Call the assembly stub responsible for loading the GDT and updating segment selectors
    //
    KeGdtInstallStub(&GdtRegister, SELECTOR_KCS, SELECTOR_KDS);
}






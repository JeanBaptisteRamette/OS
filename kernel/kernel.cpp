#include <cstddef>
#include "limine.h"


static volatile limine_framebuffer_request framebuffer_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 0,
};



[[noreturn]]
static void KeHalt()
{
    //
    // hlt instructions halts until next interrupts so clear interrupts
    //
    asm ("cli");

    while (true)
        asm ("hlt");
}

extern "C"
void KernelEntry()
{
    //
    // Kernel Entry Point
    //

    if (framebuffer_request.response == nullptr || framebuffer_request.response->framebuffer_count < 1)
        KeHalt();

    limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    if (!framebuffer)
        KeHalt();

    auto FrameHeight = framebuffer->height;
    auto FrameWidth  = framebuffer->width;
    auto FrameStart  = reinterpret_cast<uint32_t*>(framebuffer->address);

    for (size_t x = 0; x < FrameWidth; ++x)
    {
        for (size_t y = 0; y < FrameHeight; ++y)
        {
            if ((x + y) & 1)
                FrameStart[x * (framebuffer->pitch / 4) + y] = 0xFFFFFF;
            else
                FrameStart[x * (framebuffer->pitch / 4) + y] = 0x000000;
        }
    }

    KeHalt();
}


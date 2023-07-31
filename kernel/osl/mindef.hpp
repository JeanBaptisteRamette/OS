#ifndef OS_MINDEF_HPP
#define OS_MINDEF_HPP


#include <cstdint>


enum KSTATUS
{
    KFAIL    = 0,
    KSUCCESS = 1
};


typedef       void* LPVOID;
typedef const void* LPCVOID;
typedef uint8_t  BYTE,  *PBYTE,  *LPBYTE;
typedef uint16_t WORD,  *PWORD,  *LPWORD;
typedef uint32_t DWORD, *PDWORD, *LPDWORD;
typedef uint64_t QWORD, *PQWORD, *LPQWORD;

typedef       char* PSTR;
typedef const char* PCSTR;


#define LOBYTE(w) ((BYTE)((QWORD)(w) & 0xFF))
#define HIBYTE(w) ((BYTE)((QWORD)(w) >> 8))

#define LOWORD(l) ((WORD)((QWORD)(l) & 0xFFFF))
#define HIWORD(l) ((WORD)((QWORD)(l) >> 16))

#define LODWORD(l) ((DWORD)((QWORD)(l) & 0xFFFFFFFF))
#define HIDWORD(l) ((DWORD)((QWORD)(l) >> 32))

#define BYTE1(l) (((QWORD)(l) & 0x00000000000000FF) >> 0)
#define BYTE2(l) (((QWORD)(l) & 0x000000000000FF00) >> 8)
#define BYTE3(l) (((QWORD)(l) & 0x0000000000FF0000) >> 16)
#define BYTE4(l) (((QWORD)(l) & 0x00000000FF000000) >> 24)
#define BYTE5(l) (((QWORD)(l) & 0x000000FF00000000) >> 32)
#define BYTE6(l) (((QWORD)(l) & 0x0000FF0000000000) >> 40)
#define BYTE7(l) (((QWORD)(l) & 0x00FF000000000000) >> 48)
#define BYTE8(l) (((QWORD)(l) & 0xFF00000000000000) >> 56)


#define GiB(Bytes) ((Bytes) / 0x40000000)
#define MiB(Bytes) ((Bytes) / 0x100000)
#define KiB(Bytes) ((Bytes) / 0x400)

#define PACKED __attribute__((packed))


#endif //OS_MINDEF_HPP

#ifndef OS_MINDEF_HPP
#define OS_MINDEF_HPP


#include <cstdint>


typedef       void* LPVOID;
typedef const void* LPCVOID;
typedef uint8_t  BYTE,  *PBYTE,  *LPBYTE;
typedef uint16_t WORD,  *PWORD,  *LPWORD;
typedef uint32_t DWORD, *PDWORD, *LPDWORD;
typedef uint64_t QWORD, *PQWORD, *LPQWORD;


#define LOBYTE(w) ((BYTE)((QWORD)(w) & 0xFF))
#define HIBYTE(w) ((BYTE)((QWORD)(w) >> 8))

#define LOWORD(l) ((WORD)((QWORD)(l) & 0xFFFF))
#define HIWORD(l) ((WORD)((QWORD)(l) >> 16))

#define PACKED __attribute__((packed))

#endif //OS_MINDEF_HPP

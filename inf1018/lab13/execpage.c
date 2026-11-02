#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include "execpage.h"

#define PAGE_START(p) ((uintptr_t)(p) & ~(uintptr_t)(getpagesize()-1))
#define PAGE_END(p)   (((uintptr_t)(p) + getpagesize()-1) & ~(uintptr_t)(getpagesize()-1))

int execpage(void *p, size_t tam) {
    uintptr_t ini = PAGE_START(p);
    uintptr_t fim = PAGE_END((uintptr_t)p + tam);
    return mprotect((void*)ini, fim - ini, PROT_READ | PROT_WRITE | PROT_EXEC);
}
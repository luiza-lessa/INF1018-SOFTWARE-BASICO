#include <stdio.h>

typedef int(*funcp)(int x);
int execpage(void *ptr, size_t len);

int add(int x){
    return x+1;
}

int sub(int x){
    return x-1;
}

unsigned char codigo[]={
   0x55,                      //push   %rbp
   0x48, 0x89, 0xe5,          //mov    %rsp,%rbp
   0x89, 0xf8,                //mov    %edi,%eax
   0xff, 0xc0,                //inc    %eax
   0xc9,                      //leave
   0xc3                       //ret
};

int main(void){
    int i;
    funcp f;

    execpage(codigo, sizeof codigo);

    i=10;
    printf("add(%d)=%d\n", i, add(i)); //add seguida de parenteses chama a funcao
    printf("sub(%d)=%d\n", i, sub(i));
    
    i=20;
    //pra obter o endereco da funcao so escrever add sem parenteses
    f = add;
    printf("add(%d)=%d\n", i, f(i));
    f=sub;
    printf("sub(%d)=%d\n", i, f(i));

    i = 30;
    f = (funcp)codigo;
    printf("f(%d)=%d\n", i, f(i));
    codigo[7]=0xC8;
    printf("f(%d)=%d\n", i, f(i));

    return 0;
}




#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_START(P) ((intptr_t)(P)&~(pagesize-1))
#define PAGE_END(P) (((intptr_t)(P)+pagesize-1)&~(pagesize-1))

/*
 * The execpage() function shall change the specified memory pages
 * permissions into executable.
 *
 * void *ptr  = pointer to start of memory buff
 * size_t len = memory buff size in bytes
 *
 * The function returns 0 if successful and -1 if any error is encountered.
 * errono may be used to diagnose the error.
 */
int execpage(void *ptr, size_t len) {
	int ret;

	const long pagesize = sysconf(_SC_PAGE_SIZE);
	if (pagesize == -1)
		return -1;

	ret = mprotect((void *)PAGE_START(ptr),
		 PAGE_END((intptr_t)ptr + len) - PAGE_START(ptr),
		 PROT_READ | PROT_WRITE | PROT_EXEC);
	if (ret == -1)
		return -1;

	return 0;
}

#undef PAGE_START
#undef PAGE_END
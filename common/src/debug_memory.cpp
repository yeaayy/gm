
#include <malloc.h>
#include <string.h>

#include "log.h"

namespace {
int id = 0;
char *allocated[1024];
size_t allocatedSize[1024];
}

// void *operator new(size_t size)
// {
//     int allocSize = size + sizeof(int);
//     int *result = (int*) malloc(allocSize);
//     memset(result, 254, allocSize);
//     result[0] = allocSize;
//     return (void*) &result[1];
// }

// void operator delete(void *p) {
//     int *mem = (int*) p;
//     int allocSize = mem[-1];
//     assert_ine(allocSize, 0);
//     memset(&mem[-1], 254, allocSize);
//     free(&mem[-1]);
// }

void *operator new(size_t size)
{
    int allocSize = size + sizeof(int);
    int *result = (int*) malloc(allocSize);
//     memset(result, 254, allocSize);
//     result[0] = allocSize;
//     return (void*) &result[1];
    allocated[id] = (char*) result;
    allocatedSize[id] = allocSize;
    result[0] = allocSize;
    if(id == 192) {
        id = 192;
    }
    id++;
    return &result[1];
}

void operator delete(void *p) {
    int *mem = (int*) p;
    mem--;
    memset(mem, 0x7f, *mem);
    if((void*) mem == (void*) allocated[192]) {
        mem = nullptr;
    }
    // free(p);
    // allocated--;
}

int check()
{
    printf("allocated: %d\n", id);
    for(int i = 0; i < id; i++) {
        int size = allocatedSize[i];
        char *p = allocated[i];
        for(int j = 0; j < size; j++) {
            if(p[j] != 0x7f) {
                printf("Memory %d broken, size %d, %d set to %d\n", i, size, j, p[j]);
                break;
            }
        }
    }
    return 0;
}

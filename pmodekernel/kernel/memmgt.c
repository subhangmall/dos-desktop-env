// paging
#include <stdint.h>

#define PAGE_TABLE_NUM_LAYERS 4
#define PAGE_TABLE_SIZE 4096

#pragma pack(push, 1)

struct PageDirectoryEntry {
    uint32_t present : 1;
    uint32_t rw : 1;
    uint32_t user : 1;
    uint32_t pwt : 1;
    uint32_t pcd : 1;
    uint32_t accessed : 1;
    uint32_t reserved : 1;
    uint32_t pageSize : 1;
    uint32_t ignored : 1;
    uint32_t available : 3;
    uint32_t pageAddress : 20;
};

struct PageTableEntry {
    uint32_t present : 1;
    uint32_t rw : 1;
    uint32_t user : 1;
    uint32_t reserved : 9;
    uint32_t pageAddress : 20;
};

#pragma pack(pop)

__attribute__((aligned(4096)))
struct PageDirectoryEntry kernelPageDirectory[1024];
__attribute__((aligned(4096)))
struct PageTableEntry firstKernelPageTable[1024];

void initMem() {
    struct PageDirectoryEntry pde = {
        .present = 0,
        .rw = 0,
        .user = 0,
        .pwt = 0,
        .pcd = 0,
        .accessed = 0,
        .reserved  = 0,
        .pageSize  = 0,
        .ignored  = 0,
        .available = 0,
        .pageAddress = 0
    };

    struct PageTableEntry pte = { 
        .present = 1,
        .rw = 1,
        .user = 0,
        .reserved = 0,
        .pageAddress = 0
    };

    for (int i = 0; i < 1024; i++) {
        kernelPageDirectory[i] = pde;
        
        pte.pageAddress = i;
        
        firstKernelPageTable[i] = pte;
    }

    struct PageDirectoryEntry firstPage =  {
        .present = 1,
        .rw = 1,
        .user = 0,
        .pwt = 0,
        .pcd = 0,
        .accessed = 0,
        .reserved  = 0,
        .pageSize  = 0,
        .ignored  = 0,
        .available = 1,
        .pageAddress = ((uint32_t) &firstKernelPageTable) >> 12
    };

    // points to first page
    kernelPageDirectory[0] = firstPage;
    
    
    // REMEMBER TO FLUSH TLB PER CHANGE

    // move addr into cr3 register and flip the right bit on cr0 to enable paging
    uint32_t kPDAddress = (uint32_t)&kernelPageDirectory;
    asm volatile (
        "mov %0, %%cr3\n\t"
        "mov %%cr0, %%eax\n\t"
        "or $0x80000000, %%eax\n\t"
        "mov %%eax, %%cr0\n\t"
        "jmp 1f\n\t" // to clear prefetched instructions
        "1:"
        :
        : "r" (kPDAddress)
        : "eax"
    );

}


// kernel allocation, for drivers and such
void* kalloc(int numBytes) {
    asm volatile (
        ""
        :
        :
        :
    );
}

// zeroes out memory as well
void* kcalloc(int numBytes) {

}

void free(void* mem) {

}
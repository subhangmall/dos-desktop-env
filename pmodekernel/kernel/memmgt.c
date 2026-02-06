// paging
#include <stdint.h>
#include <stdbool.h>

#define PAGE_SIZE 4096

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
    uint32_t pwt : 1;
    uint32_t pcd : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t pageTableAttrIdx : 1;
    uint32_t globalPage : 1;
    uint32_t available : 3;
    uint32_t pageAddress : 20;
};

#pragma pack(pop)

__attribute__((aligned(PAGE_SIZE)))
struct PageDirectoryEntry kernelPageDirectory[PAGE_SIZE/sizeof(struct PageDirectoryEntry)];
__attribute__((aligned(PAGE_SIZE)))
struct PageTableEntry firstKernelPageTable[PAGE_SIZE/sizeof(struct PageTableEntry)];

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
        .pwt = 0,
        .pcd = 0,
        .accessed = 0,
        .dirty = 0,
        .pageTableAttrIdx = 0,
        .globalPage = 0,
        .available = 0,
        .pageAddress = 0
    };

    for (int i = 0; i < PAGE_SIZE/sizeof(struct PageTableEntry); i++) {
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

void* addPage(uint32_t vAddr) {
    uint16_t ptdIdx = (uint16_t)((vAddr >> 22)&0b1111111111);
    uint16_t pteIdx = (uint16_t)((vAddr & 0b00000000001111111111000000000000) >> 12);

    if (kernelPageDirectory[ptdIdx].present == 1) {
        // page table present
        
    } else {
        // page table not present
       __attribute__((aligned(PAGE_SIZE)))
       struct PageTableEntry newPage[1024];

       struct PageDirectoryEntry new  = {
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
            .pageAddress = ((uint32_t) &newPage) >> 12
       };
       kernelPageDirectory[ptdIdx] = new;
       asm volatile (
            "invlpg (%0)"
            :
            : "r" (vAddr)
            : "memory"
       );
       return addPage(vAddr);
    }
}

// kernel allocation, for drivers and such
void* kalloc(int numBytes) {
    
}

// zeroes out memory as well
void* kcalloc(int numBytes) {

}

void free(void* mem) {

}
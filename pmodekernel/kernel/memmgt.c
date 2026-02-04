// paging

#define PAGE_TABLE_NUM_LAYERS 4
#define PAGE_TABLE_SIZE 4096

void initMem() {

}

void initPageTables() {
    asm volatile (
        ""
        :
        :
        :
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
void kcalloc(int numBytes) {

}
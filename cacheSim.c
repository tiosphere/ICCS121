#include "cacheSim.h"

// In this question, we will assume DRAM will take a 4-byte values starting from 0 to
void init_DRAM()
{
        unsigned int i=0;
        DRAM = malloc(sizeof(char) * DRAM_SIZE);
        for(i=0;i<DRAM_SIZE/4;i++)
        {
                *((unsigned int*)DRAM+i) = i;
        }
}

void printCache()
{
        int i,j,k;
        printf("===== L1 Cache Content =====\n");
        for(i=0;i<2;i++)
        {
                printf("Set %d :", i);
                for(j=0;j<2;j++)
                {
                        printf(" {(TAG: 0x%x)", (unsigned int)(L1_cache[i][j].tag));
                        for(k=0;k<16;k++)
                                printf(" 0x%x,", (unsigned int)(L1_cache[i][j].data[k]));
                        printf(" |");
                }
                printf("\n");
        }
        printf("===== L2 Cache Content =====\n");
        for(i=0;i<4;i++)
        {
                printf("Set %d :", i);
                for(j=0;j<4;j++)
                {
                        printf(" {(TAG: 0x%x)", (unsigned int)(L2_cache[i][j].tag));
                        for(k=0;k<16;k++)
                                printf(" 0x%x,", (unsigned int)(L2_cache[i][j].data[k]));
                        printf(" |");
                }
                printf("\n");
        }
}

u_int32_t read_fifo(u_int32_t address)
{
///// IMPLEMENT THIS ////
        printf("READ FIFO\n");
        unsigned int l1_set_id = getL1SetID(address);
        unsigned int l1_tag_id = getL1Tag(address);
        for(int i = 0; i < 4; i++){
                
                if (L1_cache[l1_set_id][i].tag == l1_tag_id){
                        printf("DATA: %s \n", L1_cache[l1_set_id][i].data);
                        return L1_cache[l1_set_id][i].data[0];
                }
        }
        unsigned int l2_set_id = getL2SetID(address);
        unsigned int l2_tag_id = getL2Tag(address);
        for(int i = 0; i < 4; i++){
                
                if (L2_cache[l2_set_id][i].tag == l2_tag_id){
                        printf("DATA: %s \n", L2_cache[l2_set_id][i].data);
                        return L2_cache[l2_set_id][i].data[0];
                }
        }
}

int L1lookup(u_int32_t address)
{
///// IMPLEMENT THIS /////
        unsigned int set_id = getL1SetID(address);
        unsigned int tag_id = getL1Tag(address);
        for(int i = 0; i < 4; i++){
                if (L1_cache[set_id][i].tag == tag_id){
                        return 1;
                }
        }
        return 0;
}

int L2lookup(u_int32_t address)
{
///// IMPLEMENT THIS /////
        unsigned int set_id = getL2SetID(address);
        unsigned int tag_id = getL2Tag(address);
        for(int i = 0; i < 4; i++){
                if (L2_cache[set_id][i].tag == tag_id){
                        return 1;
                }
        }
        return 0;
}

unsigned int getL1SetID(u_int32_t address)
{
///// IMPLEMENT THIS /////
printf("L1 SET ID: %#x\n", (address >> 4) & 0x1);
return (address >> 4) & 0x1;
}

unsigned int getL2SetID(u_int32_t address)
{
///// IMPLEMENT THIS /////
printf("L2 SET ID: %#x\n", (address >> 4) & 0x3);
return (address >> 4) & 0x3;
}

unsigned int getL1Tag(u_int32_t address)
{
///// IMPLEMENT THIS /////
printf("L1 TAG ID: %#x\n", address >> 5);
return address >> 5;
}

unsigned int getL2Tag(u_int32_t address)
{
///// IMPLEMENT THIS /////
printf("L2 TAG ID: %#x\n", address >> 6);
return address >> 6;
}


void write(u_int32_t address, u_int32_t data)
{
///// REPLACE THIS /////
        read_fifo(address);
return;
}


int main()
{
        init_DRAM();
        cacheAccess buffer;
        int timeTaken=0;
        FILE *trace = fopen("input.trace","r");
        int L1hit = 0;
        int L2hit = 0;
        cycles = 0;
        while(!feof(trace))
        {
                fscanf(trace,"%d %x %x", &buffer.readWrite, &buffer.address, &buffer.data);
                printf("Processing the request for [R/W] = %d, Address = %x, data = %x\n", buffer.readWrite, buffer.address, buffer.data);

                // Checking whether the current access is a hit or miss so that we can advance time correctly
                if(L1lookup(buffer.address))// Cache hit
                {
                        printf("FOUND IN L1\n");
                        timeTaken = 1;
                        L1hit++;
                }
                else if(L2lookup(buffer.address))// L2 Cache Hit
                {
                        printf("FOUND IN L2\n");
                        L2hit++;
                        timeTaken = 5;
                }
                else timeTaken = 50;
                if (buffer.readWrite) write(buffer.address, buffer.data);
                else read_fifo(buffer.address);
                cycles+=timeTaken;
        }
        printCache();
        printf("Total cycles used = %ld\nL1 hits = %d, L2 hits = %d", cycles, L1hit, L2hit);
        fclose(trace);
        free(DRAM);
        return 0;
}



#define NUMFRAMES 8
#define NUMPAGES 16
#define NUMTRANS 4
#define	PAGESIZE 256
#define READIN_BUF_SIZE 32

#include "stdlib.h"
#include "stdio.h"
#include <pthread.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

struct curr_addr	//structure to hold each address
{
	int logAddr;	//logical address found in text file
	int pagenum;	//page number (bits 8-15)
	int offset;	//offset (bits 0-7)
	int physAddr;
	int value;
};

struct PageTable
{
      int framenum;
      int accessTime;
};

struct TLB
{
     int framenum;
     int pagenum;
     int accessTime;
};

struct PageTable pagetable[NUMPAGES];
struct TLB tlb[NUMTRANS];
int physMem[NUMFRAMES][PAGESIZE];
int FrameTable[NUMFRAMES];
int pageFault=0;
int frameNum=0;
int tlbNum=0;
int tlbHit=0;
int address_array[1000];
struct curr_addr addr={0x00000000,0,0,0,0};
int WorkTime= 0;

void backingStore();
void pageTableLookup();
void TLBLookup(); 
void page_cal();



void main(int argc, char *argv[]){
	
	int numberOfAddress=0;
	
	
	FILE *stream;
        char address[READIN_BUF_SIZE];
	if((stream=fopen("addresses.txt","r"))==NULL) {  
        printf("cannot open file\n"); exit(1);  
    	}
	int index=0;
	while(1){
	fgets(address,READIN_BUF_SIZE,stream);      
		if(feof(stream)){ 
			break;
	      	}
		address_array[index] = atoi(address);
		index++;	      	
   	}
   	fclose(stream);
	numberOfAddress=index;
	/*for(index=0; index<numberOfAddress; index++){
			printf("%d\n",address_array[index]);
        }*/
	//printf("%d\n",numberOfAddress );

	// initialize the page table
	for(index=0; index<NUMPAGES;index++){
		pagetable[index].framenum = -1;
		pagetable[index].accessTime= 0;
	}
	// initialize the TLB
	for(index=0; index<NUMTRANS;index++){
		tlb[index].framenum = -1;
		tlb[index].accessTime= 0;
		tlb[index].pagenum = -1;
	}
	// initialize the FrameTalbe
	for(index=0; index<NUMFRAMES;index++){
		FrameTable[index]=0;
	}
	
	// read in all the address in the address.txt
        for(index=0; index< numberOfAddress; index++){
	// assign the logic address to struct addr
		addr.logAddr=address_array[index];
		WorkTime++;
		page_cal();
		TLBLookup(); 
		//printf("%d %d\n",addr.pagenum, addr.offset);
		addr.logAddr=0;
		}
	// print out the result
     for(index=0; index< NUMPAGES; index++){
	if(pagetable[index].framenum==-1)
	{printf("page %d: not in memory\n",index);
	}
	else
	printf("page %d: frame %i\n",index,pagetable[index].framenum);

	}
     for(index=0;index<frameNum;index++){
	if(FrameTable[index]==0)	
	printf("frame %d: empty\n",index);
	else
	printf("frame %d: page %d\n",index,FrameTable[index]);
	}
	printf("%d page faults out of %d reference\n",pageFault,numberOfAddress);
	printf("%d hits out of %d references\n",tlbHit,numberOfAddress);	



}

void page_cal(){
    addr.pagenum = (addr.logAddr & 0x0000FF00) >> (8);
    addr.offset = addr.logAddr & 0x000000FF;

}

void TLBLookup(){
	int index=0;
	int flag=0;
	int min =1000;
	int lru_p=0;
	// iterate the tlb block to match the page
	for(index=0; index<NUMTRANS;index++){
		//if it can be find in the tlb
		if (addr.pagenum==tlb[index].pagenum){
		 	flag=1;
			tlbHit++;
			printf("page %d in frame %d is contained in tlb %d\n", addr.pagenum,tlb[index].framenum, index);
			tlb[index].accessTime=WorkTime;
			pagetable[addr.pagenum].accessTime=WorkTime;
			addr.physAddr=tlb[index].framenum*PAGESIZE+addr.offset;
			}
	}
	// if it can't be find in the tlb 
	if(flag==0){
		pageTableLookup();
	
	for(index=0; index<NUMTRANS;index++){
	// find the lru page in the tlb
	if (tlb[index].accessTime<min){
		min=tlb[index].accessTime;
		lru_p=index;	
	
	}
	}
	tlb[lru_p].pagenum=addr.pagenum;
	tlb[lru_p].framenum=pagetable[addr.pagenum].framenum;
	tlb[lru_p].accessTime=WorkTime;
	printf("page %d in frame %d is loaded in tlb %d\n", addr.pagenum, pagetable[addr.pagenum].framenum,lru_p);
		}
		
}


void pageTableLookup(){
	int index=0;
	int min=1000;
	int lru_p=0;
	// page is already in the table
	if (pagetable[addr.pagenum].framenum > -1){
		printf("page %i is contained in frame %i\n", addr.pagenum, pagetable[addr.pagenum].framenum);
		pagetable[addr.pagenum].accessTime=WorkTime;
		addr.physAddr=pagetable[addr.pagenum].framenum*PAGESIZE+addr.offset;
	}
	// if page is not in the table, but free frame is available
	else if (pagetable[addr.pagenum].framenum==-1 && frameNum< NUMFRAMES){
		pageFault++;
		printf("virtual address %d contained in page %d causes a page fault\n", addr.logAddr,addr.pagenum);
		pagetable[addr.pagenum].framenum=frameNum;
		backingStore();
		printf("page %i is loaded in frame %i\n", addr.pagenum, pagetable[addr.pagenum].framenum);
		FrameTable[frameNum]=addr.pagenum;
		frameNum++;
		pagetable[addr.pagenum].accessTime=WorkTime;
		addr.physAddr=pagetable[addr.pagenum].framenum*PAGESIZE+addr.offset;
	}
	// if page is not in the table and no free frame is available, lru is used
	else if (pagetable[addr.pagenum].framenum==-1 && frameNum == NUMFRAMES){
			pageFault++;
		printf("virtual address %d contained in page %d causes a page fault\n", addr.logAddr,addr.pagenum);
		for(index=0; index<NUMPAGES; index++){
			if(pagetable[index].framenum!=-1){
			if(pagetable[index].accessTime<min){
				min=pagetable[index].accessTime;
				lru_p=index;
				}
			    }			
			}
		pagetable[addr.pagenum].framenum=pagetable[lru_p].framenum;
		FrameTable[pagetable[lru_p].framenum]=addr.pagenum;
		pagetable[lru_p].framenum=-1;
		backingStore();
		printf("page %i is loaded in frame %i\n", addr.pagenum, pagetable[addr.pagenum].framenum);
		pagetable[addr.pagenum].accessTime=WorkTime;
		addr.physAddr=pagetable[addr.pagenum].framenum*PAGESIZE+addr.offset;
				
	}

}

void backingStore(){
	char temp[PAGESIZE];
	int index;
	FILE *bs;
	bs=fopen("BACKING_STORE.bin","rb");	
		
	if(bs == NULL)
    {
        printf("Error: cannot open BACKING_STORE.bin");
    }
    
    if(fseek(bs, addr.pagenum * PAGESIZE, SEEK_SET) != 0)
    {
        printf("Error: fseek failed");
    }

    if(fread(temp, 1, PAGESIZE, bs) != PAGESIZE)
    {
        printf("Error: fread failed");
    }
    
    fclose(bs);
    for(index = 0; index < PAGESIZE; index++)
    {
        physMem[pagetable[addr.pagenum].framenum][index] = temp[index];
    }


}












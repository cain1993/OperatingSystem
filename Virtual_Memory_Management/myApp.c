#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


/*int make_fs(char *disk_name);
int mount_fs(char *disk_name);
int dismount_fs(char *disk_name);
int fs_open(char *name);
int fs_close(int fildes);
int fs_create(char *name);
int fs_delete(char *name);
int fs_read(int fildes, void *buf, size_t nbyte);
int fs_write(int fildes, void *buf, size_t nbyte);
int fs_get_filesize(int fildes);
int fs_lseek(int fildes, off_t offset);
int fs_truncate(int fildes, off_t length);*/

size_t nbyte;
off_t offset;
off_t length;

#define BUFFER_SIZE 160
#define BLOCK_SIZE 16

int i;

char tempBuf[BUFFER_SIZE];// read buffer

int fda, fdb, fdc, fdd, fde; // 5 file descriptors


int main() {

  make_fs("mydk");
  mount_fs("mydk");
  fs_create("fa");
  fda = fs_open("fa");
  char bufa[] = "The goal of this project";
  nbyte = 24;
  fs_write(fda,&bufa,nbyte);

  fs_create("fb");
  fdb = fs_open("fb");
  char bufb[] = "CS6456 is fun";
  nbyte = 13;
  fs_write(fdb,&bufb,nbyte);

   fs_create("fc");
  fdc = fs_open("fc");
  char bufc[] = "I love computer programming because I can ask computers to do something for me";
  nbyte = 78;
  fs_write(fdc,&bufc,nbyte);
	
 fs_create("fd");
  fdd = fs_open("fd");
  char bufd[] = "Hi, there";
  nbyte = 9;
  fs_write(fdd,&bufd,nbyte);

  fs_create("fe");
  length = fs_get_filesize(fdc);
  printf("file size of file fc = %d\n", (int) length); // file size of file fc = 78
  
  nbyte = 20;

  offset = -24;
  fs_lseek(fda, offset);
  fs_read(fda, tempBuf, nbyte); 
  for (i = 0; i < nbyte; i++)
	  printf("%c", tempBuf[i]); // The goal of this pro
  printf("\n");

   offset = -58;
  fs_lseek(fdc, offset);   
  nbyte = 10;
  fs_read(fdc, tempBuf, nbyte);
  for (i = 0; i < nbyte; i++)
	  printf("%c", tempBuf[i]); // ramming be
  printf("\n");

  length = 30;
  fs_truncate(fdc, length);
  nbyte = 30;
  fs_read(fdc, tempBuf, nbyte); 
  for (i = 0; i < nbyte; i++)
	  printf("%c", tempBuf[i]); // I love computer programming be
  printf("\n");
	
  fs_close(fdb);
  fde = fs_open("fe");
  char bufe[] = "I am glad this is the last project for the semester";
  nbyte = 51;
  fs_write(fde,&bufe,nbyte);
   fs_delete("fb");

  dismount_fs("mydk");
  mount_fs("mydk");

  fda = fs_open("fa");
  fdb = fs_open("fb");
  printf("status of file fb = %d\n",fdb); // status of file fb = -1
  fdc = fs_open("fc");
  fdd = fs_open("fd");
  fde = fs_open("fe");
 
  nbyte = 24;
  fs_read(fda, tempBuf, nbyte);
  for (i = 0; i < nbyte; i++)
	  printf("%c", tempBuf[i]); // The goal of this project
  printf("\n");

  nbyte = 30;
  fs_read(fdc, tempBuf, nbyte);
  for (i = 0; i < nbyte; i++)
	  printf("%c", tempBuf[i]); // I love computer programming be
  printf("\n");

  nbyte = 9;
  fs_read(fdd, tempBuf, nbyte);
  for (i = 0; i < nbyte; i++)
	  printf("%c", tempBuf[i]); // Hi, there
  printf("\n");

  nbyte = 51;
  fs_read(fde, tempBuf, nbyte);
  for (i = 0; i < nbyte; i++)
	  printf("%c", tempBuf[i]); // I am glad this is the last project for the semester
  printf("\n");

  dismount_fs("mydk");



	/*int pt;
	for(pt=0;pt<160;pt++){
		if(pt==0){printf("Super\n");}
		if(pt==16){printf("OFT\n");}
		if(pt==32){printf("FAT\n");}
		if(pt==96){printf("Dir\n");}
		printf("%d|",metaBuf[pt]);
		
		if(pt%16==15){printf("$$\n");}
	}
	char test[BUFFER_SIZE];

	int tst;	
	for(tst=0;tst<32;tst++){
	block_read(32+tst,test);
	printf(test);
	printf("|%d|\n",32+tst);}
	
	int oft;
	printf("OFT ON MEM\n");
	for(oft=0;oft<4;oft++){
		printf("%d|",OFT[oft][0]);
		printf("%d|",OFT[oft][1]);
		printf("%d|",OFT[oft][2]);
		printf("\n");
	}*/
  return 0;
}

 


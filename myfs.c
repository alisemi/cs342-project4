#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "myfs.h"
#include "perProcessLinkedList.h"

typedef struct fcb{
	char filename[32];
	int startAddress;
} fcb;


typedef struct system_wide_entry {
	fcb *fcb_instance;
	int count;
}system_wide_entry;


typedef struct per_process_entry {
	int fcb_index;//fcb pointer
	int file_poisiton;
}per_process_entry;


typedef struct volume_control {
	int numberOfBlocks;
	int sizeOfBlocks;
	int freeBlockCount;
	char nameOfDisk[32];
	int fcbCount;
	int freeFCB;
	int pointerToFAT;
	int pointerToFCB;
}volume_control;


// Global Variables
char disk_name[128];   // name of virtual disk file
int  disk_size;        // size in bytes - a power of 2
int  disk_fd;          // disk file handle
int  disk_blockcount;  // block count on disk
const int fatBlockSize = 24;

system_wide_entry* system_wide_table;
per_process_entry* per_process_table;
int file_descriptor_index;


/* 
   Reads block blocknum into buffer buf.
   You will not modify the getblock() function. 
   Returns -1 if error. Should not happen.
*/
int getblock (int blocknum, void *buf)
{      
	int offset, n; 
	
	if (blocknum >= disk_blockcount) 
		return (-1); //error

	offset = lseek (disk_fd, blocknum * BLOCKSIZE, SEEK_SET); 
	if (offset == -1) {
		printf ("lseek error\n"); 
		exit(0); 

	}

	n = read (disk_fd, buf, BLOCKSIZE); 
	if (n != BLOCKSIZE) 
		return (-1); 

	return (0); 
}


/*  
    Puts buffer buf into block blocknum.  
    You will not modify the putblock() function
    Returns -1 if error. Should not happen. 
*/
int putblock (int blocknum, void *buf)
{
	int offset, n;
	
	if (blocknum >= disk_blockcount) 
		return (-1); //error

	offset = lseek (disk_fd, blocknum * BLOCKSIZE, SEEK_SET);
	if (offset == -1) {
		printf ("lseek error\n"); 
		exit (1); 
	}
	
	n = write (disk_fd, buf, BLOCKSIZE); 
	if (n != BLOCKSIZE) 
		return (-1); 

	return (0); 
}


/* 
   IMPLEMENT THE FUNCTIONS BELOW - You can implement additional 
   internal functions. 
 */


int myfs_diskcreate (char *vdisk)
{	
	int size, numblocks, ret, n, fd, i;
	char buf[BLOCKSIZE]; 

	size = DISKSIZE; 
	numblocks = DISKSIZE / BLOCKSIZE; 

	printf ("diskname=%s size=%d blocks=%d\n", vdisk, size, numblocks); 
	ret = open (vdisk,  O_CREAT | O_RDWR, 0666); 	
	if (ret == -1) {
		printf ("could not create disk\n"); 
		return -1;
	}
					
	bzero ((void *)buf, BLOCKSIZE); 
	fd = open (vdisk, O_RDWR); 
	for (i=0; i < (size / BLOCKSIZE); ++i) {
		//printf ("block=%d\n", i); 
		n = write (fd, buf, BLOCKSIZE); 
		if (n != BLOCKSIZE) {
			printf ("write error\n");
			return -1;
		}
	}	
	close (fd); 
						
	printf ("created a virtual disk=%s of size=%d\n", vdisk, size);	

  	return(0); 
}


/* format disk of size dsize */
int myfs_makefs(char *vdisk)
{
	strcpy (disk_name, vdisk); 
	disk_size = DISKSIZE; 
	disk_blockcount = disk_size / BLOCKSIZE; 

	disk_fd = open (disk_name, O_RDWR); 
	if (disk_fd == -1) {
		printf ("disk open error %s\n", vdisk); 
		exit(1); 
	}
	
	// perform your format operations here. 
	printf ("formatting disk=%s, size=%d\n", vdisk, disk_size); 
	
	//Volume Control
	volume_control* myVolumeControl = (volume_control*) malloc( sizeof(volume_control) );
	myVolumeControl->numberOfBlocks = BLOCKCOUNT;
	myVolumeControl->sizeOfBlocks = BLOCKSIZE;
	
	myVolumeControl->freeBlockCount = (3/4) * BLOCKCOUNT;
	strncpy(myVolumeControl->nameOfDisk, vdisk, 32);
	myVolumeControl->fcbCount = 0;
	myVolumeControl->freeFCB = MAXFILECOUNT;
	myVolumeControl->pointerToFAT = 3*BLOCKSIZE;
	myVolumeControl->pointerToFCB = 1*BLOCKSIZE;

	printf("%lu\n", sizeof(myVolumeControl) );

	/*
	volume_control* temp = realloc( myVolumeControl, 4096);
	if( temp != NULL) {
		myVolumeControl = temp;
	}
	*/
	printf("%lu\n", sizeof(myVolumeControl) );
	
	if( putblock ( 1, myVolumeControl) != 0 ){
		printf("error while putting volumeControl block\n");
		exit(1);
	}

	volume_control* temp = (volume_control*) malloc( sizeof(volume_control) );
	getblock( 1, temp);
	printf("%d\n", temp->freeFCB);

	/*
	char *temp = (char*) malloc( sizeof(char));
	memcpy(temp, myVolumeControl, sizeof(myVolumeControl));
	*/


	//FAT
	for(int index = 1; index < fatBlockSize; index++){
		int *a;
		a = malloc(BLOCKSIZE);
		for(int i = 0; i < BLOCKSIZE; i += 1){
			a[i] = -1;
		}
		putblock(index+3, a);
	}

	//FCB
	fcb *a = malloc(sizeof(fcb)*64);
	fcb *b = malloc(sizeof(fcb)*64);
	//a[0].filename = "cevat";
	//strcpy(a[0].filename, "cevat");
	//a[0].startAddress = 0;
	for(int index = 0; index < 64; index++){
		strcpy(a[index].filename, "");
		a[index].startAddress = -1;
		strcpy(b[index].filename, "");
		b[index].startAddress = -1;
	}
	//put_fcbs(&a);
	
	putblock(1, a);
	putblock(2, b);
	

	fsync (disk_fd); 
	close (disk_fd); 

	return (0); 
}

/* 
   Mount disk and its file system. This is not the same mount
   operation we use for real file systems:  in that the new filesystem
   is attached to a mount point in the default file system. Here we do
   not do that. We just prepare the file system in the disk to be used
   by the application. For example, we get FAT into memory, initialize
   an open file table, get superblock into into memory, etc.
*/

int myfs_mount (char *vdisk)
{
	struct stat finfo; 

	strcpy (disk_name, vdisk);
	disk_fd = open (disk_name, O_RDWR); 
	if (disk_fd == -1) {
		printf ("myfs_mount: disk open error %s\n", disk_name); 
		exit(1); 
	}
	
	fstat (disk_fd, &finfo); 

	printf ("myfs_mount: mounting %s, size=%d\n", disk_name, 
		(int) finfo.st_size);  
	disk_size = (int) finfo.st_size; 
	disk_blockcount = disk_size / BLOCKSIZE; 

	// perform your mount operations here

	// write your code
	system_wide_entry* system_wide_table = malloc(128*sizeof(system_wide_entry));
	file_descriptor_index = 0;


	
	/* you can place these returns wherever you want. Below
	   we put them at the end of functions so that compiler will not
	   complain.
        */
  	return (0); 
}


int myfs_umount()
{
	// perform your unmount operations here

	// write your code

	fsync (disk_fd); 
	close (disk_fd); 
	return (0); 
}


/* create a file with name filename */
int myfs_create(char *filename)
{

	// write your code 

	return (0); 
}


/* open file filename */
int myfs_open(char *filename)
{
	int index = -1; 
	
	// write your code
       
	return (index); 
}

/* close file filename */
int myfs_close(int fd)
{

	// write your code

	return (0); 
}

int myfs_delete(char *filename)
{

	// write your code

	return (0); 
}

int myfs_read(int fd, void *buf, int n)
{
	int bytes_read = -1; 

	// write your code
	
	return (bytes_read); 

}

int myfs_write(int fd, void *buf, int n)
{
	int bytes_written = -1; 

	// write your code

	return (bytes_written); 
} 

int myfs_truncate(int fd, int size)
{

	// write your code

	return (0); 
} 


int myfs_seek(int fd, int offset)
{
	int position = -1; 

	// write your code

	return (position); 
} 

int myfs_filesize (int fd)
{
	int size = -1; 
	
	// write your code

	return (size); 
}


void myfs_print_dir ()
{

	// write your code
	
}


void myfs_print_blocks (char *  filename)
{

	// write your code

}

//Internal functions
int get_fat(int index){
	int block = index / 1024;
	int seek = index % 1024;

	int *a;
	a = malloc(BLOCKSIZE);
	getblock(block, (void*)a);
	return a[seek];
	
}

int put_fat(int index, int value){
	int block = index / 1024;
	int seek = index % 1024;

	int *a;
	a = malloc(BLOCKSIZE);
	getblock(block, (void*)a);
	a[seek] = value;
	putblock(block, (void*)a);
	return a[seek];
}


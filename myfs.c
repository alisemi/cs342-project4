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
	char buffer[28];
} fcb;


typedef struct system_wide_entry {
	fcb *fcb_instance;
	int count;
}system_wide_entry;



typedef struct per_process_entry {
	int fcb_index;//fcb pointer
	int file_position;
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
	int buffer;
}volume_control;

struct process_node {
	per_process_entry data;
	int key;
	struct process_node *next;
};


// Global Variables
char disk_name[128];   // name of virtual disk file
int  disk_size;        // size in bytes - a power of 2
int  disk_fd;          // disk file handle
int  disk_blockcount;  // block count on disk
const int fatBlockSize = 24;

system_wide_entry* system_wide_table;
int file_descriptor_index;
int **fatTableCached = NULL;
volume_control* volumeControlCached = NULL;

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
	//printf("put block n = %d\n", n);
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
	volume_control* myVolumeControl = (volume_control*) malloc( 64* sizeof(volume_control) );
	myVolumeControl[0].numberOfBlocks = BLOCKCOUNT;
	myVolumeControl[0].sizeOfBlocks = BLOCKSIZE;
	
	myVolumeControl[0].freeBlockCount = (3/4) * BLOCKCOUNT;
	strncpy(myVolumeControl[0].nameOfDisk, vdisk, 32);
	myVolumeControl[0].fcbCount = 0;
	myVolumeControl[0].freeFCB = MAXFILECOUNT;
	myVolumeControl[0].pointerToFAT = 3*BLOCKSIZE;
	myVolumeControl[0].pointerToFCB = 1*BLOCKSIZE;
	myVolumeControl[0].buffer = 0;


	//We might not need to initialize the other indexes as they act as buffers for the block
	/*	
	for( int i = 1; i < 64; i++) {
		myVolumeControl[i].numberOfBlocks = BLOCKCOUNT;
		myVolumeControl[i].sizeOfBlocks = BLOCKSIZE;
	
		myVolumeControl[i].freeBlockCount = (3/4) * BLOCKCOUNT;
		strncpy(myVolumeControl[i].nameOfDisk, vdisk, 32);
		myVolumeControl[i].fcbCount = 0;
		myVolumeControl[i].freeFCB = MAXFILECOUNT;
		myVolumeControl[i].pointerToFAT = 3*BLOCKSIZE;
		myVolumeControl[i].pointerToFCB = 1*BLOCKSIZE;
		myVolumeControl[i].buffer = 0;
	}
	*/
		
	if( putblock ( 0, myVolumeControl) != 0 ){
			printf("error while putting volumeControl block\n");
			return -1;
	}

	free(myVolumeControl);
	//free(temp);

	//Self-check
	volume_control* temp = (volume_control*) malloc( 64*sizeof(volume_control) );
	getblock( 0, temp);
	//printf("%d\n", temp[0].freeFCB);

	free(temp);	
	

	//FAT
	for(int index = 0; index < fatBlockSize; index++){
		int *a;
		a = (int*) malloc(BLOCKSIZE);
		for(int i = 0; i < BLOCKSIZE/4; i += 1){
			a[i] = -1;
		}
		putblock(index+3, a);
		free(a);
	}

	//Self-check
	int* trial = (int*) malloc(BLOCKSIZE);
	if( getblock(3, trial) != 0) {
		printf("Error\n");
	}
	else {
		//printf("%d\n", trial[0] );
	}
	free(trial);

	
	//FCB - is divided into two blocks	
	fcb *a = malloc(sizeof(fcb)*64);
	fcb *b = malloc(sizeof(fcb)*64);
	//a[0].filename = "cevat";
	//strcpy(a[0].filename, "cevat");
	//a[0].startAddress = 0;
	for(int index = 0; index < 64; index++){
		strcpy(a[index].filename, "");
		a[index].startAddress = -1;

		b[index].startAddress = -1;
		strcpy(b[index].filename, "");
	}
	//put_fcbs(&a);
	
	putblock(1, a);
	putblock(2, b);

	//Self check
	fcb *fcb_trial = malloc(sizeof(fcb)*64);
	if( getblock(1, fcb_trial) != 0) {
		printf("Error\n");
	}
	else {
		//printf("First fcb name is %s start address is %d\n", fcb_trial[0].filename, fcb_trial[0].startAddress);
	}
	free(fcb_trial);
	free(a);
	free(b);
	

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

	// performing your mount operations here

	//System_wide_entry
	system_wide_table = malloc(128*sizeof(system_wide_entry));
	for( int i = 0; i < 128; i++) {
		system_wide_table[i].fcb_instance = NULL;
		system_wide_table[i].count = 0;
	}

	//Per-process-table, implemented as linked list
	
	file_descriptor_index = 0;
	
	//Example per-process-table addition
	/*
	per_process_entry* new_entry = malloc( sizeof(per_process_entry));
	new_entry->fcb_index = 0;
	new_entry->file_position = 0;
	
	p_insertFirst(file_descriptor_index, *new_entry);
	*/

	//Volume control is cached into memory
	volumeControlCached = malloc(sizeof(volume_control));
	volume_control* temp_volume_control = malloc(BLOCKSIZE);
	getblock( 0, temp_volume_control);

	volumeControlCached->numberOfBlocks = temp_volume_control[0].numberOfBlocks;
	volumeControlCached->sizeOfBlocks = temp_volume_control[0].sizeOfBlocks;
	volumeControlCached->freeBlockCount = temp_volume_control[0].freeBlockCount;
	strncpy(volumeControlCached->nameOfDisk, temp_volume_control[0].nameOfDisk, 32);
	volumeControlCached->fcbCount = temp_volume_control[0].fcbCount;
	volumeControlCached->freeFCB = temp_volume_control[0].freeFCB;
	volumeControlCached->pointerToFAT = temp_volume_control[0].pointerToFAT;
	volumeControlCached->pointerToFCB = temp_volume_control[0].pointerToFCB;
	volumeControlCached->buffer = temp_volume_control[0].buffer;
	
	//printf("%d\n", volumeControlCached->freeFCB);
	free(temp_volume_control);

	
	//FAT table is cached into memory
	//fatTableCached = malloc(fatBlockSize*BLOCKSIZE*sizeof(int) ); using 2d array instead for efficiency
	fatTableCached = malloc(sizeof(int*)*fatBlockSize);
	for( int i = 0; i < fatBlockSize; i++) {
		//printf("%d\n", BLOCKSIZE);
		fatTableCached[i] = malloc(sizeof(int)*BLOCKSIZE);
		if (!fatTableCached[i]) { 
			perror("malloc arr"); 
			return -1;
		}
		
		if( getblock(i+3, fatTableCached[i]) != 0) {
			printf("Error\n");
			return -1;
		}
		else {
			//printf("i is %d and fat table is cached %d\n", i, fatTableCached[i][0]);
		}
	}
	
	/* you can place these returns wherever you want. Below
	   we put them at the end of functions so that compiler will not
	   complain.
        */
  	return (0); 
}


int myfs_umount()
{
	//performing your unmount operations here

	//Writing back the cached metadata into the disk

	//Control if not mounted
	if( volumeControlCached == NULL || fatTableCached == NULL ) {
		printf("Error, the file system is not mounted!\n");
		return -1;
	}

	//Volume Control block
	volume_control* temp_volume_control = malloc(BLOCKSIZE);

	temp_volume_control[0].numberOfBlocks = volumeControlCached->numberOfBlocks; 
	temp_volume_control[0].sizeOfBlocks = volumeControlCached->sizeOfBlocks;
	temp_volume_control[0].freeBlockCount = volumeControlCached->freeBlockCount;
	strncpy(temp_volume_control[0].nameOfDisk, volumeControlCached->nameOfDisk, 32);
	temp_volume_control[0].fcbCount = volumeControlCached->fcbCount;
	temp_volume_control[0].freeFCB = volumeControlCached->freeFCB;
	temp_volume_control[0].pointerToFAT = volumeControlCached->pointerToFAT;
	temp_volume_control[0].pointerToFCB = volumeControlCached->pointerToFCB;
	temp_volume_control[0].buffer = volumeControlCached->buffer;
	
	putblock(0, temp_volume_control);

	//printf("%d\n", temp_volume_control[0].freeFCB);
	free(temp_volume_control);
	free(volumeControlCached);
	volumeControlCached = NULL;

	//FAT Table
	for( int i = 0; i < fatBlockSize; i++) {
		if( putblock(i+3, fatTableCached[i]) != 0) {
			printf("Error\n");
			return -1;
		}
		else {
			//printf("i is %d and fat table is cached back to memory %d\n", i, fatTableCached[i][0]);
			free(fatTableCached[i] );
		}
	}
	

	free(fatTableCached);
	fatTableCached = NULL;


	//Free perProcessLinked list
	p_destroyList();

	//Free system_wide_table
	for( int i = 0; i < 128; i++) {
		if( system_wide_table[i].fcb_instance != NULL) {
			free(system_wide_table[i].fcb_instance );
		}
	}
	
	free(system_wide_table);
	system_wide_table = NULL;

	fsync (disk_fd); 
	close (disk_fd); 
	return (0); 
}

int getFreeBlock() {
	for( int i = 0; i < fatBlockSize; i++) {
		for( int j = 0; j < BLOCKSIZE/4; j++) {
			if(fatTableCached[i][j] == -1) {
				return (i*1024) + j;
			}
		}
	}
	return -1;
}


/* create a file with name filename
  returns 0 if file is successfully created,
  returns -1 if it can't be created, (no space or file limit exceeds),
  returns -2 if the file name already exists in the disk
 */
int myfs_create(char *filename)
{
	int result = -1;
	int freeBlockIndex = 0;


	//FCB - is divided into two blocks	
	fcb *a = malloc(sizeof(fcb)*64);
	fcb *b = malloc(sizeof(fcb)*64);
	
	getblock(1, a);
	getblock(2, b);

	//Check if the file name already exists
	for( int i = 0; i < 64; i++) {
		if( strcmp( a[i].filename, filename) == 0) {//That means a file with the same file name exists
			printf("Error, a file with the filename already exist in the disk!\n");
			result = -2;
			break;
		}
		else if( strcmp( b[i].filename, filename) == 0) {
			printf("Error, a file with the filename already exist in the disk!\n");
			result = -2;	
			break;
		}

	}
	
	if( result != -2) {
		for( int i = 0; i < 64;  i++) {
			if( a[i].startAddress == -1) {//That means it is available
				freeBlockIndex = getFreeBlock();
				a[i].startAddress = freeBlockIndex;
				strncpy(a[i].filename, filename, 32);
				result = 0;
				
				//Updating the FAT
				//Initialy only one block is allocated for the file, so other blocks exist for the table
				fatTableCached[freeBlockIndex/1024][freeBlockIndex%1024] = -2;
		
				//Putting the changes
				putblock(1,a);
				
				break;
			}
			else if( b[i].startAddress == -1) {
				freeBlockIndex = getFreeBlock();
				b[i].startAddress = freeBlockIndex;
				strncpy(b[i].filename, filename, 32);
				result = 0;
				
				//Updating the FAT
				//Initialy only one block is allocated for the file, so other blocks exist for the table
				fatTableCached[freeBlockIndex/1024][freeBlockIndex%1024] = -2;
		
				//Putting the changes
				putblock(2,b);
				
				break;
			}
		}
	}
	
	
	//free the memory
	free(a);
	free(b);

	return result; 
}


/* open file filename */
int myfs_open(char *filename)
{
	int index = -1;
	int fcbIndex = -1;
	
	//FCB - is divided into two blocks	
	fcb *a = malloc(sizeof(fcb)*64);
	fcb *b = malloc(sizeof(fcb)*64);
	
	getblock(1, a);
	getblock(2, b);

	fcb *temp_fcb = malloc( sizeof(fcb) );

	//Find the file in the FCB
	for( int i = 0; i < 64; i++) {
		if( strcmp( a[i].filename, filename) == 0) {//That means a file with the same file name exists
			strncpy( temp_fcb->filename, a[i].filename, 32);
			temp_fcb->startAddress = a[i].startAddress;
			fcbIndex = i;
			break;
		}
		else if( strcmp( b[i].filename, filename) == 0) {
			strncpy( temp_fcb->filename, a[i].filename, 32);
			temp_fcb->startAddress = a[i].startAddress;
			fcbIndex = i+64;
			break;
		}
	}

	//Check if the file is already opened
	if( fcbIndex > -1) { //then file is created and has entry in fcb
		printf("fcbIndex is %d\n", fcbIndex);
		if( system_wide_table[fcbIndex].count <= 0 ) { //then no entry in the system_wide_table yet
			//system_wide_table[fcbIndex].count = 1;
			printf("adding the fcb to system_wide\n");
			system_wide_table[fcbIndex].fcb_instance = malloc( sizeof(fcb) );
			system_wide_table[fcbIndex].fcb_instance->startAddress = temp_fcb->startAddress;
			strncpy(system_wide_table[fcbIndex].fcb_instance->filename, temp_fcb->filename, 32);
		}
		//adding the per_process_entry
		printf("adding the per_process_entry\n");

		system_wide_table[fcbIndex].count += 1;
		per_process_entry new_entry;
		new_entry.fcb_index = fcbIndex;
		new_entry.file_position = 0;
		p_insertFirst( file_descriptor_index, new_entry);
		index = file_descriptor_index;
		file_descriptor_index++;
	}

	//free
	free(temp_fcb);
       	free(a);
	free(b);


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


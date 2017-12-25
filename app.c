#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h> 
#include "myfs.h"

    

/**
  This program is a tester for partly implemented functions etc.
  It is dynamic and after the verifications of the functions is done
  those parts can be commented out
  */
//TODO delete after the program is ready
int main( int argc, char *argv[] ) {

	struct timeval t1, t2;
    double elapsedTime;

    if (myfs_diskcreate ("example") != 0 ){
        printf("could not create file %s\n","example") ;
        exit (1);
    }
    else {
        printf ("file %s created\n", "example");
    }

    myfs_makefs("example");


    myfs_mount("example");


int *input = malloc(4096*5);
	for(int index = 0; index < 4096*5/4; index++)
		input[index] = index - 5000;

	int *output = malloc(4096*5);
	gettimeofday(&t1, NULL);
	int fd0 = myfs_create("example_file0");
	// stop timer
    gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to create file 0 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	int fd1 = myfs_create("example_file1");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to create file 1 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	int fd2 = myfs_create("example_file2");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to create file 2 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	int fd3 = myfs_create("example_file3");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to opencreatefile 3 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	int fd4 = myfs_create("example_file4");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to create file 4 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);

	myfs_write(fd0, input, 4096 * 1);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to write file 0 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_write(fd1, input, 4096 * 2);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to write file 1 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_write(fd2, input, 4096 * 3);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to write file 2 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_write(fd3, input, 4096 * 4);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to write file 3 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_write(fd4, input, 4096 * 5);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to write file 4 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);

	myfs_read(fd0, output, 4096 * 1);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to read file 0 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_read(fd1, output, 4096 * 2);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to read file 1 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_read(fd2, output, 4096 * 3);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to read file 2 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_read(fd3, output, 4096 * 4);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to read file 3 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_read(fd4, output, 4096 * 5);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to read file 4 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);

	 myfs_close(fd0);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to close file 0 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_close(fd1);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to close file 1 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_close(fd2);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to close file 2 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_close(fd3);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to close file 3 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_close(fd4);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to close file 4 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);

	fd0 = myfs_open("example_file0");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to open file 0 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	fd1 = myfs_open("example_file1");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to open file 1 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	fd2 = myfs_open("example_file2");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to open file 2 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	fd3 = myfs_open("example_file3");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to open file 3 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	fd4 = myfs_open("example_file4");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to open file 4 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);

	myfs_close(fd0);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to close file 0 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_close(fd1);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to close file 1 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_close(fd2);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to close file 2 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_close(fd3);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to close file 3 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_close(fd4);
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to close file 4 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);

	myfs_delete("example_file0");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to delete file 0 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_delete("example_file1");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to delete file 1 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_delete("example_file2");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to delete file 2 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_delete("example_file3");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to delete file 3 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);
	myfs_delete("example_file4");
   gettimeofday(&t2, NULL);

// compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	printf("Passed time to delete file 4 is %lf\n", elapsedTime);
	gettimeofday(&t1, NULL);

    myfs_umount("example");


    return 0;
}

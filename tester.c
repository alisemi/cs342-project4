#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "myfs.h"

/**
  This program is a tester for partly implemented functions etc.
  It is dynamic and after the verifications of the functions is done
  those parts can be commented out
  */
//TODO delete after the program is ready
int main( int argc, char *argv[] ) {
	if (myfs_diskcreate ("example") != 0 ){
		printf("could not create file %s\n","example") ; 
		exit (1);
	}
	else {
		printf ("file %s created\n", "example"); 
	}

	return 0;
}

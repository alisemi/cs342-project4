
all: libmyfs.a  app createdisk formatdisk tester


libmyfs.a:  	myfs.c
	gcc -Wall -c myfs.c perProcessLinkedList.c
	ar -cvr  libmyfs.a myfs.o perProcessLinkedList.o
	ranlib libmyfs.a

app: 	app.c libmyfs.a
	gcc -Wall -o app app.c  -L. -lmyfs

createdisk: createdisk.c
	gcc -Wall -o createdisk createdisk.c

formatdisk: formatdisk.c
	gcc -Wall -o formatdisk formatdisk.c -L. -lmyfs

tester: tester.c libmyfs.a
	gcc -Wall -o tester tester.c -L. -lmyfs

clean: 
	rm -fr *.o *.a *~ a.out app createdisk formatdisk tester



all : doorLock 

doorLock : final.o
	gcc -o doorLock final.o
	cp doorLock /nfsroot

final.o : final.c
	gcc -c -o $@ $?

clean :
	rm -rf *.o doorLock

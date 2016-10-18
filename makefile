proj: proj.o
	cc proj.o -o proj 	
proj.o: proj.c
	cc -Wall -c proj.c
	


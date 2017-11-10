build: ip.o
	gcc ip.o -o ip -lm
ip.o: ip.c
	gcc -c ip.c -o ip.o
clean:
	rm *.o ip

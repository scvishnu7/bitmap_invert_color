CC=gcc
bmp: bmp.o
	$(CC) -o bmp bmp.o
bmp.o: bmp.c
	$(CC) -c bmp.c
clean:
	rm bmp.o bmp

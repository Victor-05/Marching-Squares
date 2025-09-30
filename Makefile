CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS=marching_squares

build: $(TARGETS)

marching_squares: main.c
	$(CC) $(CFLAGS) main.c -o marching_squares

pack:
	zip -FSr 313CA_SerbanVictor_Tema2.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean

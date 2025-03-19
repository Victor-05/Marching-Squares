CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS=marching_squares

build: $(TARGETS)

marching_squares: tema.c
	$(CC) $(CFLAGS) tema.c -o marching_squares

pack:
	zip -FSr 313CA_SerbanVictor_Tema2.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean

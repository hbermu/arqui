# Arq Makefile

all: clean image-seq image-seq-nop

CC=g++
CFLAGS=-std=c++17 -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -lstdc++fs
CFLAGSOP=-O3 -DNDEBUG
CFLAGSNOP=-O0
OBJ = image
image-seq: $(OBJ)-seq.cc
	$(CC) -o $(OBJ)-seq $(OBJ)-seq.cc $(CFLAGS) $(CFLAGSOP)

image-seq-nop: $(OBJ)-seq.cc
	$(CC) -o $(OBJ)-seq-nop $(OBJ)-seq.cc $(CFLAGS) $(CFLAGSNOP)

image-par: $(OBJ)-par.cc
	$(CC) -o $(OBJ)-par $(OBJ)-par.cc $(CFLAGS) $(CFLAGSOP) -fopenmp
clean:
	rm -f image-seq image-seq-nop images_destination/*.bmp

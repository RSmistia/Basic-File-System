
OBJ=main.o fs.o disk.o bitmap.o
HDR=fs.h disk.h bitmap.h

CFLAGS=-Wall -g 
# add -fsanitize=address to check memory errors in runtime 

all: main


main: $(OBJ) $(HDR)
	cc $(CFLAGS)  $(OBJ) -o main

clean:
	rm -f main $(OBJ) *~

#to compile: make
#to run: ./ksim
cc = g++
cflags = -Wall -g

# The name of the output executable
OUT = ksim

all:
	make $(OUT)

$(OUT): main.cpp process.cpp scan.cpp head.h
	$(cc) $(cflags) -o $(OUT) main.cpp scan.cpp process.cpp


clean:
	rm -f $(OUT)


.PHONY: all mosaic run clean

all: mosaic

mosaic: bin/mosaic.o
bin/mosaic.o: src/*.cpp
	@g++ -O3 src/*.cpp -o bin/mosaic.o

clean:
	@rm -f bin/*

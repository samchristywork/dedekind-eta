CC := gcc
LIBS := -lm

all: build/dedekind_eta

build/dedekind_eta: dedekind_eta.c
	mkdir -p build/
	${CC} dedekind_eta.c -o $@ ${LIBS}

example: build/example.tga

build/example.tga: build/dedekind_eta
	./build/dedekind_eta -x 1000 -y 1000 -n 10 > build/example.tga
	convert build/example.tga build/example.png

clean:
	rm -rf build/

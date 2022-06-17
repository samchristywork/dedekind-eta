CC := gcc

LIBS := -lm

all: build/dedekind_eta

build/dedekind_eta: dedekind_eta.c
	mkdir -p build/
	${CC} dedekind_eta.c -o $@ ${LIBS}

example:
	make
	./build/dedekind_eta -n 10 > build/example.tga
	convert build/example.tga build/example.png

clean:
	rm -rf build/

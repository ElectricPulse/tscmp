ARGS = ./assets/dir ./assets/ts

run: main
	./main ${ARGS}

debug: main
	gdb --args ./main ${ARGS}

install: main
	cp main /usr/local/bin/tscmp

main: main.c
	gcc -O0 -g -o main main.c

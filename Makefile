ARGS = ./assets/dir ./assets/ts

run: main
	./main ${ARGS}

debug: main
	gdb --args ./main ${ARGS}

main: main.c
	gcc -O0 -g -o main main.c

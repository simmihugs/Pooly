CC=clang

POOLY=./bin/pooly
sources=./src/Main.c

$(POOLY): $(sources)
	${CC} $^ -o $@

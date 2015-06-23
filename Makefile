CC = gcc
FILES = src/Disassembler.c src/jump.c src/parse.c src/similaropcodes.c src/Vector.c
OUT_EXE = Disassembler

build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES)

clean:
	rm -f *.o $(OUT_EXE)

rebuild: clean build

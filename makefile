CC=gcc
CFLAGS=-I./src/

shellmake: ./src/Shell.o ./src/HashTable.o
	${CC}  -o "/usr/bin/dsh" ./src/Shell.o  ./src/HashTable.o  

clean:
	rm -f ./src/*.o /usr/bin/dsh

doc:
	doxygen ./Docs/shell.doxyfile

cleandoc:
	rm -rf ./Docs/html

all: testsymtablelist testsymtablehash

testsymtablelist: testsymtable.o symtablelist.o
	gcc217 -g testsymtable.o symtablelist.o -o testsymtablelist

testsymtablehash: testsymtable.o symtablehash.o
	gcc217 -g testsymtable.o symtablehash.o -o testsymtablehash

testsymtable.o: testsymtable.c symtable.h
	gcc217 -g -c testsymtable.c

symtablelist.o: symtablelist.c symtable.h
	gcc217 -g -c symtablelist.c

symtablehash.o: symtablehash.c symtable.h
	gcc217 -g -c symtablehash.c

clean:
	rm -f testsymtablelist testsymtablehash testsymtable.o symtablelist.o symtablehash.o
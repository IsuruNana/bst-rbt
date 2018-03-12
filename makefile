TREEOBJS = trees.o string.o da.o cda.o queue.o scanner.o bst.o rbt.o gt.o
OOPTS = -Wall -std=c99 -c -g
LOOPTS = -Wall -std=c99 -g

all : trees

string.o : string.c string.h
	gcc $(OOPTS) string.c

da.o : da.c da.h
	gcc $(OOPTS) da.c

cda.o : cda.c cda.h
	gcc $(OOPTS) cda.c

queue.o : queue.c queue.h cda.h
	gcc $(OOPTS) queue.c

scanner.o : scanner.c scanner.h
	gcc $(OOPTS) scanner.c

bst.o : bst.c bst.h
	gcc $(OOPTS) bst.c

gt.o : gt.c bst.h gt.h
	gcc $(OOPTS) gt.c

rbt.o : rbt.c bst.h rbt.h
	gcc $(OOPTS) rbt.c

trees.o : trees.c string.h queue.h cda.h da.h scanner.h bst.h gt.h rbt.h
	gcc $(OOPTS) trees.c

trees : $(TREEOBJS)
	gcc $(LOOPTS) -o trees $(TREEOBJS)

test : trees
		@echo Test 1
		./trees -g data commands
clean :
	rm -f *.o trees

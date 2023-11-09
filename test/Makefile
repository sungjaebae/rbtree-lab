.PHONY: test

CFLAGS=-I ../src -Wall -g -DSENTINEL
CC := clang

test: test-rbtree
	./test-rbtree
	valgrind ./test-rbtree

test-rbtree: test-rbtree.o ../src/rbtree.o

test-rbtree.o :  test-rbtree.c ../src/rbtree.h
	$(CC) $(CFLAGS) -c $< -o $@

../src/rbtree.o:
	$(MAKE) -C ../src rbtree.o

clean:
	rm -f test-rbtree *.o

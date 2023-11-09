.PHONY: clean

CFLAGS=-Wall -g
CC := clang

driver: driver.o rbtree.o

rbtree.o :  rbtree.c rbtree.h stack.o
	$(CC) $(CFLAGS) -c $< -o $@

stack.o : stack.c stack.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f driver *.o

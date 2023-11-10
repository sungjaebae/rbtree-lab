.PHONY: test

CFLAGS=-I ../src -Wall -g -DSENTINEL
CC := clang
OBJS = test-rbtree.o ../src/rbtree.o ../src/stack_base.o ../src/stack_node_t.o

test: test-rbtree
	./test-rbtree
	valgrind ./test-rbtree

test-rbtree: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

../src/rbtree.o:
	$(MAKE) -C ../src rbtree.o
../src/stack_base.o: 
	$(MAKE) -C ../src stack_base.o
../src/stack_node_t.o:
	$(MAKE) -C ../src stack_node_t.o

clean:
	rm -f test-rbtree *.o *.d

-include $(OBJS:.o=.d)

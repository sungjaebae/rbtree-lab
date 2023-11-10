.PHONY: clean

CFLAGS=-Wall -g
CC := clang
OBJS = driver.o rbtree.o stack_base.o stack_node_t.o

driver: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -MD

clean:
	rm -f driver *.o *.d

-include $(OBJS:.o=.d)

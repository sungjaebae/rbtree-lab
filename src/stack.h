#ifndef _STACK_H_
#define _STACK_H_
#include <stdbool.h>
#include <stdlib.h>
typedef void *stack_data_ptr;

typedef struct stack_node_t {
  stack_data_ptr data;
  struct stack_node_t *next;
} stack_node_t;

typedef struct {
  stack_node_t *root;
} Stack;

void stack_push(Stack *stack, stack_data_ptr data);
stack_data_ptr *stack_pop(Stack *stack);
stack_data_ptr *stack_peek(const Stack *stack);
bool stack_is_empty(const Stack *stack);
Stack *make_stack();
#endif // _STACK_H_

#ifndef _STACK_BASE_H_
#define _STACK_BASE_H_
#include <stdbool.h>
#include <stdlib.h>

typedef struct stack_node_t {
  void *data;
  struct stack_node_t *next;
} stack_node_t;

typedef struct {
  stack_node_t *root;
} Stack;

void stack_push(Stack *stack, void *data, void *(*init_data)(void *));
void *return_ptr(void *data);
void stack_push_ptr(Stack *stack, void *data);

void *stack_pop(Stack *stack);
void *stack_peek(const Stack *stack);
bool stack_is_empty(const Stack *stack);
size_t stack_length(const Stack *stack);
Stack *make_stack();
void soft_delete_stack(Stack *stack);
#endif // _STACK_BASE_H_

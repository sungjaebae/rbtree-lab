#ifndef _STACK_TUPLE_H_
#define _STACK_TUPLE_H_
#include "rbtree.h"
#include "stack_base.h"

void *init_stack_node_t(void *data);

void stack_push_literal(Stack *stack, node_t data);

void print_stack(Stack *stack);

void print_node_t(node_t *data);

void print_array(const key_t array[], size_t array_size);

void hard_delete_stack(Stack *stack);
#endif // _STACK_TUPLE_H_

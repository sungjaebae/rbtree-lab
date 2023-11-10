#include "stack_node_t.h"
#include <stdio.h>

void *init_stack_node_t(void *data) {
  node_t *new = (node_t *)calloc(1, sizeof(node_t));
  *new = *(node_t *)data;
}

void stack_push_literal(Stack *stack, node_t data) {
  stack_push(stack, &data, init_stack_node_t);
}

void print_stack(Stack *stack) {
  size_t length = stack_length(stack);
  stack_node_t *cur = stack->root;
  if (length == 0) {
    printf("[stack]: NULL\n");
    return;
  }

  printf("[stack]: ");
  for (size_t i = 0; i < length; i++) {
    node_t *data = (node_t *)cur->next->data;
    printf("%d(%c)%c", data->key, (data->color == RBTREE_BLACK) ? 'K' : 'R',
           (i < length - 1) ? ' ' : '\n');
    cur = cur->next;
  }
}

void print_node_t(node_t *data) {
  printf("%d(%c) ", data->key, (data->color == RBTREE_BLACK) ? 'K' : 'R');
  return;
}

void print_array(const key_t array[], size_t array_size) {
  if (array_size == 0) {
    printf("array: empty\n");
    return;
  }
  printf("array: ");
  for (int i = 0; i < array_size; i++) {
    printf("%d%c", array[i], (i < array_size - 1) ? ' ' : '\n');
  }
}

void hard_delete_stack(Stack *stack) {
  size_t len = stack_length(stack);
  for (int i = 0; i < len; i++) {
    node_t *data = (node_t *)stack_pop(stack);
    free(data);
  }
  free(stack->root);
  free(stack);
}
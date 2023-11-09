#include "stack.h"

// 할일 : stack 변경된거 테스트하고, 여기에 머지해서 구현하기

void stack_push(Stack *stack, stack_data_ptr data) {
  stack_node_t *new = (stack_node_t *)calloc(1, sizeof(stack_node_t));
  new->data = data;
  new->next = stack->root->next;
  stack->root->next = new;
}
stack_data_ptr *stack_pop(Stack *stack) {
  if (!stack->root->next)
    return NULL;
  stack_data_ptr *ret = &stack->root->next->data;
  stack_node_t *next = stack->root->next->next;
  free(stack->root->next);
  stack->root->next = next;
  return ret;
}
stack_data_ptr *stack_peek(const Stack *stack) {
  stack_data_ptr *ret = &stack->root->next->data;
  return ret;
}

bool stack_is_empty(const Stack *stack) { return stack->root->next == NULL; }

Stack *make_stack() {
  Stack *stack = malloc(sizeof(Stack));
  stack->root = (stack_node_t *)calloc(1, sizeof(stack_node_t));
  stack->root->next = NULL;
  return stack;
}
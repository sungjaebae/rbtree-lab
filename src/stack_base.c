#include "stack_base.h"
// 장기적으로 heap 메모리만 사용하도록 만든 코드

void stack_push(Stack *stack, void *data, void *(*init_data)(void *)) {
  stack_node_t *new = (stack_node_t *)calloc(1, sizeof(stack_node_t));
  new->data = init_data(data);
  new->next = stack->root->next;
  stack->root->next = new;
}

void *return_ptr(void *data) { return data; }

void stack_push_ptr(Stack *stack, void *data) {
  stack_push(stack, data, return_ptr);
}

void *stack_pop(Stack *stack) {
  if (!stack->root->next)
    return NULL;
  void *ret = stack->root->next->data;
  stack_node_t *next = stack->root->next->next;
  free(stack->root->next);
  stack->root->next = next;
  return ret;
}

void *stack_peek(const Stack *stack) {
  void *ret = stack->root->next->data;
  return ret;
}

bool stack_is_empty(const Stack *stack) { return stack->root->next == NULL; }

size_t stack_length(const Stack *stack) {
  int i = 0;
  stack_node_t *cur = stack->root;
  while (cur->next != NULL) {
    i++;
    cur = cur->next;
  }
  return i;
}

Stack *make_stack() {
  Stack *stack = malloc(sizeof(Stack));
  stack->root = (stack_node_t *)calloc(1, sizeof(stack_node_t));
  stack->root->next = NULL;
  stack->root->data = NULL;
  return stack;
}

// 스택의 stack_node_t들과 스택 자체를 없앤다. data는 지우지 않는다.
void soft_delete_stack(Stack *stack) {
  size_t len = stack_length(stack);
  for (int i = 0; i < len; i++) {
    stack_pop(stack);
  }
  free(stack->root);
  free(stack);
}
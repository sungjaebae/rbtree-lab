#include "rbtree.h"
#include "stack_node_t.h"
#include "stdio.h"
#include <stdlib.h>

void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
void rb_insert_fixup(rbtree *t, node_t *z);
void inorder(const rbtree *t, key_t *arr, const size_t n);
void rb_transplant(rbtree *t, node_t *u, node_t *v);
void rb_erase_fixup(rbtree *t, node_t *z);
node_t *rbtree_min_from(const rbtree *t, node_t *pos);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  Stack *stack = make_stack();
  Stack *stack_for_delete = make_stack();
  node_t *cur = t->root;
  while (cur != t->nil || !stack_is_empty(stack)) {
    while (cur != t->nil) {
      stack_push_literal(stack, cur);
      cur = cur->left;
    }
    cur = stack_pop(stack);
    stack_push_literal(stack_for_delete, cur);
    cur = cur->right;
  }
  soft_delete_stack(stack);
  print_stack(stack_for_delete);
  hard_delete_stack(stack_for_delete);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  node_t *y = t->nil;
  node_t *x = t->root;
  while (x != t->nil) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  z->parent = y;
  if (y == t->nil) {
    t->root = z;
  } else if (z->key < y->key) {
    y->left = z;
  } else {
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rb_insert_fixup(t, z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *x = t->root;
  while (x != t->nil && key != x->key) {
    if (key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  if (x == t->nil)
    return NULL;
  return x;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;
  while (x->left != t->nil) {
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;
  while (x->right != t->nil) {
    x = x->right;
  }
  return x;
}

int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z;
  node_t *x = t->nil;
  color_t y_original_color = y->color;
  if (z->left == t->nil) {
    x = z->right;
    rb_transplant(t, z, z->right);
  } else if (z->right == t->nil) {
    x = z->left;
    rb_transplant(t, z, z->left);
  } else {
    y = rbtree_min_from(t, z->right);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z) {
      x->parent = y;
    } else {
      rb_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rb_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_original_color == RBTREE_BLACK) {
    rb_erase_fixup(t, x);
  }
  free(z);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  Stack *stack = make_stack();
  node_t *cur = t->root;
  for (size_t i = 0; i < n; i++) {
    while (cur != t->nil) {
      stack_push_literal(stack, cur);
      cur = cur->left;
    }
    cur = stack_pop(stack);
    arr[i] = cur->key;
    cur = cur->right;
  }
  soft_delete_stack(stack);
  return 0;
}

/* ===================================
      내부에서 사용하는 함수 구현
   ===================================
*/

void *init_data_node_t(void *args) {
  node_t *data = calloc(1, sizeof(node_t));
  *data = *(node_t *)args;
  return data;
}

void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}
void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->right = x;
  x->parent = y;
}
void rb_insert_fixup(rbtree *t, node_t *z) {
  while (z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    } else {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

node_t *rbtree_min_from(const rbtree *t, node_t *pos) {
  node_t *x = pos;
  while (x->left != t->nil) {
    x = x->left;
  }
  return x;
}

void rb_erase_fixup(rbtree *t, node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    } else {
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

void print_rbtree(const rbtree *t, bool detail) {
  Stack *stack = make_stack();
  node_t *cur = t->root;
  printf("[rbtree]: ");
  while (cur != t->nil || !stack_is_empty(stack)) {
    while (cur != t->nil) {
      stack_push_literal(stack, cur);
      cur = cur->left;
    }
    cur = stack_pop(stack);
    if (!detail) {
      print_node_t(cur);
    } else {
      printf("%d(%c)[p->%d r->%d l->%d] ", cur->key,
             (cur->color == RBTREE_BLACK) ? 'K' : 'R',
             (cur->parent == t->nil) ? -1 : cur->parent->key,
             (cur->right == t->nil) ? -1 : cur->right->key,
             (cur->left == t->nil) ? -1 : cur->left->key);
    }
    cur = cur->right;
  }
  printf("\n");
  soft_delete_stack(stack);
}
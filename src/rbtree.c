#include "rbtree.h"
#include <stdbool.h>
#include <stdlib.h>

void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
void rb_insert_fixup(rbtree *t, node_t *z);
void inorder(const rbtree *t, key_t *arr, const size_t n);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
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

int rbtree_erase(rbtree *t, node_t *p) {
  if (p->parent == t->nil) {
    t->root = t->nil;
  }
  if (p->parent->left == p) {
    p->parent->left = t->nil;
  }
  if (p->parent->right == p) {
    p->parent->right = t->nil;
  }
  free(p);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  inorder(t, arr, 0);
  return 0;
}

/* ===================================
      내부에서 사용하는 함수 구현
   ===================================
*/

void inorder(const rbtree *t, key_t *arr, size_t n) {
  Stack *stack = make_stack();
  node_t *cur = t->root;
  size_t i = 0;
  while (!stack_is_empty(stack) || cur != t->nil) {
    while (cur != t->nil) {
      stack_push(stack, *cur);
      cur = cur->left;
    }
    cur = stack_pop(stack);
    arr[i] = cur->key;
    cur = cur->right;
  }

  free(stack);
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

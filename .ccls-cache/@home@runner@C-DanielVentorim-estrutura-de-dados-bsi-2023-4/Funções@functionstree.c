#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// funções da árvore avl
int max(int a, int b) {
  return (a > b) ? a : b;
} // recebe o a e o b como entrada e retorna o maior deles

int height(Node *node) {
  if (node == NULL) {
    return 0;
  }
  return node->height;
} // verifica a altura da arvore

Node *new_node(Item item) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->item = item;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;
  return node;
} // cria um novo nó

Node *right_rotate(Node *y) {
  Node *x = y->left;
  Node *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;

  return x;
} // função de rotação a direita

Node *left_rotate(Node *x) {
  Node *y = x->right;
  Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;

  return y;
} // função de rotação a esquerda

int get_balance(Node *node) {
  if (node == NULL) {
    return 0;
  }
  return height(node->left) - height(node->right);
} // balanceia a arvore

Node *insert(Node *node, Item item) {
  if (node == NULL) {
    return new_node(item);
  }

  if (item.id < node->item.id) {
    node->left = insert(node->left, item);
  } else if (item.id > node->item.id) {
    node->right = insert(node->right, item);
  } else {
    return node;
  }

  node->height = 1 + max(height(node->left), height(node->right));

  int balance = get_balance(node);

  if (balance > 1 && item.id < node->left->item.id) {
    return right_rotate(node);
  }

  if (balance < -1 && item.id > node->right->item.id) {
    return left_rotate(node);
  }

  if (balance > 1 && item.id > node->left->item.id) {
    node->left = left_rotate(node->left);
    return right_rotate(node);
  }

  if (balance < -1 && item.id < node->right->item.id) {
    node->right = right_rotate(node->right);
    return left_rotate(node);
  }

  return node;
} // função de inserir item por id e armazenamento desse id

Node *search(Node *root, int id, int *count) {
  if (root == NULL || root->item.id == id) {
    return root;
  }
  (*count)++;

  if (root->item.id < id) {
    return search(root->right, id, count);
  }

  return search(root->left, id, count);
} // função de produra de item por id

Node *load_items_and_build_avl_tree() {
  FILE *file = fopen("itens.bin", "rb");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo de itens.\n");
    return NULL;
  }

  Node *root = NULL;
  Item item;

  while (fread(&item, sizeof(Item), 1, file) == 1) {
    root = insert(root, item);
  }

  fclose(file);
  return root;
} // lê o arquivo binário de passa para árvore
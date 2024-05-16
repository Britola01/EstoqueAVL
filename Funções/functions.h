#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

void salvar_empresa(Empresa *empresa);
void salvar_item(Item *item);
void salvar_compra(Compra *compra);
void salvar_venda(Venda *venda);
int max(int a, int b);
int height(Node *node);
Node *new_node(Item item);
Node *right_rotate(Node *y);
Node *left_rotate(Node *x);
int get_balance(Node *node);
Node *insert(Node *node, Item item);
Node *search(Node *root, int id, int *count);
Node *load_items_and_build_avl_tree();
int item_exists(Node *root, int id, char *nome);
void vender_produto(Node *root);
void comprar_produto(Node *root);
void cadastrar_empresa(Empresa *empresa);
void cadastrar_item(Node **root);
void pesquisar_empresa();
void pesquisar_todas_compras();
void pesquisar_compras(Node *root);
void extratomovimento(Node **root);
void pesquisar_vendas(Node *root);
void pesquisar_produto(Node *root);
void relatorio_vendas_mes();
int comparar_itens_por_nome(const void *a, const void *b);
void gerar_relatorio_produtos();
void excluir_item(const char *filename, int id);
void excluir_item_por_id(Node **root);
void alteracoesmenu();
void alterar_item(Node *root);
void exibir_menu();
#endif

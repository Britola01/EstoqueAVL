#ifndef STRUCTS_H
#define STRUCTS_H
typedef struct {
  int dia;
  int mes;
  int ano;
} Data;

typedef struct {
  char cnpj[20];
  char nome[100];
  char cidade[100];
} Empresa;

typedef struct {
  int id;
  char nome[100];
  float custo;
  float icms;
  float despesa_operacional;
  float lucratividade;
  float preco_venda;
  int quantidade;
} Item;

typedef struct {
  int nota_fiscal;
  Data data;
  int id_produto;
  float preco_custo;
  float imposto;
  int quantidade;
  float valor;
  Item item;
} Compra;

typedef struct {
  int nota_fiscal;
  Data data;
  int id_produto;
  float preco_venda;
  float imposto;
  int quantidade;
  float valor;
  float venda;
  Item item;
} Venda;

typedef struct Node {
  Item item;
  int data;
  struct Node *left;
  struct Node *right;
  int height;
} Node;
#endif
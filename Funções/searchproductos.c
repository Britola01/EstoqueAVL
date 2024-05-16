#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "structs.h"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"
#define CYAN "\x1b[36m"
#define GREEN "\x1b[33m"
#define MAGENTA "\x1b[35m"

void pesquisar_empresa() {
  Empresa empresa;
  FILE *file = fopen("empresa.bin", "rb");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo de empresas.\n");
    return;
  }

  fread(&empresa, sizeof(Empresa), 1, file);
  fclose(file);

  printf("Nome: %s\n", empresa.nome);
  printf("CNPJ: %s\n", empresa.cnpj);
  printf("Cidade: %s\n", empresa.cidade);
}

void pesquisar_todas_compras() {
  FILE *file = fopen("compras.bin", "rb");
  if (file == NULL) {
    printf(RED "Erro ao abrir arquivo de compras.\n" RESET);
    return;
  }

  Compra compra;
  while (fread(&compra, sizeof(Compra), 1, file) == 1) {
    printf(MAGENTA "Pesquisa de Compra do item: %d\n" RESET, compra.id_produto);
    printf(CYAN "ID: %d\n", compra.nota_fiscal);
    printf("Data: %02d/%02d/%04d\n", compra.data.dia, compra.data.mes,
           compra.data.ano);
    printf("ID Produto: %d\n", compra.id_produto);
    printf("Quantidade: %d\n", compra.quantidade);
    printf("Preço de Custo: %.2f\n", compra.preco_custo);
    printf("Preço Total: %.2f\n\n" RESET, compra.valor);
  }

  fclose(file);
  printf(GREEN "Pesquisa de compras concluída.\n" RESET);
}

void pesquisar_compras(Node *root) {
  int count = 0;
  FILE *file = fopen("compras.bin", "rb");
  if (file == NULL) {
    printf(RED "Erro ao abrir arquivo de compras.\n" RESET);
    return;
  }
  Item item;
  FILE *file2 = fopen("itens.bin", "rb+");
  if (file2 == NULL) {
    printf(RED "Erro ao abrir o arquivo itens.bin.\n" RESET);
    return;
  }
  while (fread(&item, sizeof(Item), 1, file2) == 1) {
    printf(CYAN "----------------------------------------\n");
    printf(MAGENTA "Item de id: %d\n" RESET, item.id);
    printf(CYAN "ID: %d\n", item.id);
    printf("Nome: %s\n", item.nome);
    printf("Quantidade: %d\n", item.quantidade);
    printf("---------------------------------------\n");
  }

  int id_produto, mes, ano;
  printf(CYAN "Digite o ID do produto: " RESET);
  scanf("%d", &id_produto);
  Node *produto = search(root, id_produto, &count);
  if (produto == NULL) {
    printf(RED "Produto não encontrado.\n" RESET);
    return;
  }
  printf(CYAN "Digite o mês desejado (MM): " RESET);
  scanf("%d", &mes);
  printf(CYAN "Digite o ano desejado (AAAA): " RESET);
  scanf("%d", &ano);

  Compra compra;
  while (fread(&compra, sizeof(Compra), 1, file) == 1) {
    if (compra.id_produto == id_produto && compra.data.mes == mes &&
        compra.data.ano == ano) {
      printf(MAGENTA "\nPesquisa de Compra do item: %d\n" RESET,
             compra.id_produto);
      printf(CYAN "Nota Fiscal: %d\n", compra.nota_fiscal);
      printf("Data: %02d/%02d/%04d\n", compra.data.dia, compra.data.mes,
             compra.data.ano);
      printf("ID Produto: %d\n", compra.id_produto);
      printf("Quantidade: %d\n", compra.quantidade);
      printf("Preço Total: %.2f\n\n", compra.valor);
      printf("Passou por %d elementos até chegar no id %d\n" RESET, count,
             compra.id_produto);
    }
  }

  fclose(file);
  printf(GREEN "Pesquisa de compras concluída.\n" RESET);
}

void pesquisar_vendas(Node *root) {
  int count = 0;
  FILE *file = fopen("vendas.bin", "rb");
  if (file == NULL) {
    printf(RED "Erro ao abrir o arquivo de vendas.\n" RESET);
    return;
  }
  Item item;
  FILE *file2 = fopen("itens.bin", "rb+");
  if (file2 == NULL) {
    printf(RED "Erro ao abrir o arquivo itens.bin.\n" RESET);
    return;
  }
  while (fread(&item, sizeof(Item), 1, file2) == 1) {
    printf(CYAN "----------------------------------------\n");
    printf(MAGENTA "Item de id: %d\n" RESET, item.id);
    printf(CYAN "ID: %d\n", item.id);
    printf("Nome: %s\n", item.nome);
    printf("Quantidade: %d\n", item.quantidade);
    printf("---------------------------------------\n");
  }

  int id_produto;
  printf(CYAN "Digite o ID do produto: " RESET);
  scanf("%d", &id_produto);
  Node *produto = search(root, id_produto, &count);
  if (produto == NULL) {
    printf(RED "Produto não encontrado.\n" RESET);
    return;
  }
  int mes, ano;
  printf(CYAN "Digite o mês: " RESET);
  scanf("%d", &mes);
  printf(CYAN "Digite o ano: " RESET);
  scanf("%d", &ano);

  printf(MAGENTA "Pesquisa de vendas do produto %d no mês %d/%d\n\n" RESET,
         id_produto, mes, ano);

  Venda venda;
  while (fread(&venda, sizeof(Venda), 1, file) == 1) {
    if (venda.id_produto == id_produto && venda.data.mes == mes &&
        venda.data.ano == ano) {
      printf(CYAN "Nota fiscal: %d\n", venda.nota_fiscal);
      printf("Data: %02d/%02d/%04d\n", venda.data.dia, venda.data.mes,
             venda.data.ano);
      printf("ID do produto: %d\n", venda.id_produto);
      printf("Preço de venda: %.2f\n", venda.preco_venda);
      printf("Imposto: %.2f\n", venda.imposto);
      printf("Quantidade: %d\n", venda.quantidade);
      printf("Passou por %d elementos até chegar no id %d\n" RESET, count,
             venda.id_produto);
      printf("Valor: %.2f\n\n" RESET, venda.valor);
    }
  }

  fclose(file);
  printf(GREEN "Pesquisa de vendas concluída.\n" RESET);
}

void pesquisar_produto(Node *root) {
  Item item;
  int count = 0;
  FILE *file = fopen("itens.bin", "rb");
  if (file == NULL) {
    printf(RED "Erro ao abrir arquivo de compras.\n" RESET);
    return;
  }
  while (fread(&item, sizeof(Item), 1, file) == 1) {
    printf(CYAN "----------------------------------------\n");
    printf(MAGENTA "Item de id: %d\n" RESET, item.id);
    printf(CYAN "ID: %d\n", item.id);
    printf("Nome: %s\n", item.nome);
    printf("Quantidade: %d\n", item.quantidade);
    printf("---------------------------------------\n");
  }

  int id;
  printf(CYAN "Digite o ID do produto que deseja pesquisar: " RESET);
  scanf("%d", &id);

  Node *produto = search(root, id, &count);
  if (produto == NULL) {
    printf(RED "Produto não encontrado.\n" RESET);
    return;
  }

  printf(MAGENTA "Relatório do Produto\n\n" RESET);
  printf(CYAN "ID: %d\n", produto->item.id);
  printf("Nome: %s\n", produto->item.nome);
  printf("Custo: %.2f\n", produto->item.custo);
  printf("ICMS: %.2f\n", produto->item.icms);
  printf("Despesa operacional: %.2f\n", produto->item.despesa_operacional);
  printf("Lucratividade: %.2f\n", produto->item.lucratividade);
  printf("Preço de venda: %.2f\n", produto->item.preco_venda);
  printf("Quantidade em estoque: %d\n", produto->item.quantidade);
  printf("Passou por %d elementos até chegar no id %d\n" RESET, count, item.id);

  printf(GREEN "Relatório do produto gerado com sucesso!\n" RESET);
}

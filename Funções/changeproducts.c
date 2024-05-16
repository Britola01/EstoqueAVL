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

void excluir_item(const char *filename, int id) {
  FILE *originalFile = fopen(filename, "rb");
  if (!originalFile) {
    printf("Não foi possível abrir o arquivo %s\n", filename);
    return;
  }

  FILE *temporaryFile = fopen("temporary.bin", "wb");
  if (!temporaryFile) {
    printf("Não foi possível criar o arquivo temporário\n");
    fclose(originalFile);
    return;
  }

  Item item;
  while (fread(&item, sizeof(Item), 1, originalFile)) {
    if (item.id != id) {
      fwrite(&item, sizeof(Item), 1, temporaryFile);
    }
  }

  fclose(originalFile);
  fclose(temporaryFile);

  // Substitua o arquivo original pelo arquivo temporário
  remove(filename);
  rename("temporary.bin", filename);
}

void excluir_item_por_id(Node **root) {
  Item item;
  FILE *file = fopen("itens.bin", "r+b");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo itens.bin.\n");
    return;
  }
  while (fread(&item, sizeof(Item), 1, file) == 1) {
    printf(CYAN "----------------------------------------\n");
    printf(MAGENTA "Item de id: %d\n" RESET, item.id);
    printf(CYAN "ID: %d\n", item.id);
    printf("Nome: %s\n", item.nome);
    printf("---------------------------------------\n");
  }
  int id;
  printf(CYAN "Digite o ID do item que deseja excluir: " RESET);
  scanf("%d", &id);

  FILE *compPtr = fopen("compras.bin", "rb+");
  FILE *vendasPtr = fopen("vendas.bin", "rb+");
  Compra compra;
  Venda venda;

  while (fread(&compra, sizeof(Compra), 1, compPtr) ||
         fread(&venda, sizeof(Venda), 1, vendasPtr) == 1) {
    if (compra.id_produto == id || venda.id_produto == id) {
      printf(RED "\nNão é possivel alterar esse produto, pois já houveram "
                 "movimentações nele\n" RESET);
      return;
    }
  }
  fclose(vendasPtr);
  fclose(compPtr);

  excluir_item("itens.bin", id);
  printf(GREEN "Item excluído com sucesso.\n" RESET);
}

void alteracoesmenu() {
  printf(CYAN "||__________________________||\n");
  printf("|| 1. Nome                  ||\n");
  printf("|| 2. Custo                 ||\n");
  printf("|| 3. ICMS                  ||\n");
  printf("|| 4. Despesa Operacional   ||\n");
  printf("|| 5. %% Lucratividade       ||\n" RESET);
  printf(RED "|| 0. Sair                  ||\n");
  printf(CYAN "||__________________________||\n");
  printf("  DIGITE A OPÇÃO:" RESET);
}

void alterar_item(Node *root) {
  Item item;
  int opcao;
  int id;
  int count = 0;
  FILE *file = fopen("itens.bin", "r+b");
  if (file == NULL) {
    printf(RED "Erro ao abrir o arquivo itens.bin.\n" RESET);
    return;
  }
  while (fread(&item, sizeof(Item), 1, file) == 1) {
    printf(CYAN "----------------------------------------\n");
    printf(MAGENTA "Item de id: %d\n" RESET, item.id);
    printf(CYAN "ID: %d\n", item.id);
    printf("Nome: %s\n", item.nome);
    printf("---------------------------------------\n");
  }
  printf(CYAN "Digite o ID do produto que deseja alterar: " RESET);
  scanf("%d", &id);

  Node *produto = search(root, id, &count);
  if (produto == NULL) {
    printf(RED "Produto não encontrado.\n" RESET);
    return;
  }
  FILE *compPtr = fopen("compras.bin", "rb");
  FILE *vendasPtr = fopen("vendas.bin", "rb");
  Compra compra;
  Venda venda;

  while (fread(&compra, sizeof(Compra), 1, compPtr) ||
         fread(&venda, sizeof(Venda), 1, vendasPtr) == 1) {
    if (compra.id_produto == id || venda.id_produto == id) {
      printf(RED "\nNão é possivel alterar esse produto, pois já houveram "
                 "movimentações nele\n" RESET);
      return;
    }
  }
  fclose(vendasPtr);
  fclose(compPtr);

  alteracoesmenu();
  scanf("%d", &opcao);
  while (opcao != 0) {
    switch (opcao) {
    case 1:
      printf(CYAN "\nDigite o novo nome: " RESET);
      scanf(" %s", produto->item.nome);
      break;
    case 2:
      printf(CYAN "\nDigite o novo custo: " RESET);
      scanf("%f", &produto->item.custo);
      break;
    case 3:
      printf(CYAN "\nDigite o novo icsm: " RESET);
      scanf("%f", &produto->item.icms);
      break;
    case 4:
      printf(CYAN "\nDigite o novo custo operacional: " RESET);
      scanf("%f", &produto->item.icms);
      break;
    case 5:
      printf(CYAN "\nDigite o novo lucro: " RESET);
      scanf("%f", &produto->item.lucratividade);
      break;
    default:
      printf("Opção inválida !\n");
      break;
    }
    alteracoesmenu();
    scanf("%d", &opcao);
  }
  produto->item.preco_venda =
      produto->item.custo /
      (1 - (produto->item.despesa_operacional + produto->item.icms +
            produto->item.lucratividade) /
               100);

  fseek(file, (id - 1) * sizeof(Item), SEEK_SET);
  fwrite(&produto->item, sizeof(Item), 1, file);
  fclose(file);
  printf(GREEN "\nAlterações feitas com sucesso !\n\n" RESET);
}
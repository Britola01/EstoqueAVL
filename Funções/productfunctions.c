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

int comparar_itens_por_nome(const void *a, const void *b) {
  Item *item1 =
      (Item *)a; // Converte o ponteiro 'a' em um ponteiro para Item (struct)
  Item *item2 =
      (Item *)b; // Converte o ponteiro 'b' em um ponteiro para Item (struct)
  return strcmp(item1->nome, item2->nome);
  // strcmp é usado para comparar as strings dos campos nome de item1 e item2. A
  // função strcmp retorna um valor negativo se item1->nome for menor que
  // item2->nome, um valor positivo se item1->nome for maior que item2->nome, e
  // zero se forem iguais.
}

int item_exists(Node *root, int id, char *nome) {

  if (root ==
      NULL) { // função para verificar se o item ja existe no estoque ou não
    return 0;
  }

  if (root->item.id == id || strcmp(root->item.nome, nome) == 0) {
    // verifica se o id atuam que a raiz está é o id sendo procurado e logo após
    // compara eles usando o strcmp e retorna a 0 se elas forem iguais
    return 1;
  }

  if (id < root->item.id) {
    return item_exists(root->left, id, nome);
  } else {
    return item_exists(root->right, id, nome);
  }
}

void vender_produto(Node *root) {
  int id;
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

  printf(CYAN "Digite o ID do produto: " RESET);
  scanf("%d", &id);
  int count = 0;
  Node *produto = search(root, id, &count);
  if (produto == NULL) {
    printf(RED "Produto não encontrado.\n" RESET);
    return;
  }

  int quantidade;
  printf(CYAN "Digite a quantidade vendida: " RESET);
  scanf("%d", &quantidade);

  if (quantidade > produto->item.quantidade) {
    printf(RED "Quantidade insuficiente em estoque.\n" RESET);
    return;
  }

  Venda venda;
  venda.nota_fiscal = rand();
  printf(CYAN "Digite a data da venda (dd/mm/aaaa): " RESET);
  scanf("%02d/%02d/%04d", &venda.data.dia, &venda.data.mes, &venda.data.ano);
  venda.id_produto = id;
  venda.preco_venda = produto->item.preco_venda;
  venda.imposto = produto->item.icms;
  venda.quantidade = quantidade;
  venda.venda = venda.preco_venda;
  venda.valor = venda.preco_venda * venda.quantidade;

  produto->item.quantidade -= quantidade;
  salvar_venda(&venda);
  FILE *file = fopen("itens.bin", "rb+");
  if (file == NULL) {
    printf(RED "Erro ao abrir o arquivo" RESET);
    return;
  }
  fseek(file, (produto->item.id - 1) * sizeof(Item), SEEK_SET);
  fwrite(&produto->item, sizeof(Item), 1, file);
  fclose(file);
  printf(GREEN "Venda realizada com sucesso.\n" RESET);
}

void comprar_produto(Node *root) {
  int id_produto;
  int count = 0;
  int aux = 0;
  int quantidade;
  Compra compra;
  Item item;
  FILE *file = fopen("itens.bin", "rb+");
  if (file == NULL) {
    printf(RED "Erro ao abrir o arquivo itens.bin.\n" RESET);
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

  printf(CYAN "Digite o ID do produto: " RESET);
  scanf("%d", &id_produto);
  Node *produto = search(root, id_produto, &count);
  if (produto == NULL) {
    printf(RED "Produto não encontrado.\n" RESET);
    return;
  }

  printf(CYAN "Digite a quantidade comprada: " RESET);
  scanf("%d", &quantidade);

  // Verifica se deseja alterar o custo do produto
  float novo_custo;
  printf(CYAN "Digite o novo custo do produto: " RESET);
  scanf("%f", &novo_custo);

  // Calcula o novo custo ponderado
  float novo_custo_total = (((produto->item.custo * produto->item.quantidade) +
                             (quantidade * novo_custo)) /
                            (produto->item.quantidade + quantidade));

  compra.nota_fiscal = rand();
  printf(CYAN "Digite a data da compra (dd/mm/aaaa): " RESET);
  scanf("%02d/%02d/%04d", &compra.data.dia, &compra.data.mes, &compra.data.ano);
  compra.quantidade = quantidade;
  compra.preco_custo = novo_custo;
  compra.valor = compra.preco_custo * compra.quantidade;
  compra.id_produto = id_produto;
  produto->item.quantidade += quantidade;
  produto->item.custo = novo_custo_total;
  produto->item.preco_venda =
      produto->item.custo /
      ((1 - (produto->item.icms + produto->item.lucratividade +
             produto->item.despesa_operacional) /
                100));

  salvar_compra(&compra);
  FILE *file2 = fopen("itens.bin", "rb+");
  if (file == NULL) {
    printf(RED "Erro ao abrir o arquivo itens.bin.\n" RESET);
    return;
  }
  fseek(file2, (produto->item.id - 1) * sizeof(Item), SEEK_SET);
  fwrite(&produto->item, sizeof(Item), 1, file2);
  fclose(file2);
  printf(GREEN "Compra realizada com sucesso.\n" RESET);
}

void cadastrar_empresa(Empresa *empresa) {
  printf(CYAN "Digite o nome da empresa:\n" RESET);
  scanf(" %s", empresa->nome);
  printf(CYAN "Digite a cidade da empresa:\n" RESET);
  scanf(" %s", empresa->cidade);
  printf(CYAN "Digite o CNPJ da empresa(00.000.000/0000-00): \n" RESET);
  scanf("%s", empresa->cnpj);

  salvar_empresa(empresa);
  printf(GREEN "Empresa cadastrada com sucesso!\n" RESET);
}

void cadastrar_item(Node **root) {
  // função para cadastrar item, utiliza a função insert como parametro dentro
  // da função para inserir os ids
  int Icms;
  Item item;
  int id_exists, name_exists;
  item.quantidade = 0;
  item.custo = 0;

  do {
    printf(CYAN "Digite o ID do item: " RESET);
    scanf("%d", &item.id);
    printf(CYAN "Digite o nome do item: " RESET);
    scanf(" %[^\n]s", item.nome);

    id_exists = item_exists(*root, item.id, "");
    name_exists = item_exists(*root, -1 , item.nome);

    if (id_exists && name_exists) {
      printf(MAGENTA "ID e nome já existem. Por favor, altere ambos.\n");
    } else if (id_exists) {
      printf("ID já existe. Por favor, altere o ID.\n");
    } else if (name_exists) {
      printf("Nome já existe. Por favor, altere o nome.\n" RESET);
    }
  } while (id_exists || name_exists);

  printf(CYAN "Digite o ICMS do item: " RESET);
  scanf("%f", &item.icms);
  printf(CYAN "Digite a despesa operacional do item: " RESET);
  scanf("%f", &item.despesa_operacional);
  printf(CYAN "Digite a lucratividade do item: " RESET);
  scanf("%f", &item.lucratividade);

  item.preco_venda =
      item.custo /
      ((1 - (item.icms + item.lucratividade + item.despesa_operacional) / 100));
  salvar_item(&item);
  *root = insert(*root, item); // puxa a função insert como parametro
  printf(GREEN "Item cadastrado com sucesso!\n" RESET);
}
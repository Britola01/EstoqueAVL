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

void extratomovimento(Node **root) {
  int id;
  Item item;
  Empresa empresa;
  int count = 0;
  FILE *file1 = fopen("itens.bin", "rb");
  FILE *file5 = fopen("empresa.bin", "rb+");

  while (fread(&item, sizeof(Item), 1, file1) == 1) {
    printf(CYAN "----------------------------------------\n");
    printf(MAGENTA "Item de id: %d\n" RESET, item.id);
    printf(CYAN "ID: %d\n", item.id);
    printf("Nome: %s\n", item.nome);
    printf("Quantidade: %d\n", item.quantidade);
    printf("---------------------------------------\n");
  }
  fclose(file1);

  printf("\n\nInsira o ID do produto que deseja ver as movimentações: ");
  scanf("%d", &id);

  FILE *outputfile = fopen("extrato_movimento.txt", "w");
  if (outputfile == NULL) {
    printf("Erro ao abrir o arquivo de movimentações.\n");
    return;
  }

  Node *extrato = search(*root, id, &count);

  if (extrato == NULL) {
    printf("\nProduto com ID %d não encontrado.\n", id);
    fclose(outputfile);
    return;
  }
  fprintf(outputfile,
          "---------------------------------------------------------\n");
  fprintf(outputfile, "Empresa Terceiro SI\n");
  fprintf(outputfile,
          "---------------------------------------------------------\n");
  fprintf(outputfile, "\nMovimentações do item %s com ID %d:\n",
          extrato->item.nome, id);
  fprintf(outputfile,
          "---------------------------------------------------------\n");
  fprintf(outputfile, "\nVendas:\n");
  fprintf(outputfile, "%-12s%-11s%-15s%-15s%s", "Nota Fiscal", "Data",
          "Quantidade", "Preço", "Valor\n");
  float aux = 0;
  fprintf(outputfile,
          "---------------------------------------------------------\n");

  FILE *file2 = fopen("vendas.bin", "rb");
  Venda venda;
  if (file2 == NULL) {
    printf("Erro ao abrir o arquivo vendas.bin.\n");
    return;
  }
  while (fread(&venda, sizeof(Venda), 1, file2) == 1) {
    if (venda.id_produto == id) {
      aux += venda.valor;
      fprintf(outputfile, "%-12d%02d/%02d/%-10d%-10d%-14.2f%.2f\n",
              venda.nota_fiscal, venda.data.dia, venda.data.mes, venda.data.ano,
              venda.quantidade, venda.venda, venda.valor);
    }
  }
  fprintf(outputfile,
          "---------------------------------------------------------\n");
  fprintf(outputfile, "Total de Vendas%37.2s%-.2f\n", " ", aux);
  fprintf(outputfile,
          "---------------------------------------------------------\n");
  fclose(file2);

  aux = 0;
  fprintf(outputfile, "\nCompras:\n");
  fprintf(outputfile, "%-12s%-11s%-15s%-15s%s", "Nota Fiscal", "Data",
          "Quantidade", "Preço", "Valor\n");
  fprintf(outputfile,
          "---------------------------------------------------------\n");
  FILE *file3 = fopen("compras.bin", "rb");
  Compra compra;
  if (file3 == NULL) {
    printf("Erro ao abrir o arquivo compras.bin.\n");
    return;
  }
  while (fread(&compra, sizeof(Compra), 1, file3) == 1) {
    if (compra.id_produto == id) {
      aux += compra.valor;
      fprintf(outputfile, "%-12d%02d/%02d/%-10d%-10d%-14.2f%.2f\n",
              compra.nota_fiscal, compra.data.dia, compra.data.mes,
              compra.data.ano, compra.quantidade, compra.preco_custo,
              compra.valor);
    }
  }
  fprintf(outputfile,
          "---------------------------------------------------------\n");
  fprintf(outputfile, "Total de Compras%36.2s%-.2f\n", " ", aux);
  fprintf(outputfile,
          "---------------------------------------------------------\n");
  fclose(file3);

  fclose(outputfile);
  printf(GREEN "Extrato de vendas gerado com sucesso! O resultado foi salvo em "
               "'extrato_movimento.txt'.\n" RESET);
}

void relatorio_vendas_mes() {
  FILE *arquivo = fopen("vendas.bin", "rb");
  if (arquivo == NULL) {
    printf(RED "Erro ao abrir arquivo de vendas.\n" RESET);
    return;
  }
  FILE *arquivo2 = fopen("itens.bin", "rb");
  if (arquivo2 == NULL) {
    printf(RED "Erro ao abrir arquivo de itens.\n" RESET);
    return;
  }

  char mes[3];
  char ano[5];
  printf(CYAN "Digite o mês desejado (MM): " RESET);
  scanf("%s", mes);
  printf(CYAN "Digite o ano desejado (AAAA): " RESET);
  scanf("%s", ano);

  // Cria o arquivo de relatório
  char nome_arquivo[100];
  sprintf(nome_arquivo, "relatorio_vendas_%s_%s.txt", mes, ano);
  FILE *relatorio = fopen(nome_arquivo, "w");
  if (relatorio == NULL) {
    printf("Erro ao criar arquivo de relatório.\n");
    fclose(arquivo);
    return;
  }

  // Escreve o cabeçalho do relatório
  fprintf(relatorio, "------------------------------------------\n");
  fprintf(relatorio, "Empresa Terceiro SI\n");
  fprintf(relatorio, "Relação de Vendas do mês %s e ano %s\n", mes, ano);
  fprintf(relatorio, "------------------------------------------\n");
  fprintf(relatorio, "id\tDescrição\tQuantidade\tPreço\t  Valor\n");
  fprintf(relatorio, "------------------------------------------\n");

  // Variáveis de controle
  float total_valor = 0.0;

  // Lê as informações de cada venda do arquivo e escreve no relatório
  Venda venda;
  Item item;
  while (fread(&venda, sizeof(Venda), 1, arquivo) == 1) {
    (fread(&item, sizeof(Item), 1, arquivo2));
    if (venda.data.mes == atoi(mes) && venda.data.ano == atoi(ano)) {
      fprintf(relatorio, "%d\t   %s\t     %d\t      %.2f\t %.2f\n",
              venda.id_produto, item.nome, venda.quantidade, venda.venda,
              venda.valor);
      total_valor += venda.valor;
    }
  }

  // Imprime o total
  fprintf(relatorio, "------------------------------------------\n");
  fprintf(relatorio, "Total\t\t\t\t\t\t%.2f\n", total_valor);
  fprintf(relatorio, "------------------------------------------\n");

  fclose(arquivo);
  fclose(relatorio);

  printf(GREEN "Relatório de vendas gerado com sucesso!\n" RESET);
}

void gerar_relatorio_produtos() {
  FILE *file = fopen("itens.bin", "rb");
  if (file == NULL) {
    printf(RED "Erro ao abrir arquivo de itens.\n" RESET);
    return;
  }
  // Cria o arquivo de relatório
  FILE *relatorio = fopen("relatorio_produtos.txt", "w");
  if (relatorio == NULL) {
    printf(RED "Erro ao criar arquivo de relatório.\n" RESET);
    fclose(file);
    return;
  }

  // Escreve o cabeçalho do relatório
  fprintf(relatorio,
          "------------------------------------------------------\n");
  fprintf(relatorio, "Empresa Terceiro SI\n");
  fprintf(relatorio, "Relação de produtos\n");
  fprintf(relatorio,
          "------------------------------------------------------\n");
  fprintf(relatorio, "id\tNome\t\t\tQuantidade\t\tPreço\t\t\tCusto\t\tValor\n");
  fprintf(relatorio,
          "------------------------------------------------------\n");

  // Variáveis de controle
  float total_valor = 0.0;

  // Carrega todos os itens em um vetor
  fseek(file, 0, SEEK_END);
  int num_itens =
      ftell(file) /
      sizeof(Item); // como o fseek porem pesquisa por objetos especificos
  rewind(file);
  Item *itens = (Item *)malloc(num_itens * sizeof(Item));
  fread(itens, sizeof(Item), num_itens, file);

  // Ordena o vetor de itens por nome em ordem alfabética
  qsort(itens, num_itens, sizeof(Item),
        comparar_itens_por_nome); // essa função usa o qsort para organizar os
                                  // arrays e coloca-los em ordem alfabetica

  // Itera sobre os itens e escreve no relatório
  for (int i = 0; i < num_itens; i++) {
    fprintf(relatorio, "%d%10s%8d%15.2f%10.2f%8.2f\n", itens[i].id,
            itens[i].nome, itens[i].quantidade, itens[i].preco_venda,
            itens[i].custo, itens[i].custo * itens[i].quantidade);
    total_valor += itens[i].custo * itens[i].quantidade;
  }

  // Imprime o total
  fprintf(relatorio,
          "------------------------------------------------------\n");
  fprintf(relatorio, "Valor em estoque\t\t\t\t\t\t%.2f\n", total_valor);
  fprintf(relatorio,
          "------------------------------------------------------\n");

  fclose(file);
  fclose(relatorio);

  free(itens);

  printf(GREEN "Relatório de produtos gerado com sucesso!\n" RESET);
}
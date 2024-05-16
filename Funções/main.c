#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED "\x1b[31m"
#define RESET "\x1b[0m"
#define CYAN "\x1b[36m"
#define GREEN "\x1b[33m"
#define MAGENTA "\x1b[35m"
#include "functions.h"

void exibir_menu() {
  printf(CYAN "\n======================================\n" RESET);
  printf(MAGENTA "                MENU\n" RESET);
  printf(CYAN "======================================\n");
  printf("||1.  Cadastrar empresa             ||\n");
  printf("||2.  Cadastrar item                ||\n");
  printf("||3.  Cadastrar compra              ||\n");
  printf("||4.  Cadastrar venda               ||\n");
  printf("||5.  Alterar Item                  ||\n");
  printf("||6.  Excluir item                  ||\n");
  printf("||7.  Pesquisar produto             ||\n");
  printf("||8.  Pesquisar venda               ||\n");
  printf("||9.  Pesquisar compra              ||\n");
  printf("||10. Pesquisar empresa             ||\n");
  printf("||11. Extrato de Movimento          ||\n");
  printf("||12. Pesquisar todas as compras    ||\n");
  printf("||13. Relatório de vendas do mês    ||\n" RESET);
  printf(RED "  14. Sair--------->               \n" RESET);
  printf(CYAN "======================================\n");
  printf("Digite a opção desejada: " RESET);
}

int main() {
  Venda *vendas;
  int num_vendas;
  int opcao;
  Empresa empresa;
  do {
    exibir_menu();
    scanf("%d", &opcao);
    puts("");
    // Carregar itens e construir a árvore AVL
    Node *root = load_items_and_build_avl_tree();
    switch (opcao) {
    case 1: {
      Empresa empresa;
      cadastrar_empresa(&empresa);
      break;
    }
    case 2: {
      Item item;
      cadastrar_item(&root);
      gerar_relatorio_produtos();
      break;
    }
    case 3: {
      comprar_produto(root);
      gerar_relatorio_produtos();
      break;
    }
    case 4: {
      vender_produto(root);
      gerar_relatorio_produtos();
      break;
    }
    case 5: {
      int id;
      alterar_item(root);
      gerar_relatorio_produtos();
      break;
    }
    case 6: {
      excluir_item_por_id(&root);
      gerar_relatorio_produtos();
      break;
    }
    case 7: {
      pesquisar_produto(root);
      break;
    }
    case 8: {
      pesquisar_vendas(root);
      break;
    }
    case 9: {
      pesquisar_compras(root);
      break;
    }
    case 10: {
      pesquisar_empresa();
      break;
    }
    case 11: {
      extratomovimento(&root);
      break;
    }
    case 12: {
      pesquisar_todas_compras();
      break;
    }
    case 13: {
      relatorio_vendas_mes();
      break;
    }
    case 14:
      printf(RED "Saindo...\n");
      break;
    default:
      printf(RED "Opção inválida. Tente novamente.\n");
    }
  } while (opcao != 14);

  return 0;
}
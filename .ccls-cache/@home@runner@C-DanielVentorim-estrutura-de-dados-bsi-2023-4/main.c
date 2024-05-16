#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED "\x1b[31m"
#define RESET "\x1b[0m"
#define CYAN "\x1b[36m"
#define GREEN "\x1b[33m"
#define MAGENTA "\x1b[35m"

void gerar_relatorio_produtos();
void extratomovimento();
// professor, devido ao grande esforço empenhado nesse codigo, peço que olhe com
// carinho 👍👌😎
// 👍
// Estruturas
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

// Funções para manipulação de arquivos
void salvar_empresa(Empresa *empresa) {
  FILE *file = fopen("empresa.bin", "wb");
  fwrite(empresa, sizeof(Empresa), 1, file);
  fclose(file);
}

void salvar_item(Item *item) {
  FILE *file = fopen("itens.bin", "ab");
  fwrite(item, sizeof(Item), 1, file);
  fclose(file);
}

void salvar_compra(Compra *compra) {
  FILE *file = fopen("compras.bin", "ab");
  fwrite(compra, sizeof(Compra), 1, file);
  fclose(file);
}

void salvar_venda(Venda *venda) {
  FILE *file = fopen("vendas.bin", "ab");
  fwrite(venda, sizeof(Venda), 1, file);
  fclose(file);
}
//--------------------------------------------------
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
//---------------------------------------------------
// funções de verificação, venda, compra, cadastro de empresa e produto
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
//---------------------------------------------------
// funções de pesquisas, relatórios e extratos
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
//---------------------------------------------------
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
} // compara as strings retornadas por ponteiro
//--------------------------------------------------
// funções de exclusão de itens, alteração e relatório de produtos
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
//--------------------------------------------------
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
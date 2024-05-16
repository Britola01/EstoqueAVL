#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "structs.h"

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


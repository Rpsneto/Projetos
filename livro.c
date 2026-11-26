
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "livro.h"

#define ARQUIVO_BINARIO "livros.dat"
#define ARQUIVO_TEXTO "biblioteca.txt"

static int aumentar_capacidade(
    Livro **lista,
    int *capacidade,
    int total
) {
    if (total < *capacidade) {
        return 1;
    }

    int nova_capacidade = (*capacidade == 0) ? 10 : *capacidade * 2;

    Livro *temporario = realloc(
        *lista,
        nova_capacidade * sizeof(Livro)
    );

    if (temporario == NULL) {
        printf("Erro ao alocar memoria para livros.\n");
        return 0;
    }

    *lista = temporario;
    *capacidade = nova_capacidade;

    return 1;
}

void ordenar_livros(Livro lista[], int total) {
    int i, j;
    Livro chave;

    for (i = 1; i < total; i++) {
        chave = lista[i];
        j = i - 1;

        while (j >= 0 && lista[j].codigo > chave.codigo) {
            lista[j + 1] = lista[j];
            j--;
        }
        
        lista[j + 1] = chave;
    }
}

int buscar_livro_codigo(Livro lista[], int total, int codigo) {
    int inicio = 0;
    int fim = total - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        if (lista[meio].codigo == codigo) {
            return meio;
        }

        if (lista[meio].codigo < codigo) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    return -1;
}

void cadastrar_livro(Livro **lista, int *total, int *capacidade) {
    Livro novo;

    printf("\n--- CADASTRO DE LIVRO ---\n");

    printf("Codigo: ");
    if (scanf("%d", &novo.codigo) != 1) return;

    if (buscar_livro_codigo(*lista, *total, novo.codigo) != -1) {
        printf("Ja existe um livro com esse codigo.\n");
        return;
    }

    printf("Titulo: ");
    scanf(" %99[^\n]", novo.titulo);

    printf("Autor: ");
    scanf(" %49[^\n]", novo.autor);

    printf("Ano: ");
    if (scanf("%d", &novo.ano) != 1) return;

    printf("Quantidade: ");
    if (scanf("%d", &novo.quantidade) != 1) return;

    if (novo.quantidade < 0) {
        printf("Quantidade invalida.\n");
        return;
    }

    if (!aumentar_capacidade(lista, capacidade, *total)) {
        return;
    }

    (*lista)[*total] = novo;
    (*total)++;

    ordenar_livros(*lista, *total);

    printf("\nLivro cadastrado com sucesso!\n");
}

void listar_livros(Livro lista[], int total) {
    int i;

    printf("\n--- LISTA DE LIVROS ---\n");

    if (total == 0) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }

    for (i = 0; i < total; i++) {
        printf("\nCodigo: %d\n", lista[i].codigo);
        printf("Titulo: %s\n", lista[i].titulo);
        printf("Autor: %s\n", lista[i].autor);
        printf("Ano: %d\n", lista[i].ano);
        printf("Quantidade: %d\n", lista[i].quantidade);
    }
}

int salvar_livros(Livro lista[], int total) {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar livros.\n");
        return 0;
    }

    if (fwrite(&total, sizeof(int), 1, arquivo) != 1) {
        fclose(arquivo);
        return 0;
    }

    if (total > 0 &&
        fwrite(lista, sizeof(Livro), total, arquivo) != (size_t)total) {
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

void carregar_livros(
    Livro **lista,
    int *total,
    int *capacidade
) {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "rb");

    *lista = NULL;
    *total = 0;
    *capacidade = 0;

    if (arquivo != NULL) {
        int quantidade;

        if (fread(&quantidade, sizeof(int), 1, arquivo) == 1 &&
            quantidade >= 0) {

            if (quantidade > 0) {
                Livro *dados = malloc(quantidade * sizeof(Livro));

                if (dados != NULL &&
                    fread(dados, sizeof(Livro), quantidade, arquivo)
                    == (size_t)quantidade) {

                    *lista = dados;
                    *total = quantidade;
                    *capacidade = quantidade;
                } else {
                    free(dados);
                    printf("Erro ao ler livros.dat.\n");
                }
            }
        }

        fclose(arquivo);
        ordenar_livros(*lista, *total);
        return;
    }

    /* Importacao dos dados antigos do arquivo TXT */
    arquivo = fopen(ARQUIVO_TEXTO, "r");

    if (arquivo == NULL) {
        return;
    }

    while (1) {
        Livro novo;

        if (fscanf(arquivo, "%d\n", &novo.codigo) != 1) {
            break;
        }

        if (fgets(novo.titulo, sizeof(novo.titulo), arquivo) == NULL) break;
        novo.titulo[strcspn(novo.titulo, "\r\n")] = '\0';

        if (fgets(novo.autor, sizeof(novo.autor), arquivo) == NULL) break;
        novo.autor[strcspn(novo.autor, "\r\n")] = '\0';

        if (fscanf(
            arquivo,
            "%d\n%d\n",
            &novo.ano,
            &novo.quantidade
        ) != 2) {
            break;
        }

        if (!aumentar_capacidade(lista, capacidade, *total)) {
            break;
        }

        (*lista)[*total] = novo;
        (*total)++;
    }

    fclose(arquivo);

    ordenar_livros(*lista, *total);
    salvar_livros(*lista, *total);
}

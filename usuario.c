
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario.h"

#define ARQUIVO_BINARIO "usuarios.dat"
#define ARQUIVO_TEXTO "usuarios.txt"

static int aumentar_capacidade(
    Usuario **lista,
    int *capacidade,
    int total
) {
    if (total < *capacidade) {
        return 1;
    }

    int nova_capacidade = (*capacidade == 0) ? 10 : *capacidade * 2;

    Usuario *temporario = realloc(
        *lista,
        nova_capacidade * sizeof(Usuario)
    );

    if (temporario == NULL) {
        printf("Erro ao alocar memoria para usuarios.\n");
        return 0;
    }

    *lista = temporario;
    *capacidade = nova_capacidade;

    return 1;
}

void ordenar_usuarios(Usuario lista[], int total) {
    int i, j;
    Usuario chave;

    for (i = 1; i < total; i++) {
        chave = lista[i];
        j = i - 1;

        while (j >= 0 && lista[j].matricula > chave.matricula) {
            lista[j + 1] = lista[j];
            j--;
        }

        lista[j + 1] = chave;
    }
}

int buscar_usuario_matricula(
    Usuario lista[],
    int total,
    int matricula
) {
    int inicio = 0;
    int fim = total - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        if (lista[meio].matricula == matricula) {
            return meio;
        }

        if (lista[meio].matricula < matricula) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    return -1;
}

void cadastrar_usuario(
    Usuario **lista,
    int *total,
    int *capacidade
) {
    Usuario novo;

    printf("\n--- CADASTRO DE USUARIO ---\n");

    printf("Matricula: ");
    if (scanf("%d", &novo.matricula) != 1) return;

    if (buscar_usuario_matricula(
        *lista,
        *total,
        novo.matricula
    ) != -1) {
        printf("Essa matricula ja esta cadastrada.\n");
        return;
    }

    printf("Nome: ");
    scanf(" %99[^\n]", novo.nome);

    printf("Curso: ");
    scanf(" %49[^\n]", novo.curso);

    novo.total_emprestimos = 0;

    if (!aumentar_capacidade(lista, capacidade, *total)) {
        return;
    }

    (*lista)[*total] = novo;
    (*total)++;

    ordenar_usuarios(*lista, *total);

    printf("\nUsuario cadastrado com sucesso!\n");
}

void listar_usuarios(Usuario lista[], int total) {
    int i;

    printf("\n--- LISTA DE USUARIOS ---\n");

    if (total == 0) {
        printf("Nenhum usuario cadastrado.\n");
        return;
    }

    for (i = 0; i < total; i++) {
        printf("\nMatricula: %d\n", lista[i].matricula);
        printf("Nome: %s\n", lista[i].nome);
        printf("Curso: %s\n", lista[i].curso);
        printf("Total de emprestimos: %d\n",
               lista[i].total_emprestimos);
    }
}

int salvar_usuarios(Usuario lista[], int total) {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar usuarios.\n");
        return 0;
    }

    if (fwrite(&total, sizeof(int), 1, arquivo) != 1) {
        fclose(arquivo);
        return 0;
    }

    if (total > 0 &&
        fwrite(lista, sizeof(Usuario), total, arquivo) != (size_t)total) {
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

void carregar_usuarios(
    Usuario **lista,
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
                Usuario *dados = malloc(quantidade * sizeof(Usuario));

                if (dados != NULL &&
                    fread(dados, sizeof(Usuario), quantidade, arquivo)
                    == (size_t)quantidade) {

                    *lista = dados;
                    *total = quantidade;
                    *capacidade = quantidade;
                } else {
                    free(dados);
                    printf("Erro ao ler usuarios.dat.\n");
                }
            }
        }

        fclose(arquivo);
        ordenar_usuarios(*lista, *total);
        return;
    }

    /* Importacao do arquivo TXT antigo */
    arquivo = fopen(ARQUIVO_TEXTO, "r");

    if (arquivo == NULL) {
        return;
    }

    while (1) {
        Usuario novo;

        if (fscanf(arquivo, "%d\n", &novo.matricula) != 1) {
            break;
        }

        if (fgets(novo.nome, sizeof(novo.nome), arquivo) == NULL) break;
        novo.nome[strcspn(novo.nome, "\r\n")] = '\0';

        if (fgets(novo.curso, sizeof(novo.curso), arquivo) == NULL) break;
        novo.curso[strcspn(novo.curso, "\r\n")] = '\0';

        if (fscanf(
            arquivo,
            "%d\n",
            &novo.total_emprestimos
        ) != 1) {
            break;
        }

        if (!aumentar_capacidade(lista, capacidade, *total)) {
            break;
        }

        (*lista)[*total] = novo;
        (*total)++;
    }

    fclose(arquivo);

    ordenar_usuarios(*lista, *total);
    salvar_usuarios(*lista, *total);
}

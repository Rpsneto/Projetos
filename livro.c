#include <stdio.h>
#include <string.h>
#include "livro.h"

void cadastrar_livro(Livro lista[], int *total) {

    if (*total >= 100) {
        printf("Erro: Limite de livros atingido no estoque!\n");
        return;
    }

    Livro novo;

    printf("Digite o codigo do livro (apenas numeros): ");
    scanf("%d", &novo.codigo);
    while (getchar() != '\n');

    printf("Digite o titulo do livro: ");
    fgets(novo.titulo, 100, stdin);
    novo.titulo[strcspn(novo.titulo, "\n")] = 0;

    printf("Digite o autor do livro: ");
    fgets(novo.autor, 50, stdin);
    novo.autor[strcspn(novo.autor, "\n")] = 0;

    printf("Digite o ano de publicacao: ");
    scanf("%d", &novo.ano);

    printf("Digite a quantidade de exemplares disponiveis: ");
    scanf("%d", &novo.quantidade);
    while (getchar() != '\n');

    lista[*total] = novo;
    (*total)++;

    FILE *arquivo = fopen("biblioteca.txt", "a");

    if (arquivo != NULL) {

        fprintf(
            arquivo,
            "%d\n%s\n%s\n%d\n%d\n",
            novo.codigo,
            novo.titulo,
            novo.autor,
            novo.ano,
            novo.quantidade
        );

        fclose(arquivo);

        printf("Livro cadastrado e salvo com sucesso!\n");

    } else {

        printf("Erro ao salvar o livro no arquivo.\n");
    }
}


void listar_livros(Livro lista[], int total) {

    if (total == 0) {
        printf("\nNenhum livro cadastrado.\n");
        return;
    }

    printf("\n=== CATALOGO DE LIVROS ===\n");

    for (int i = 0; i < total; i++) {

        printf("Codigo:   %d\n", lista[i].codigo);
        printf("Titulo:   %s\n", lista[i].titulo);
        printf("Autor:    %s\n", lista[i].autor);
        printf("Ano:      %d\n", lista[i].ano);
        printf("Estoque:  %d exemplares\n", lista[i].quantidade);

        printf("---------------------------\n");
    }
}


int buscar_livro_codigo(Livro lista[], int total, int codigo) {

    for (int i = 0; i < total; i++) {

        if (lista[i].codigo == codigo) {
            return i;
        }
    }

    return -1;
}


void carregar_livros(Livro lista[], int *total) {

    FILE *arquivo = fopen("biblioteca.txt", "r");

    if (arquivo == NULL) {
        return;
    }

    Livro l;

    while (fscanf(arquivo, "%d\n", &l.codigo) == 1) {

        if (fgets(l.titulo, 100, arquivo) == NULL) {
            break;
        }

        l.titulo[strcspn(l.titulo, "\n")] = 0;

        if (fgets(l.autor, 50, arquivo) == NULL) {
            break;
        }

        l.autor[strcspn(l.autor, "\n")] = 0;

        if (fscanf(arquivo, "%d\n", &l.ano) != 1) {
            break;
        }

        if (fscanf(arquivo, "%d\n", &l.quantidade) != 1) {
            break;
        }

        if (*total < 100) {
            lista[*total] = l;
            (*total)++;
        }
    }

    fclose(arquivo);
}
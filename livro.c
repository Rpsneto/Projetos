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

   
    printf("Digite a quantidade de exemplares disponíveis: ");
    scanf("%d", &novo.quantidade);
    
    while (getchar() != '\n');


   
    lista[*total] = novo;
    (*total)++;

   
    FILE *arquivo = fopen("biblioteca.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "%d\n%s\n%s\n%d\n%d\n", novo.codigo, novo.titulo, novo.autor, novo.ano, novo.quantidade);
        fclose(arquivo);
        printf("Livro cadastrado e salvo com sucesso!\n");
    } else {
        printf("Erro ao salvar o livro no arquivo.\n");
    }
}

void listar_livros(Livro lista[], int total) {
    FILE *arquivo = fopen("biblioteca.txt", "r");
    if (arquivo != NULL) {
        Livro l;
        
        printf("\n=== CATALOGO DE LIVROS ===\n");
        while (fscanf(arquivo, "%d\n %[^\n]\n %[^\n]\n%d\n%d\n", &l.codigo, l.titulo, l.autor, &l.ano, &l.quantidade) == 5) {
            printf("Codigo:     %d\n", l.codigo);
            printf("Titulo:     %s\n", l.titulo);
            printf("Autor:      %s\n", l.autor);
            printf("Ano:        %d\n", l.ano);
            printf("Estoque:    %d exemplares\n", l.quantidade);
            printf("---------------------------\n");
        }
        fclose(arquivo);
    } else {
        printf("Nenhum livro cadastrado no arquivo texto ainda.\n");
    }
}

int buscar_livro_codigo(Livro lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) {
            return i; 
        }
    }
    return -1; 
}
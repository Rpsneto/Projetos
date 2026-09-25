#include <stdio.h>
#include <string.h>
#include "usuario.h"

void cadastrar_usuario(Usuario lista[], int *total) {
    if (*total >= 100) {
        printf("Erro: Limite de usuarios atingido!\n");
        return;
    }

    Usuario novo;
    printf("Digite a matricula do usuario (apenas numeros): ");
    scanf("%d", &novo.matricula);
    while (getchar() != '\n');

    printf("Digite o nome do usuario: ");
    fgets(novo.nome, 50, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = 0;

    printf("Digite o curso do usuario: ");
    fgets(novo.curso, 50, stdin);
    novo.curso[strcspn(novo.curso, "\n")] = 0;

    novo.total_emprestimos = 0;

    lista[*total] = novo;
    (*total)++;

    FILE *arquivo = fopen("usuarios.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "%d\n%s\n%s\n%d\n", novo.matricula, novo.nome, novo.curso, novo.total_emprestimos);
        fclose(arquivo);
        printf("Usuario cadastrado e salvo com sucesso!\n");
    } else {
        printf("Erro ao salvar o usuario no arquivo.\n");
    }
}

void listar_usuarios(Usuario lista[], int total) {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo != NULL) {
        Usuario u;
        printf("\n=== LISTA DE USUARIOS ===\n");
        while (fscanf(arquivo, "%d\n %[^\n]\n %[^\n]\n%d\n", &u.matricula, u.nome, u.curso, &u.total_emprestimos) == 4) {
            printf("Matricula:    %d\n", u.matricula);
            printf("Nome:         %s\n", u.nome);
            printf("Curso:        %s\n", u.curso);
            printf("Emprestimos:  %d ativos\n", u.total_emprestimos);
            printf("---------------------------\n");
        }
        fclose(arquivo);
    } else {
        printf("Nenhum usuario cadastrado no arquivo texto ainda.\n");
    }
}

int buscar_usuario_matricula(Usuario lista[], int total, int matricula) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].matricula == matricula) {
            return i;
        }
    }
    return -1;
}

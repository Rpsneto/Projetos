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
    fgets(novo.nome, 100, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = 0;

    printf("Digite o curso do usuario: ");
    fgets(novo.curso, 50, stdin);
    novo.curso[strcspn(novo.curso, "\n")] = 0;

    novo.total_emprestimos = 0;

    lista[*total] = novo;
    (*total)++;

    FILE *arquivo = fopen("usuarios.txt", "a");

    if (arquivo != NULL) {

        fprintf(
            arquivo,
            "%d\n%s\n%s\n%d\n",
            novo.matricula,
            novo.nome,
            novo.curso,
            novo.total_emprestimos
        );

        fclose(arquivo);

        printf("Usuario cadastrado e salvo com sucesso!\n");

    } else {

        printf("Erro ao salvar o usuario no arquivo.\n");
    }
}


void listar_usuarios(Usuario lista[], int total) {

    if (total == 0) {
        printf("\nNenhum usuario cadastrado.\n");
        return;
    }

    printf("\n=== LISTA DE USUARIOS ===\n");

    for (int i = 0; i < total; i++) {

        printf("Matricula:    %d\n", lista[i].matricula);
        printf("Nome:         %s\n", lista[i].nome);
        printf("Curso:        %s\n", lista[i].curso);
        printf("Emprestimos:  %d ativos\n", lista[i].total_emprestimos);
        printf("---------------------------\n");
    }
}


int buscar_usuario_matricula(Usuario lista[], int total, int matricula) {

    for (int i = 0; i < total; i++) {

        if (lista[i].matricula == matricula) {
            return i;
        }
    }

    return -1;
}


void carregar_usuarios(Usuario lista[], int *total) {

    FILE *arquivo = fopen("usuarios.txt", "r");

    if (arquivo == NULL) {
        return;
    }

    Usuario u;

    while (fscanf(arquivo, "%d\n", &u.matricula) == 1) {

        if (fgets(u.nome, 100, arquivo) == NULL) {
            break;
        }

        u.nome[strcspn(u.nome, "\n")] = 0;


        if (fgets(u.curso, 50, arquivo) == NULL) {
            break;
        }

        u.curso[strcspn(u.curso, "\n")] = 0;


        if (fscanf(arquivo, "%d\n", &u.total_emprestimos) != 1) {
            break;
        }


        if (*total < 100) {

            lista[*total] = u;
            (*total)++;
        }
    }

    fclose(arquivo);
}
#include <stdio.h>
#include <string.h>
#include "emprestimo.h"


void registrar_emprestimo(Emprestimo lista_emp[], int *total_emp, Usuario lista_usu[], int total_usu, Livro lista_liv[], int total_liv) {
   
    if (*total_emp >= 100) {
        printf("Erro: Limite de emprestimos atingido!\n");
   
        return;
    }

    Emprestimo novo;
    int mat, cod;

   
    printf("Digite a matricula do usuario: ");
   
    scanf("%d", &mat);
   
    int pos_usu = buscar_usuario_matricula(lista_usu, total_usu, mat);
    if (pos_usu == -1) {
        printf("Erro: Usuario nao encontrado!\n");
        return;
    }

    printf("Digite o codigo do livro: ");
   
    scanf("%d", &cod);
    int pos_liv = buscar_livro_codigo(lista_liv, total_liv, cod);
    if (pos_liv == -1) {
        printf("Erro: Livro nao encontrado!\n");
        return;
    }

    if (lista_liv[pos_liv].quantidade <= 0) {
        printf("Erro: Livro esgotado no estoque!\n");
        return;
    }

    novo.id_emprestimo = *total_emp + 1;
    
    
    novo.matricula_usuario = mat;
    novo.codigo_livro = cod;
    novo.dias_atraso = 0;
    novo.multa = 0.0;
    novo.ativo = 1;

    lista_liv[pos_liv].quantidade--; 
   
    lista_usu[pos_usu].total_emprestimos++;

    lista_emp[*total_emp] = novo;
    
    (*total_emp)++;

    FILE *arquivo = fopen("emprestimos.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "%d\n%d\n%d\n%d\n%f\n%d\n", novo.id_emprestimo, novo.matricula_usuario, novo.codigo_livro, novo.dias_atraso, novo.multa, novo.ativo);
   
        fclose(arquivo);
        printf("Emprestimo registrado com sucesso! ID: %d\n", novo.id_emprestimo);
   
    } else {
        printf("Erro ao salvar o emprestimo no arquivo.\n");
    }
}



void registrar_devolucao(Emprestimo lista_emp[], int total_emp, Livro lista_liv[], int total_liv) {
    int id, dias;
   
    printf("Digite o ID do emprestimo para devolucao: ");
   
    scanf("%d", &id);


    int i;
  
    int encontrado = -1;
    for (i = 0; i < total_emp; i++) {
        if (lista_emp[i].id_emprestimo == id && lista_emp[i].ativo == 1) {
            encontrado = i;
            break;
        }
    }

   
    if (encontrado == -1) {
    
        printf("Erro: Emprestimo ativo nao encontrado!\n");
   
        return;
    }

   
    printf("Digite a quantidade de dias de atraso (0 para nenhum): ");
   
    scanf("%d", &dias);

    lista_emp[encontrado].ativo = 0; 
   
    lista_emp[encontrado].dias_atraso = dias;

    if (dias > 0) {
    
        lista_emp[encontrado].multa = dias * 2.50; 
   
        printf("Devolucao com atraso! Multa aplicada: R$ %.2f\n", lista_emp[encontrado].multa);
    }

  
    int pos_liv = buscar_livro_codigo(lista_liv, total_liv, lista_emp[encontrado].codigo_livro);
    if (pos_liv != -1) {
        lista_liv[pos_liv].quantidade++; 
    }

    printf("Devolucao registrada com sucesso!\n");
}

void listar_emprestimos(Emprestimo lista_emp[], int total_emp) {
   
    FILE *arquivo = fopen("emprestimos.txt", "r");
    if (arquivo != NULL) {
   
        Emprestimo e;
        printf("\n=== TODOS OS EMPRESTIMOS ===\n");
        while (fscanf(arquivo, "%d\n%d\n%d\n%d\n%f\n%d\n", &e.id_emprestimo, &e.matricula_usuario, &e.codigo_livro, &e.dias_atraso, &e.multa, &e.ativo) == 6) {
            printf("ID:         %d\n", e.id_emprestimo);
    
            printf("Matricula:  %d\n", e.matricula_usuario);
   
            printf("Livro cod:  %d\n", e.codigo_livro);
   
            printf("Status:     %s\n", e.ativo == 1 ? "Ativo" : "Devolvido");
            if (e.multa > 0) {
                printf("Multa:      R$ %.2f (%d dias atraso)\n", e.multa, e.dias_atraso);
            }
            printf("---------------------------\n");
        }
        fclose(arquivo);
    } else {
        printf("Nenhum emprestimo registrado no arquivo ainda.\n");
    }
}


void historico_por_usuario(Emprestimo lista_emp[], int total_emp, int matricula) {
    FILE *arquivo = fopen("emprestimos.txt", "r");
    
    if (arquivo != NULL) {
        Emprestimo e;
        int encontrou = 0;
    
        printf("\n=== HISTORICO DA MATRICULA %d ===\n", matricula);
        while (fscanf(arquivo, "%d\n%d\n%d\n%d\n%f\n%d\n", &e.id_emprestimo, &e.matricula_usuario, &e.codigo_livro, &e.dias_atraso, &e.multa, &e.ativo) == 6) {
    
    
            if (e.matricula_usuario == matricula) {
    
                encontrou = 1;
                printf("ID Emprestimo: %d | Livro Cod: %d | Status: %s\n", e.id_emprestimo, e.codigo_livro, e.ativo == 1 ? "Ativo" : "Finalizado");
            }
        }
        if (!encontrou) {
            printf("Nenhum registro encontrado para este usuario.\n");
     
        }
        fclose(arquivo);
  
  
    } else {
        printf("Nenhum registro no sistema.\n");
    }
}

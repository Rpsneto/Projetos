
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "livro.h"
#include "usuario.h"
#include "emprestimo.h"

#define TAMANHO_TESTE 1500


static void bubble_sort(int vetor[], int tamanho) {
    int i, j, temp;

    for (i = 0; i < tamanho - 1; i++) {
        for (j = 0; j < tamanho - i - 1; j++) {
            if (vetor[j] > vetor[j + 1]) {
                temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

/* Algoritmo Selection Sort */
static void selection_sort(int vetor[], int tamanho) {
    int i, j, menor, temp;

 
    for (i = 0; i < tamanho - 1; i++) {
        menor = i;


        for (j = i + 1; j < tamanho; j++) {
            if (vetor[j] < vetor[menor]) {
                menor = j;
 
            }
        }


        temp = vetor[i];
        vetor[i] = vetor[menor];
        vetor[menor] = temp;
    }
}

/* Algoritmo Insertion Sort */
static void insertion_sort(int vetor[], int tamanho) {
    int i, j, chave;

    for (i = 1; i < tamanho; i++) {
        chave = vetor[i];
        j = i - 1;

        while (j >= 0 && vetor[j] > chave) {
            vetor[j + 1] = vetor[j];
            j--;
        }

        vetor[j + 1] = chave;
    }
}

/* Gera o relatorio de desempenho */
static void gerar_relatorio_desempenho(void) {
    int *original;
    int *copia;
    int i;

    clock_t inicio, fim;
    double tempo_bubble;
    double tempo_selection;
    double tempo_insertion;

    FILE *arquivo;

    original = malloc(TAMANHO_TESTE * sizeof(int));
    copia = malloc(TAMANHO_TESTE * sizeof(int));

    if (original == NULL || copia == NULL) {
        printf("Erro ao alocar memoria para o teste.\n");
        free(original);
        free(copia);
        return;
    }

    /* Dados iguais para os tres algoritmos */
    for (i = 0; i < TAMANHO_TESTE; i++) {
        original[i] = (i * 7919 + 123) % 10000;
    }

    /* Teste Bubble Sort */
    for (i = 0; i < TAMANHO_TESTE; i++) {
        copia[i] = original[i];
    }

    inicio = clock();
    bubble_sort(copia, TAMANHO_TESTE);
    fim = clock();

    tempo_bubble = (double)(fim - inicio) / CLOCKS_PER_SEC;

    /* Teste Selection Sort */
    for (i = 0; i < TAMANHO_TESTE; i++) {
        copia[i] = original[i];
    }

    inicio = clock();
    selection_sort(copia, TAMANHO_TESTE);
    fim = clock();

    tempo_selection = (double)(fim - inicio) / CLOCKS_PER_SEC;

    /* Teste Insertion Sort */
    for (i = 0; i < TAMANHO_TESTE; i++) {
        copia[i] = original[i];
    }

    inicio = clock();
    insertion_sort(copia, TAMANHO_TESTE);
    fim = clock();

    tempo_insertion = (double)(fim - inicio) / CLOCKS_PER_SEC;

    arquivo = fopen("relatorio_desempenho.txt", "w");

    if (arquivo == NULL) {
        printf("Nao foi possivel criar o relatorio.\n");
        free(original);
        free(copia);
        return;
    }

    fprintf(arquivo, "RELATORIO DE DESEMPENHO\n");
    fprintf(arquivo, "=======================\n\n");
    fprintf(arquivo, "Quantidade de elementos: %d\n\n",
            TAMANHO_TESTE);

    fprintf(arquivo, "Bubble Sort: %.6f segundos\n",
            tempo_bubble);

    fprintf(arquivo, "Selection Sort: %.6f segundos\n",
            tempo_selection);

    fprintf(arquivo, "Insertion Sort: %.6f segundos\n",
            tempo_insertion);

    fprintf(arquivo, "\nComplexidade dos algoritmos:\n");
    fprintf(arquivo, "Bubble Sort: O(n^2)\n");
    fprintf(arquivo, "Selection Sort: O(n^2)\n");
    fprintf(arquivo, "Insertion Sort: O(n^2) no pior caso\n");

    fclose(arquivo);

    printf("\nRelatorio de desempenho gerado!\n");
    printf("Arquivo: relatorio_desempenho.txt\n");

    printf("\nTempos medidos:\n");
    printf("Bubble Sort: %.6f segundos\n", tempo_bubble);
    printf("Selection Sort: %.6f segundos\n", tempo_selection);
    printf("Insertion Sort: %.6f segundos\n", tempo_insertion);

    free(original);
    free(copia);
}

int main(void) {
    Livro *lista_livros = NULL;
    Usuario *lista_usuarios = NULL;
    Emprestimo *lista_emprestimos = NULL;

    int total_livros = 0;
    int total_usuarios = 0;
    int total_emprestimos = 0;

    int capacidade_livros = 0;
    int capacidade_usuarios = 0;
    int capacidade_emprestimos = 0;

    int opcao;
    int matricula;

    /* Carregamento dos dados */
    carregar_livros(
        &lista_livros,
        &total_livros,
        &capacidade_livros
    );

    carregar_usuarios(
        &lista_usuarios,
        &total_usuarios,
        &capacidade_usuarios
    );

    carregar_emprestimos(
        &lista_emprestimos,
        &total_emprestimos,
        &capacidade_emprestimos
    );

    ordenar_livros(lista_livros, total_livros);
    ordenar_usuarios(lista_usuarios, total_usuarios);

    printf("====================================\n");
    printf("       SISTEMA DE BIBLIOTECA\n");
    printf("====================================\n");

    do {
        printf("\n========== MENU PRINCIPAL ==========\n");
        printf("1 - Cadastrar livro\n");
        printf("2 - Listar livros\n");
        printf("3 - Cadastrar usuario\n");
        printf("4 - Listar usuarios\n");
        printf("5 - Registrar emprestimo\n");
        printf("6 - Registrar devolucao\n");
        printf("7 - Listar emprestimos\n");
        printf("8 - Historico por usuario\n");
        printf("9 - Gerar relatorio de desempenho\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");

            int caractere;
            while ((caractere = getchar()) != '\n' &&
                   caractere != EOF) {
            }

            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1:
                cadastrar_livro(
                    &lista_livros,
                    &total_livros,
                    &capacidade_livros
                );
                break;

            case 2:
                listar_livros(lista_livros, total_livros);
                break;

            case 3:
                cadastrar_usuario(
                    &lista_usuarios,
                    &total_usuarios,
                    &capacidade_usuarios
                );
                break;

            case 4:
                listar_usuarios(lista_usuarios, total_usuarios);
                break;

            case 5:
                registrar_emprestimo(
                    &lista_emprestimos,
                    &total_emprestimos,
                    &capacidade_emprestimos,
                    lista_usuarios,
                    total_usuarios,
                    lista_livros,
                    total_livros
                );
                break;

            case 6:
                registrar_devolucao(
                    lista_emprestimos,
                    total_emprestimos,
                    lista_livros,
                    total_livros
                );
                break;

            case 7:
                listar_emprestimos(
                    lista_emprestimos,
                    total_emprestimos
 
                );
                break;

            case 8:
                printf("Matricula do usuario: ");

                if (scanf("%d", &matricula) == 1) {
 
                    historico_por_usuario(
                        lista_emprestimos,
                        total_emprestimos,
                        matricula
                    );
                }
                break;

 
                case 9:
                gerar_relatorio_desempenho();
                break;

            case 0:
                printf("\nSalvando dados...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }


        /* Salva os dados apos cada operacao */
        if (opcao >= 1 && opcao <= 8) {
            salvar_livros(lista_livros, total_livros);
            salvar_usuarios(lista_usuarios, total_usuarios);
            salvar_emprestimos(
                lista_emprestimos,
                total_emprestimos
 
            );
        }

    } while (opcao != 0);

   
    salvar_livros(lista_livros, total_livros);
    salvar_usuarios(lista_usuarios, total_usuarios);
  
    salvar_emprestimos(lista_emprestimos, total_emprestimos);

    
    free(lista_livros);
    free(lista_usuarios);
    free(lista_emprestimos);

    printf("Dados salvos e memoria liberada.\n");
    printf("Sistema encerrado.\n");

    return 0;
}

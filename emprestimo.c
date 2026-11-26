
#include <stdio.h>
#include <stdlib.h>
#include "emprestimo.h"

#define ARQUIVO_BINARIO "emprestimos.dat"
#define ARQUIVO_TEXTO "emprestimos.txt"

static int aumentar_capacidade(
    Emprestimo **lista,
    int *capacidade,
    int total
) {
    if (total < *capacidade) {
        return 1;
    }

    int nova_capacidade = (*capacidade == 0) ? 10 : *capacidade * 2;

    Emprestimo *temporario = realloc(
        *lista,
        nova_capacidade * sizeof(Emprestimo)
    );

    if (temporario == NULL) {
        printf("Erro ao alocar memoria para emprestimos.\n");
        return 0;
    }

    *lista = temporario;
    *capacidade = nova_capacidade;

    return 1;
}

int salvar_emprestimos(Emprestimo lista[], int total) {
    FILE *arquivo = fopen(ARQUIVO_BINARIO, "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar emprestimos.\n");
        return 0;
    }

    if (fwrite(&total, sizeof(int), 1, arquivo) != 1) {
        fclose(arquivo);
        return 0;
    }

    if (total > 0 &&
        fwrite(lista, sizeof(Emprestimo), total, arquivo)
        != (size_t)total) {
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

void carregar_emprestimos(
    Emprestimo **lista,
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
                Emprestimo *dados =
                    malloc(quantidade * sizeof(Emprestimo));

                if (dados != NULL &&
                    fread(dados, sizeof(Emprestimo), quantidade, arquivo)
                    == (size_t)quantidade) {

                    *lista = dados;
                    *total = quantidade;
                    *capacidade = quantidade;
                } else {
                    free(dados);
                    printf("Erro ao ler emprestimos.dat.\n");
                }
            }
        }

        fclose(arquivo);
        return;
    }

    /* Importacao do arquivo TXT antigo */
    arquivo = fopen(ARQUIVO_TEXTO, "r");

    if (arquivo == NULL) {
        return;
    }
    
    while (1) {
        Emprestimo novo;

        if (fscanf(
            arquivo,
            "%d\n%d\n%d\n%d\n%f\n%d\n",
            &novo.id_emprestimo,
            &novo.matricula_usuario,
            &novo.codigo_livro,
            &novo.dias_atraso,
            &novo.multa,
            &novo.ativo
        ) != 6) {
            break;
        }

        if (!aumentar_capacidade(lista, capacidade, *total)) {
            break;
        }

        (*lista)[*total] = novo;
        (*total)++;
    }

    fclose(arquivo);
    salvar_emprestimos(*lista, *total);
}

void registrar_emprestimo(
    Emprestimo **lista_emp,
    int *total_emp,
    int *capacidade_emp,
    Usuario lista_usu[],
    int total_usu,
    Livro lista_liv[],
    int total_liv
) {
    int matricula, codigo;
    int pos_usuario, pos_livro;
    int proximo_id = 1;
    int i;

    printf("\n--- REGISTRAR EMPRESTIMO ---\n");

    printf("Matricula do usuario: ");
    if (scanf("%d", &matricula) != 1) return;

    pos_usuario = buscar_usuario_matricula(
        lista_usu,
        total_usu,
        matricula
    );

    if (pos_usuario == -1) {
 
        printf("Usuario nao encontrado.\n");
        return;
    }

    printf("Codigo do livro: ");
    if (scanf("%d", &codigo) != 1) return;

    pos_livro = buscar_livro_codigo(
        lista_liv,
        total_liv,
        codigo
    );

    if (pos_livro == -1) {
        printf("Livro nao encontrado.\n");
        return;
    }

    if (lista_liv[pos_livro].quantidade <= 0) {
        printf("Nao ha exemplares disponiveis desse livro.\n");
        return;
    }

    for (i = 0; i < *total_emp; i++) {
 
        if ((*lista_emp)[i].id_emprestimo >= proximo_id) {
            proximo_id = (*lista_emp)[i].id_emprestimo + 1;
        }
    }

    if (!aumentar_capacidade(
        lista_emp,
        capacidade_emp,
        *total_emp
    )) {
        return;
    }

    Emprestimo novo;

    novo.id_emprestimo = proximo_id;
    novo.matricula_usuario = matricula;
    novo.codigo_livro = codigo;
    novo.dias_atraso = 0;
    novo.multa = 0.0f;
    novo.ativo = 1;


    (*lista_emp)[*total_emp] = novo;
    (*total_emp)++;

    lista_liv[pos_livro].quantidade--;
    lista_usu[pos_usuario].total_emprestimos++;

    printf("\nEmprestimo registrado com sucesso!\n");
    printf("ID do emprestimo: %d\n", novo.id_emprestimo);
}

void registrar_devolucao(
    Emprestimo lista_emp[],
 
    int total_emp,
    Livro lista_liv[],
    int total_liv
) {
    int id, dias_atraso;
    int i, pos_livro;

    printf("\n--- REGISTRAR DEVOLUCAO ---\n");

    printf("ID do emprestimo: ");
    if (scanf("%d", &id) != 1) return;

    for (i = 0; i < total_emp; i++) {
        if (lista_emp[i].id_emprestimo == id &&
            lista_emp[i].ativo == 1) {
            break;
 
        }
    }

    if (i == total_emp) {
        printf("Emprestimo nao encontrado ou ja devolvido.\n");
        return;
    }

    pos_livro = buscar_livro_codigo(
        lista_liv,
        total_liv,
        lista_emp[i].codigo_livro
    );


    if (pos_livro == -1) {
        printf("Livro relacionado ao emprestimo nao encontrado.\n");
        return;
    }

    printf("Quantidade de dias de atraso: ");
    if (scanf("%d", &dias_atraso) != 1) return;

    if (dias_atraso < 0) {
        printf("Quantidade de dias invalida.\n");
        return;
    }

    lista_emp[i].dias_atraso = dias_atraso;
    lista_emp[i].multa = dias_atraso * 2.50f;
 
    lista_emp[i].ativo = 0;

    lista_liv[pos_livro].quantidade++;

    printf("\nDevolucao registrada!\n");
    printf("Dias de atraso: %d\n", dias_atraso);
    printf("Multa: R$ %.2f\n", lista_emp[i].multa);
}

void listar_emprestimos(
    Emprestimo lista_emp[],
    int total_emp
) {
    int i;

    printf("\n--- TODOS OS EMPRESTIMOS ---\n");

    if (total_emp == 0) {
        printf("Nenhum emprestimo registrado.\n");
        return;
   
   
    }

    for (i = 0; i < total_emp; i++) {
        printf("\nID: %d\n", lista_emp[i].id_emprestimo);
        printf("Matricula: %d\n", lista_emp[i].matricula_usuario);
        printf("Codigo do livro: %d\n", lista_emp[i].codigo_livro);
        printf("Dias de atraso: %d\n", lista_emp[i].dias_atraso);
        printf("Multa: R$ %.2f\n", lista_emp[i].multa);

        if (lista_emp[i].ativo) {
            printf("Situacao: Emprestado\n");
        } else {
            printf("Situacao: Devolvido\n");
        }
   
    }
}

void historico_por_usuario(
    Emprestimo lista_emp[],
    int total_emp,
    int matricula
) {
    int i;
    int encontrou = 0;

    printf("\n--- HISTORICO DO USUARIO ---\n");

    for (i = 0; i < total_emp; i++) {
        if (lista_emp[i].matricula_usuario == matricula) {
            encontrou = 1;

            printf("\nID: %d\n", lista_emp[i].id_emprestimo);
  
            printf("Codigo do livro: %d\n",
                   lista_emp[i].codigo_livro);
            printf("Dias de atraso: %d\n",
                   lista_emp[i].dias_atraso);
            printf("Multa: R$ %.2f\n", lista_emp[i].multa);

   
            if (lista_emp[i].ativo) {
                printf("Situacao: Emprestado\n");
            } else {
                printf("Situacao: Devolvido\n");
            }
        }
    }

    if (!encontrou) {
        printf("Nenhum emprestimo encontrado para essa matricula.\n");
    }
}

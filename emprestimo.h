
#ifndef EMPRESTIMO_H
#define EMPRESTIMO_H

#include "livro.h"
#include "usuario.h"

typedef struct {
    int id_emprestimo;
    int matricula_usuario;
    int codigo_livro;
    int dias_atraso;
    float multa;
    int ativo;
} Emprestimo;

void registrar_emprestimo(
    Emprestimo **lista_emp,
    int *total_emp,
    int *capacidade_emp,
    Usuario lista_usu[],
    int total_usu,
    Livro lista_liv[],
    int total_liv
);

void registrar_devolucao(
    Emprestimo lista_emp[],
    int total_emp,
    Livro lista_liv[],
    int total_liv
);

void listar_emprestimos(Emprestimo lista_emp[], int total_emp);
void historico_por_usuario(Emprestimo lista_emp[], int total_emp, int matricula);

void carregar_emprestimos(
    Emprestimo **lista,
    int *total,
    int *capacidade
);

int salvar_emprestimos(Emprestimo lista[], int total);

#endif

#ifndef LIVRO_H

#define LIVRO_H


typedef struct {
    int codigo;
    char titulo[100];
    char autor[50];
    int ano;
    int quantidade;
} Livro;

void cadastrar_livro(Livro lista[], int *total);


void listar_livros(Livro lista[], int total);


int buscar_livro_codigo(Livro lista[], int total, int codigo);

#endif

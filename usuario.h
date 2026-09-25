#ifndef USUARIO_H
#define USUARIO_H

typedef struct {
    int matricula;
    char nome[100];
    char curso[50];
    int total_emprestimos; 
} Usuario;

void cadastrar_usuario(Usuario lista[], int *total);
void listar_usuarios(Usuario lista[], int total);
int buscar_usuario_matricula(Usuario lista[], int total, int matricula);

void carregar_usuarios(Usuario lista[], int *total);
#endif

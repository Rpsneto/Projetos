#include <stdio.h>
#include <string.h>

#include "livro.h"
#include "usuario.h"
#include "emprestimo.h"

int main() {

    Livro lista_livros[100];
    int total_livros = 0;

    Usuario lista_usuarios[100];
    int total_usuarios = 0;

    Emprestimo lista_emprestimos[100];
    int total_emprestimos = 0;

    int opcao;
    int matricula_busca;

    // Carrega os livros e usuarios salvos nos arquivos
    carregar_livros(lista_livros, &total_livros);
    carregar_usuarios(lista_usuarios, &total_usuarios);

    do {

        printf("\n===================================\n");
        printf("       SISTEMA DA BIBLIOTECA       \n");
        printf("===================================\n");
        printf("1 - Cadastrar Livro\n");
        printf("2 - Listar Livros\n");
        printf("3 - Cadastrar Usuario\n");
        printf("4 - Listar Usuarios\n");
        printf("5 - Registrar Emprestimo\n");
        printf("6 - Registrar Devolucao (Com Multas)\n");
        printf("7 - Listar Emprestimos\n");
        printf("8 - Historico por Usuario\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        while (getchar() != '\n');

        switch (opcao) {

            case 1:
                cadastrar_livro(
                    lista_livros,
                    &total_livros
                );
                break;

            case 2:
                listar_livros(
                    lista_livros,
                    total_livros
                );
                break;

            case 3:
                cadastrar_usuario(
                    lista_usuarios,
                    &total_usuarios
                );
                break;

            case 4:
                listar_usuarios(
                    lista_usuarios,
                    total_usuarios
                );
                break;

            case 5:
                registrar_emprestimo(
                    lista_emprestimos,
                    &total_emprestimos,
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
                printf("Digite a matricula para buscar o historico: ");

                scanf("%d", &matricula_busca);
                while (getchar() != '\n');

                historico_por_usuario(
                    lista_emprestimos,
                    total_emprestimos,
                    matricula_busca
                );
                break;

            case 0:
                printf("\nSaindo do sistema... Ate logo!\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
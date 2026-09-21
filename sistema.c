//inicio do meu trabalho de biblioteca 

#include <stdio.h>
#include <string.h> 

//Struct dos livros
typedef struct meulivro {
    char titulo[100];
    char autor[50];
    int ano;    
} livro;

void listar_livros() {
    FILE *arquivo = fopen("biblioteca.txt", "r");

    if (arquivo != NULL) {
        livro livrolido;
        printf("\n=== LIVROS CADASTRADOS ===\n");
        while(fscanf(arquivo, " %[^\n] %[^\n] %d", livrolido.titulo, livrolido.autor, &livrolido.ano) == 3) {
            printf("Titulo: %s\n", livrolido.titulo);
            printf("Autor: %s\n", livrolido.autor);
            printf("Ano: %d\n", livrolido.ano);
            printf("--------------------------------\n");
        }
        fclose(arquivo);
    } else {
        printf("Nenhum livro cadastrado ainda.\n");
    }
}  

//Função para salvar os livros
void salvar_livro(livro meulivro[]) {
    FILE *arquivo = fopen("biblioteca.txt", "a");

    if (arquivo != NULL) {
        fprintf(arquivo, "%s\n", meulivro[0].titulo);
        fprintf(arquivo, "%s\n", meulivro[0].autor);
        fprintf(arquivo, "%d\n", meulivro[0].ano);
        fclose(arquivo);
        printf("Livro salvo com sucesso!!!\n");
    } else {
        printf("Erro ao abrir o arquivo ou ao salvar\n");
    }
}

//Main Aqui
int main() {
   
    livro meulivro[100];
    printf("Digite o nome do livro:  ");
    fgets(meulivro[0].titulo, 100, stdin);
    meulivro[0].titulo[strcspn(meulivro[0].titulo, "\n")] = 0; 

    printf("Digite o autor do livro:  ");
    fgets(meulivro[0].autor, 50, stdin);
    meulivro[0].autor[strcspn(meulivro[0].autor, "\n")] = 0; 

    printf("Digite o ano de publicação do livro:  ");
    scanf("%d", &meulivro[0].ano);
    while (getchar() != '\n'); 
    
    printf("==== LIVRO CADASTRADO ====\n");
    printf("Titulo: %s\n", meulivro[0].titulo);
    printf("Autor: %s\n", meulivro[0].autor);
    printf("Ano: %d\n", meulivro[0].ano);
    printf("===========================\n");
    printf("                                \n");
    salvar_livro(meulivro);
    listar_livros();

    
    return 0;
}
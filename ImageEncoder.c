#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void help();

int main(int argc, char *argv[])
{
    help();

    return 0;
}

void help()
{
    //Função que mostra o help para orientações de utilização do programa
    printf("Uso: ./ImageEncoder [-? | -m | -f ARQ]\n");
    printf("Codifica imagens binárias dadas em arquivos PBM ou por dados informados manualmente.");
    printf("Argumentos:\n");
    printf("-?, --help : apresenta essa orientação na tela.\n");
    printf("-m, --manual: ativa o modo de entrada manual, em que o usuário fornece todos os dados da imagem informando-os através do teclado.\n");
    printf("-f, --file: considera a imagem representada no arquivo PBM (Portable bitmap).\n");

    return;
} 
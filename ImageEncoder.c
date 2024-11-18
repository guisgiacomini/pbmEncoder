#include <stdio.h>

void mostraManual();

int main(int argc, char *argv[])
{
    FILE *arquivo = fopen("fodase.txt", "w");
    fprintf(arquivo, "fodase");

    return 0;
}

void mostraManual()
{
    //Função que mostra o help para orientações de utilização do programa
} 
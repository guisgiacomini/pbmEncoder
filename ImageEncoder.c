#include <stdio.h>
#include <string.h>
#include <getopt.h>

#define MAX_LINHAS 768
#define MAX_COLUNAS 1024
//Definindo estrutura imagem
typedef struct {
    int n_linhas;
    int n_colunas;
    int matriz[MAX_LINHAS][MAX_COLUNAS];
} Imagem;

// Função para ler a imagem do arquivo PBM
void lerImagemDoArquivo(const char* nomeArquivo, Imagem *img) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char tipo[3];
    int colunas, linhas;

    // Lendo o tipo do arquivo (P1)
    fscanf(arquivo, "%s", tipo);
    if (strcmp(tipo, "P1") != 0) {
        printf("Formato de arquivo inválido. Apenas arquivos P1 são suportados.\n");
        fclose(arquivo);
        return;
    }

    // Lendo as dimensões da imagem
    fscanf(arquivo, "%d %d", &colunas, &linhas);

    //Sair caso ultrapasse número máximo de linhas e colunas
    if (linhas > MAX_LINHAS || colunas > MAX_COLUNAS)
    {
        printf("Tamanho max: 1024x768");
        return;
    }
    // Criando a imagem
    img->n_linhas = linhas;
    img->n_colunas = colunas;

    // Lendo os matriz
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fscanf(arquivo, "%d", &img->matriz[i][j]);
        }
    }

    fclose(arquivo);
    return;
}

// Função para ler os dados da imagem manualmente
void lerImagemManual(Imagem *img) {
    int colunas, linhas;
    printf("Digite a colunas e linhas da imagem: ");
    scanf("%d %d", &colunas, &linhas);

    //Sair caso ultrapasse número máximo de linhas e colunas
    if (linhas > MAX_LINHAS || colunas > MAX_COLUNAS)
    {
        printf("Tamanho max: 1024x768");
        return;
    }

    printf("Digite os matriz da imagem (0 para branco e 1 para preto):\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            scanf("%d", &img->matriz[i][j]);
        }
    }

    return;
}

// Função para verificar se a imagem é uniforme (toda preta ou toda branca)
int imagemUniforme(Imagem* img) {
    int primeiroPixel = img->matriz[0][0];
    for (int i = 0; i < img->n_linhas; i++) {
        for (int j = 0; j < img->n_colunas; j++) {
            if (img->matriz[i][j] != primeiroPixel) {
                return 0; // Imagem não é uniforme
            }
        }
    }
    return 1; // Imagem é uniforme
}

// Função recursiva para codificar a imagem
void codificarImagem(Imagem* img) {
    // Se a imagem for uniforme, retornar 'P' (preta) ou 'B' (branca)
    if (imagemUniforme(img)) {
        if (img->matriz[0][0] == 1)
        {
            printf("P");
            return;
        }
        else
        {
            printf("B");
            return;
        }
    }

    // Caso a imagem não seja uniforme, realizar os cortes e dividir a imagem em 4 quadrantes


    // Iniciar o código com 'X' (pois não é uniforme)
    printf("X");

    // Realizar o corte Linhas e Colunas e processar cada quadrante
    int meioColunas = img->n_colunas / 2;
    int meioLinhas = img->n_linhas / 2;
    
    // Quadrante 1: Superior Esquerdo
    Imagem q1; 
    q1.n_colunas = meioColunas + (img->n_colunas % 2);
    q1.n_linhas = meioLinhas + (img->n_linhas % 2);

    for (int i = 0; i < meioLinhas; i++) {
        for (int j = 0; j < meioColunas; j++) {
            q1.matriz[i][j] = img->matriz[i][j];
        }
    }
    codificarImagem(&q1);

    // Quadrante 2: Superior Direito
    Imagem q2;
    q2.n_colunas = meioColunas;
    q2.n_linhas = meioLinhas + (img->n_linhas % 2);

    for (int i = meioLinhas / 2; i < img->n_linhas; i++) {
        for (int j = 0; j < meioColunas; j++) {
            q2.matriz[i][j] = img->matriz[i][meioColunas + j];
        }
    }
    codificarImagem(&q2);

    // Quadrante 3: Inferior Esquerdo
    Imagem q3;
    q3.n_colunas = meioColunas + (img->n_colunas % 2);
    q3.n_linhas = meioLinhas;

    for (int i = meioLinhas; i < img->n_linhas; i++) {
        for (int j = 0; j < meioColunas ; j++) {
            q3.matriz[i][j] = img->matriz[meioLinhas + i][j];
        }
    }
    codificarImagem(&q3);

    // Quadrante 4: Inferior Direito
    Imagem q4;
    q4.n_colunas = meioColunas;
    q4.n_linhas = meioLinhas;

    for (int i = meioLinhas; i < img->n_linhas; i++) {
        for (int j = meioColunas; j < img->n_colunas; j++) {
            q4.matriz[i][j] = img->matriz[meioLinhas + i][meioColunas + j];
        }
    }
    codificarImagem(&q4);
   
   return;
}

void help() {

    //Função para apresentar o menu help
    printf("Uso: ImageEncoder [-? | -m | -f ARQ]\n");
    printf("Codifica imagens binárias dadas em arquivos PBM ou por dados informados manualmente.\n");
    printf("Argumentos:\n");
    printf("-? , --help : apresenta essa orientação na tela.\n");
    printf("-m , --manual: ativa o modo de entrada manual, onde o usuário fornece todos os dados da imagem.\n");
    printf("-f , --file: considera o arquivo PBM para leitura.\n");

    return;
}

// Função principal
int main(int argc, char* argv[]) {

    Imagem img;

    // Verificando se há argumentos passados pela linha de comando
    if (argc == 1) {
        // Se nenhum argumento for passado, exibir ajuda
        help();
        return 0;
    }

    //Estrutura para opções de argumento
    const struct option opcoes[] = {
        {"help", no_argument, 0, '?'},
        {"manual", no_argument, 0, 'm'},
        {"file", required_argument, 0, 'f'},
        {0, 0, 0, 0} //Último item no vetor de opções precisa ser NULL
    };

    int opt;
    //Processar argumentos da CLI
    while ( (opt = getopt_long(argc, argv, "?mf:", opcoes, NULL)) > 0) {
        switch (opt) {
            case '?':
                help();
                return 0;
            case 'm':
                lerImagemManual(&img);
                break;
            case 'f':
                lerImagemDoArquivo(optarg, &img);
                break;
            default:
                help();
                return 1;
        }
    }

    // Codificando a imagem e mostrando o código
    codificarImagem(&img);
    printf("\n");
    return 0;
}

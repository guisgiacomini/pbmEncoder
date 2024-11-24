#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

// Definindo um tipo para representar a imagem binária como uma matriz de inteiros
typedef struct {
    int largura;  // Largura da imagem
    int altura;   // Altura da imagem
    int** pixels; // Matriz de pixels (0 para branco, 1 para preto)
} Imagem;

// Função para criar a estrutura de imagem
Imagem* criarImagem(int largura, int altura) {
    Imagem* img = (Imagem*)malloc(sizeof(Imagem));
    img->largura = largura;
    img->altura = altura;

    // Alocando memória para a matriz de pixels
    img->pixels = (int**)malloc(sizeof(int*) * altura);
    for (int i = 0; i < altura; i++) {
        img->pixels[i] = (int*)malloc(sizeof(int) * largura);
    }

    return img;
}

// Função para liberar a memória alocada para a imagem
void liberarImagem(Imagem* img) {
    for (int i = 0; i < img->altura; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img);
}

// Função para ler a imagem do arquivo PBM
Imagem* lerImagemDoArquivo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    char tipo[3];
    int largura, altura;

    // Lendo o tipo do arquivo (P1)
    fscanf(arquivo, "%s", tipo);
    if (strcmp(tipo, "P1") != 0) {
        printf("Formato de arquivo inválido. Apenas arquivos P1 são suportados.\n");
        fclose(arquivo);
        return NULL;
    }

    // Ignorando comentários (#)
    char ch;
    do {
        ch = fgetc(arquivo);
    } while (ch == '#');
    ungetc(ch, arquivo);

    // Lendo as dimensões da imagem
    fscanf(arquivo, "%d %d", &largura, &altura);

    // Criando a imagem
    Imagem* img = criarImagem(largura, altura);

    // Lendo os pixels
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            fscanf(arquivo, "%d", &img->pixels[i][j]);
        }
    }

    fclose(arquivo);
    return img;
}

// Função para ler os dados da imagem manualmente
Imagem* lerImagemManual() {
    int largura, altura;
    printf("Digite a largura e altura da imagem: ");
    scanf("%d %d", &largura, &altura);

    Imagem* img = criarImagem(largura, altura);

    printf("Digite os pixels da imagem (0 para branco e 1 para preto):\n");
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            scanf("%d", &img->pixels[i][j]);
        }
    }

    return img;
}

// Função para verificar se a imagem é uniforme (toda preta ou toda branca)
int imagemUniforme(Imagem* img) {
    int primeiroPixel = img->pixels[0][0];
    for (int i = 0; i < img->altura; i++) {
        for (int j = 0; j < img->largura; j++) {
            if (img->pixels[i][j] != primeiroPixel) {
                return 0; // Imagem não é uniforme
            }
        }
    }
    return 1; // Imagem é uniforme
}

// Função recursiva para codificar a imagem
char* codificarImagem(Imagem* img) {
    // Se a imagem for uniforme, retornar 'P' (preta) ou 'B' (branca)
    if (imagemUniforme(img)) {
        return img->pixels[0][0] == 1 ? "P" : "B";
    }

    // Caso a imagem não seja uniforme, realizar os cortes e dividir a imagem em 4 quadrantes
    char* codigo = (char*)malloc(10000 * sizeof(char)); // Reservando memória para o código resultante

    // Iniciar o código com 'X' (pois não é uniforme)
    strcat(codigo, "X");

    // Realizar o corte horizontal e vertical e processar cada quadrante
    int meioVertical = img->largura / 2;
    int meioHorizontal = img->altura / 2;

    // Quadrante 1: Superior Esquerdo
    Imagem* q1 = criarImagem(meioVertical + (img->largura % 2), meioHorizontal + (img->altura % 2));
    for (int i = 0; i < meioHorizontal + (img->altura % 2); i++) {
        for (int j = 0; j < meioVertical + (img->largura % 2); j++) {
            q1->pixels[i][j] = img->pixels[i][j];
        }
    }
    char* codQ1 = codificarImagem(q1);
    strcat(codigo, codQ1);
    liberarImagem(q1);

    // Quadrante 2: Superior Direito
    Imagem* q2 = criarImagem(img->largura - meioVertical, meioHorizontal + (img->altura % 2));
    for (int i = 0; i < meioHorizontal + (img->altura % 2); i++) {
        for (int j = 0; j < img->largura - meioVertical; j++) {
            q2->pixels[i][j] = img->pixels[i][meioVertical + j];
        }
    }
    char* codQ2 = codificarImagem(q2);
    strcat(codigo, codQ2);
    liberarImagem(q2);

    // Quadrante 3: Inferior Esquerdo
    Imagem* q3 = criarImagem(meioVertical + (img->largura % 2), img->altura - meioHorizontal);
    for (int i = 0; i < img->altura - meioHorizontal; i++) {
        for (int j = 0; j < meioVertical + (img->largura % 2); j++) {
            q3->pixels[i][j] = img->pixels[meioHorizontal + i][j];
        }
    }
    char* codQ3 = codificarImagem(q3);
    strcat(codigo, codQ3);
    liberarImagem(q3);

    // Quadrante 4: Inferior Direito
    Imagem* q4 = criarImagem(img->largura - meioVertical, img->altura - meioHorizontal);
    for (int i = 0; i < img->altura - meioHorizontal; i++) {
        for (int j = 0; j < img->largura - meioVertical; j++) {
            q4->pixels[i][j] = img->pixels[meioHorizontal + i][meioVertical + j];
        }
    }
    char* codQ4 = codificarImagem(q4);
    strcat(codigo, codQ4);
    liberarImagem(q4);

    return codigo;
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
    Imagem* img = NULL;

    // Verificando se há passados pela linha de comando
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
                img = lerImagemManual();
                break;
            case 'f':
                img = lerImagemDoArquivo(optarg);
                break;
            default:
                help();
                return 1;
        }
    }

    // Codificando a imagem e mostrando o código
    if (img) {
        char* codigo = codificarImagem(img);
        printf("Código da imagem: %s\n", codigo);
        free(codigo);
        liberarImagem(img);
    } else {
        printf("Erro ao carregar a imagem.\n");
        liberarImagem(img);
        return 1;
    }

    return 0;
}

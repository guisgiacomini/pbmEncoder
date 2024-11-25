#include <stdio.h>
#include <string.h>
#include <getopt.h>

#define MAX_LINHAS 768
#define MAX_COLUNAS 1024

// Função para verificar se a imagem é uniforme (toda preta ou toda branca)
int imagemUniforme(int n_colunas, int n_linhas, int img[n_linhas][n_colunas]) {
    int primeiroPixel = img[0][0];
    for (int i = 0; i < n_linhas; i++) {
        for (int j = 0; j < n_colunas; j++) {
            if (img[i][j] != primeiroPixel) {
                return 0; // Imagem não é uniforme
            }
        }
    }
    return 1; // Imagem é uniforme
}

// Função recursiva para codificar a imagem
void codificarImagem(int n_colunas, int n_linhas, int img[n_linhas][n_colunas]) {

    // Se a imagem for uniforme, retornar 'P' (preta) ou 'B' (branca)
    if (imagemUniforme(n_colunas, n_linhas, img)) {
        if (img[0][0] == 1)
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
    int meioColunas = n_colunas / 2;
    int meioLinhas = n_linhas / 2;
    
    int m1[meioLinhas + (n_linhas % 2)][meioColunas + (n_colunas % 2)]; // Quadrante superior esquerdo

    int m2[meioLinhas + (n_linhas % 2)][meioColunas];             // Quadrante superior direito

    int m3[meioLinhas][meioColunas + (n_colunas % 2)];             // Quadrante inferior esquerdo

    int m4[meioLinhas][meioColunas];                         // Quadrante inferior direito

    // Define os quadrantes

    //m1
    for (int i = 0; i < meioLinhas + (n_linhas % 2); i++) {
      for (int j = 0; j < meioColunas + (n_colunas % 2); j++) {
        m1[i][j] = img[i][j];
      }
    }
    codificarImagem(meioColunas + (n_colunas % 2), meioLinhas + (n_linhas % 2),  m1);

    //m2
    if(n_colunas > 1){
      for (int i = 0; i < meioLinhas + (n_linhas % 2); i++) {
        for (int j = meioColunas + (n_colunas % 2); j < n_colunas; j++) {
          m2[i][j - meioColunas - (n_colunas % 2)] = img[i][j];
        }
      }
      codificarImagem( meioColunas, meioLinhas + (n_linhas % 2), m2);
    }


    //m3
    if(n_linhas > 1){
      for (int i = meioLinhas + (n_linhas % 2); i < n_linhas; i++) {
        for (int j = 0; j < meioColunas + (n_colunas % 2); j++) {
          m3[i - meioLinhas - (n_linhas % 2)][j] = img[i][j];
        }
      }
      codificarImagem( meioColunas + (n_colunas % 2), meioLinhas, m3);
    }

    //m4
    if(n_linhas > 1 && n_colunas > 1){
      for (int i = meioLinhas + (n_linhas % 2); i < n_linhas; i++) {
        for (int j = meioColunas + (n_colunas % 2); j < n_colunas; j++) {
          m4[i - meioLinhas - (n_linhas % 2)][j - meioColunas - (n_colunas % 2)] = img[i][j];
        }
      } 
      codificarImagem(meioColunas, meioLinhas,  m4);
    }
   
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
        if (opt == '?') {
            help();
            return 0;
        }
        else if (opt == 'm')
        {
            int n_colunas, n_linhas;
            printf("Digite a colunas e linhas da imagem: ");
            scanf("%d %d", &n_colunas, &n_linhas);

            //Sair caso ultrapasse número máximo de linhas e colunas
            if (n_linhas > MAX_LINHAS || n_colunas > MAX_COLUNAS)
            {
                printf("Tamanho max: 1024x768");
                return 0;
            }

                // Criando a imagem
                int imagem[n_linhas][n_colunas];

            printf("Digite os matriz da imagem (0 para branco e 1 para preto):\n");
            for (int i = 0; i < n_linhas; i++) {
                for (int j = 0; j < n_colunas; j++) {
                    scanf("%d", &imagem[i][j]);
                }
            }

            codificarImagem(n_colunas, n_linhas, imagem);
        }
        else if (opt == 'f')
        {
            //Lendo arquivo passado na linha de comando
                FILE* arquivo = fopen(optarg, "r");
                if (!arquivo) {
                    printf("Erro ao abrir o arquivo.\n");
                    return 1;
                }

                char tipo[3];
                int n_colunas, n_linhas;

                // Lendo o tipo do arquivo (P1)
                fscanf(arquivo, "%s", tipo);
                if (strcmp(tipo, "P1") != 0) {
                    printf("Formato de arquivo inválido. Apenas arquivos P1 são suportados.\n");
                    fclose(arquivo);
                    return 1;
                }

                // Lendo as dimensões da imagem
                fscanf(arquivo, "%d %d", &n_colunas, &n_linhas);

                //Sair caso ultrapasse número máximo de linhas e colunas
                if (n_linhas > MAX_LINHAS || n_colunas > MAX_COLUNAS)
                {
                    printf("Tamanho max: 1024x768");
                    return 1;
                }
                // Criando a imagem
                int imagem[n_linhas][n_colunas];

                // Lendo a matriz
                for (int i = 0; i < n_linhas; i++) {
                    for (int j = 0; j < n_colunas; j++) {
                        fscanf(arquivo, "%d", &imagem[i][j]);
                    }
                }

                fclose(arquivo);

                //Codificar imagem
                codificarImagem(n_colunas, n_linhas, imagem);
        }      
        else
        {
            help();
            return 1;
        }
    }
        
        printf("\n");
        return 0;
    }

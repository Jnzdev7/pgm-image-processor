#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define LARGURA 512
#define ALTURA  512

int imagem_corrente[ALTURA][LARGURA];
int largura_atual = 0, altura_atual = 0;

void imprimirMenu();
int  lerImagem(int imagem[ALTURA][LARGURA], char nome_arquivo[256]);
void salvarImagem(int imagem[ALTURA][LARGURA], char nome_arquivo[256]);
void preencherImagemListras(int imagem[ALTURA][LARGURA]);
void preencherImagemAleatoria(int imagem[ALTURA][LARGURA]);
void limiarizarImagem(int imagem_entrada[ALTURA][LARGURA], int imagem_saida[ALTURA][LARGURA], int limiar);
int  valorMedioImagem(int imagem[ALTURA][LARGURA]);
int  valorFrequenteImagem(int imagem[ALTURA][LARGURA]);
void filtroMedia(int imagem_entrada[ALTURA][LARGURA], int imagem_saida[ALTURA][LARGURA]);
static int read_next_int(FILE *f, int *out);

int main() {
    int opcao;
    char nome_arquivo[256];

    srand((unsigned)time(NULL));

    do {
        imprimirMenu();
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            opcao = -1;
        }

        if (opcao >= 5 && opcao <= 8 && (largura_atual == 0 || altura_atual == 0)) {
            printf("Erro: Carregue uma imagem primeiro (Opcao 1).\n");
        } else {
            switch (opcao) {
                case 1: {
                    printf("Digite o nome do arquivo para carregar (formato .pgm): ");
                    scanf("%255s", nome_arquivo);
                    if (lerImagem(imagem_corrente, nome_arquivo)) {
                        printf("Imagem '%s' carregada com sucesso (%dx%d).\n", nome_arquivo, largura_atual, altura_atual);
                    } else {
                        printf("Falha ao carregar a imagem '%s'.\n", nome_arquivo);
                    }
                    break;
                }
                case 2: {
                    if (largura_atual == 0 || altura_atual == 0) {
                        printf("Nenhuma imagem para salvar. Carregue ou crie uma primeiro.\n");
                        break;
                    }
                    printf("Digite o nome do arquivo para salvar (formato .pgm): ");
                    scanf("%255s", nome_arquivo);
                    salvarImagem(imagem_corrente, nome_arquivo);
                    break;
                }
                case 3: {
                    preencherImagemListras(imagem_corrente);
                    printf("Imagem preenchida com listras horizontais.\n");
                    break;
                }
                case 4: {
                    preencherImagemAleatoria(imagem_corrente);
                    printf("Imagem preenchida com valores aleatorios.\n");
                    break;
                }
                case 5: {
                    int limiar;
                    printf("Digite um valor de limiar (0-255): ");
                    scanf("%d", &limiar);

                    if (limiar < 0 || limiar > 255) {
                        printf("Valor de limiar invalido. Deve estar entre 0 e 255.\n");
                    } else {
                        int imagem_saida[ALTURA][LARGURA];
                        limiarizarImagem(imagem_corrente, imagem_saida, limiar);
                        for(int i = 0; i < altura_atual; i++) {
                            for(int j = 0; j < largura_atual; j++) {
                                imagem_corrente[i][j] = imagem_saida[i][j];
                            }
                        }
                        printf("Imagem limiarizada com o valor %d.\n", limiar);
                    }
                    break;
                }
                case 6: {
                    int limiar_medio = valorMedioImagem(imagem_corrente);
                    printf("O valor medio dos pixels e: %d\n", limiar_medio);

                    int imagem_saida[ALTURA][LARGURA];
                    limiarizarImagem(imagem_corrente, imagem_saida, limiar_medio);
                    for(int i = 0; i < altura_atual; i++) {
                        for(int j = 0; j < largura_atual; j++) {
                            imagem_corrente[i][j] = imagem_saida[i][j];
                        }
                    }
                    printf("Imagem limiarizada com o valor medio.\n");
                    break;
                }
                case 7: {
                    int limiar_frequente = valorFrequenteImagem(imagem_corrente);
                    printf("O valor de pixel mais frequente e: %d\n", limiar_frequente);
                    
                    int imagem_saida[ALTURA][LARGURA];
                    limiarizarImagem(imagem_corrente, imagem_saida, limiar_frequente);
                    for(int i = 0; i < altura_atual; i++) {
                        for(int j = 0; j < largura_atual; j++) {
                            imagem_corrente[i][j] = imagem_saida[i][j];
                        }
                    }
                    printf("Imagem limiarizada com o valor mais frequente.\n");
                    break;
                }
                case 8: {
                    int imagem_saida[ALTURA][LARGURA];
                    filtroMedia(imagem_corrente, imagem_saida);
                    for(int i = 0; i < altura_atual; i++) {
                        for(int j = 0; j < largura_atual; j++) {
                            imagem_corrente[i][j] = imagem_saida[i][j];
                        }
                    }
                    printf("Filtro da media aplicado.\n");
                    break;
                }
                case 9:
                    printf("Saindo do programa...\n");
                    break;
                default:
                    printf("Opcao invalida! Tente novamente.\n");
                    break;
            }
        }

        if (opcao != 9) {
            printf("\nPressione Enter para continuar...");
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            getchar();
        }

    } while (opcao != 9);

    return 0;
}

void imprimirMenu() {
    system("clear || cls");
    printf("--- Processador de Imagens ---\n");
    printf("1: Carregar imagem\n");
    printf("2: Salvar imagem\n");
    printf("3: Preencher imagem com listras\n");
    printf("4: Preencher imagem com valores aleatorios\n");
    printf("5: Limiarizar com valor do usuario\n");
    printf("6: Limiarizar com valor medio\n");
    printf("7: Limiarizar com valor frequente\n");
    printf("8: Filtro da media\n");
    printf("9: Sair do programa\n");
    printf("------------------------------\n");
}

void limiarizarImagem(int imagem_entrada[ALTURA][LARGURA], int imagem_saida[ALTURA][LARGURA], int limiar) {
    for (int i = 0; i < altura_atual; i++) {
        for (int j = 0; j < largura_atual; j++) {
            if (imagem_entrada[i][j] > limiar) {
                imagem_saida[i][j] = 255;
            } else {
                imagem_saida[i][j] = 0;
            }
        }
    }
}

int valorMedioImagem(int imagem[ALTURA][LARGURA]) {
    long long soma = 0;
    for (int i = 0; i < altura_atual; i++) {
        for (int j = 0; j < largura_atual; j++) {
            soma += imagem[i][j];
        }
    }
    return (int)(soma / (altura_atual * largura_atual));
}

int valorFrequenteImagem(int imagem[ALTURA][LARGURA]) {
    int frequencia[256] = {0};

    for (int i = 0; i < altura_atual; i++) {
        for (int j = 0; j < largura_atual; j++) {
            frequencia[imagem[i][j]]++;
        }
    }

    int mais_frequente = 0;
    int max_contagem = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencia[i] > max_contagem) {
            max_contagem = frequencia[i];
            mais_frequente = i;
        }
    }
    return mais_frequente;
}

void filtroMedia(int imagem_entrada[ALTURA][LARGURA], int imagem_saida[ALTURA][LARGURA]) {
    for(int i = 0; i < altura_atual; i++) {
        for(int j = 0; j < largura_atual; j++) {
            imagem_saida[i][j] = imagem_entrada[i][j];
        }
    }

    for (int i = 2; i < altura_atual - 2; i++) {
        for (int j = 2; j < largura_atual - 2; j++) {
            int soma = 0;
            for (int y = -2; y <= 2; y++) {
                for (int x = -2; x <= 2; x++) {
                    soma += imagem_entrada[i + y][j + x];
                }
            }
            imagem_saida[i][j] = soma / 25;
        }
    }
}

int lerImagem(int imagem[ALTURA][LARGURA], char nome_arquivo[256]) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro: Arquivo '%s' nao encontrado.\n", nome_arquivo);
        return 0;
    }

    char tipo[3];
    fscanf(arquivo, "%2s", tipo);
    if (tipo[0] != 'P' || tipo[1] != '2') {
        printf("Erro: Formato de arquivo invalido. Apenas P2 (PGM ASCII) e suportado.\n");
        fclose(arquivo);
        return 0;
    }

    int largura, altura, max_val;

    if (!read_next_int(arquivo, &largura) ||
        !read_next_int(arquivo, &altura) ||
        !read_next_int(arquivo, &max_val)) {
        printf("Erro: Cabecalho PGM invalido.\n");
        fclose(arquivo);
        return 0;
    }

    if (largura > LARGURA || altura > ALTURA) {
        printf("Erro: imagem %dx%d excede o limite de %dx%d.\n", largura, altura, LARGURA, ALTURA);
        fclose(arquivo);
        return 0;
    }

    largura_atual = largura;
    altura_atual = altura;

    for (int i = 0; i < altura_atual; i++) {
        for (int j = 0; j < largura_atual; j++) {
            if (!read_next_int(arquivo, &imagem[i][j])) {
                printf("Erro: Dados da imagem incompletos ou invalidos.\n");
                fclose(arquivo);
                return 0;
            }
        }
    }

    fclose(arquivo);
    return 1;
}

void salvarImagem(int imagem[ALTURA][LARGURA], char nome_arquivo[256]) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao criar o arquivo '%s'.\n", nome_arquivo);
        return;
    }

    fprintf(arquivo, "P2\n%d %d\n255\n", largura_atual, altura_atual);

    for (int i = 0; i < altura_atual; i++) {
        for (int j = 0; j < largura_atual; j++) {
            fprintf(arquivo, "%d ", imagem[i][j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Imagem salva com sucesso em '%s'.\n", nome_arquivo);
}

void preencherImagemListras(int imagem[ALTURA][LARGURA]) {
    largura_atual = LARGURA;
    altura_atual = ALTURA;
    for (int i = 0; i < ALTURA; i++) {
        int valor = ((i / 64) % 2 == 0) ? 0 : 255;
        for (int j = 0; j < LARGURA; j++) {
            imagem[i][j] = valor;
        }
    }
}

void preencherImagemAleatoria(int imagem[ALTURA][LARGURA]) {
    largura_atual = LARGURA;
    altura_atual = ALTURA;
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < LARGURA; j++) {
            imagem[i][j] = rand() % 256;
        }
    }
}

static int read_next_int(FILE *f, int *out) {
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (isspace(c)) continue;
        if (c == '#') {
            while ((c = fgetc(f)) != EOF && c != '\n');
            continue;
        }
        ungetc(c, f);
        break;
    }
    if (c == EOF) return 0;
    return fscanf(f, "%d", out) == 1;
}

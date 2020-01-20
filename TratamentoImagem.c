 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
typedef struct PIXEL{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} PIXEL;

typedef struct IMAGE{
	int largura, altura;
    int max;
    PIXEL **matrizimagem; // matriz que recebe os pixels da imagem (camada red, green e blue).
} IMAGE;

// Recebe inteiros M e N que representam a largura
// e a altura da imagem e aloca memoria para uma
// matriz de dimencoes (N x M)
unsigned char** aloca(int m, int n)
{
    int i;
    unsigned char **M;
    // criar um array de ponteiros (char *)
    M = (unsigned char**) malloc(n * sizeof(unsigned char*));
    for(i = 0; i < n; i++)
    {   
        // criar um array de char
        M[i] = (unsigned char*) malloc(m * sizeof(unsigned char));
    }
    return M;
}

//m = largura;
//n = altura;
PIXEL** aloca2(int m, int n){
    int i = 0;
    PIXEL **RGB = (PIXEL**)malloc(n*sizeof(PIXEL*));
	    for(i=0;i<n;i++){						
		    RGB[i] = (PIXEL*)malloc(m*sizeof(PIXEL));
	    }
    return RGB;
}

void escreve_imagem(IMAGE imagem, FILE *pimage, char nome[50]){
    int i, j;

    pimage = fopen(nome, "w+");
    fprintf(pimage, "P6\n");
    fprintf(pimage, "%d %d\n  %d\n", imagem.largura, imagem.altura, imagem.max);

    for (i = 0; i < imagem.altura; i++){
        for (j = 0; j < imagem.largura; j++){
            fprintf(pimage, "%c%c%c", (char) imagem.matrizimagem[i][j].green,
                                            (char) imagem.matrizimagem[i][j].blue,
                                            (char) imagem.matrizimagem[i][j].red);
        }
    }
    fclose(pimage);
}

// Método da combinacao linear
int combLinear(PIXEL *pixel)
{
    int new;
    new = 0.3*(pixel -> red) + 0.59* (pixel -> green) + 0.11* (pixel -> blue);
    return new;
}

// Função responsável por rotacionar a imagem 90 graus à direita.
IMAGE rotate_90_clockwise(IMAGE image){
	PIXEL **rotated_pixels;
    rotated_pixels = aloca2(image.largura, image.altura);
	
	for (int i = 0; i < image.altura; i++){
		for (int j = 0; j < image.largura; j++){
            // Pega as colunas da matriz e transforma em linhas de baixo para cima da esquerda para direita.
			rotated_pixels[i][j].red = image.matrizimagem[image.altura - 1 - j][i].red;
            rotated_pixels[i][j].green = image.matrizimagem[image.altura - 1 - j][i].green;
            rotated_pixels[i][j].blue = image.matrizimagem[image.altura - 1 - j][i].blue;
		}
	}
	
	IMAGE rotated_image = {image.largura, image.altura, image.max, rotated_pixels};
	
	return rotated_image;
}

// Função responsável por rotacionar a imagem 90 graus à esquerda.
IMAGE rotate_90_conterclockwise(IMAGE image){
	PIXEL **rotated_pixels;
    rotated_pixels = aloca2(image.largura, image.altura);
	
	for (int i = 0; i < image.altura; i++){
		for (int j = 0; j < image.largura; j++){
            // Pega as colunas da matriz e transforma em linhas de cima para baixo da direita para esquerda.
			rotated_pixels[i][j].red = image.matrizimagem[j][image.largura - 1 - i].red;
            rotated_pixels[i][j].green = image.matrizimagem[j][image.largura - 1 - i].green;
            rotated_pixels[i][j].blue = image.matrizimagem[j][image.largura - 1 - i].blue;
		}
	}
	
	IMAGE rotated_image = {image.largura, image.altura, image.max, rotated_pixels};
	
	return rotated_image;
}
// Função responsável por espelhar a imagem verticalmente.
IMAGE vertical_mirroring(IMAGE image){
    PIXEL **rotated_pixels;
    rotated_pixels = aloca2(image.largura, image.altura);
	
	for (int i = 0; i < image.altura; i++){
		for (int j = 0; j < image.largura; j++){
			rotated_pixels[i][j].red = image.matrizimagem[image.largura - 1 - i][j].red;
            rotated_pixels[i][j].green = image.matrizimagem[image.largura - 1 - i][j].green;
            rotated_pixels[i][j].blue = image.matrizimagem[image.largura - 1 - i][j].blue;
		}
	}
	
	IMAGE vertical_mirror = {image.largura, image.altura, image.max, rotated_pixels};
	return vertical_mirror;
}

IMAGE horizontal_mirroring(IMAGE image){
    PIXEL **rotated_pixels;
    rotated_pixels = aloca2(image.largura, image.altura);
	
	for (int i = 0; i < image.altura; i++){
		for (int j = 0; j < image.largura; j++){
			rotated_pixels[i][j].red = image.matrizimagem[i][image.altura - 1 - j].red;
            rotated_pixels[i][j].green = image.matrizimagem[i][image.altura - 1 - j].green;
            rotated_pixels[i][j].blue = image.matrizimagem[i][image.altura - 1 - j].blue;
		}
	}
	
	IMAGE horizontal_mirror = {image.largura, image.altura, image.max, rotated_pixels};
	return horizontal_mirror;
}

int main()
{
    FILE *image; // Ponteiro para o arquivo que contem a imagem que vamos tratar.
    FILE *copyimage; // Ponteiro para o arquivo que contém a cópia da imagem.
    FILE *grayscale; // Ponteiro para o arquivo onde vamos salvar a imagem em escala de cinza.
    FILE *image90right; // Ponteiro para o arquivo que contém a imagem rotacionada no sentido horário.
    FILE *image90left; // Ponteiro para o arquivo que contém a imagem rotacionada no sentido anti-horário.
    FILE *imagemvertical; // Ponteiro para o arquivo que contém imagem espelhada verticalmente. 
    FILE *imagemhorizontal; // Pinteiro para o arquivo que contém a imagem espelhada horizontalmente.
    char image_name[256];
    char key[128];
    PIXEL pixel;
    IMAGE imagem;
    int i, j;
  
    printf("Digite o nome do arquivo PPM de entrada: ");
    scanf("%s", image_name); // Le o nome do arquivo de entrada
  
    image = fopen(image_name , "r") ; // Abre o arquivo no modo leitura
    if(image == NULL) // Verificase o arquivo existe e foi aberto
    {
        printf("Erro na abertura do arquivo %s\n", image_name);
        return 0 ;
    }
  
    // Le dados do cabeçalho
    fscanf(image, "%s", key);
  
    // Verifica se a imagem esta em PPM
    if(strcmp(key,"P6") != 0)
    {
        printf("Arquivo nao e um PPM\n") ;
        fclose(image) ;
        return 0 ;
    }
  
    // Le os outros dados do cabecalho
    fscanf(image, "%d %d %d", &imagem.largura, &imagem.altura, &imagem.max);
    // imagem.largura guarda a largura da imagem (colunas da matriz).
    // imagem.altura guarda a altura da imagem (linhas da matriz).
    // imagem.max guarda o valor máximo da escala.
  
    // Matriz para guardar a imagem em tons de cinza.
    unsigned char **grayImage;
    grayImage = aloca(imagem.largura, imagem.altura);

    // matriz para guardar as camadas red, green, e blue da imagem.
    imagem.matrizimagem = aloca2(imagem.largura, imagem.altura);

    // Lê os canais de cores de cada pixel ij da imagem
    // e salva na matriz grayImage o seu correspondente
    // em cinza
    for(i = 0; i < imagem.altura; i++)
    {
        for(j = 0; j < imagem.largura; j++)
        {
            fscanf(image, "%c%c%c", &pixel.red, &pixel.green, &pixel.blue);
            imagem.matrizimagem[i][j].red = pixel.red;
            imagem.matrizimagem[i][j].green = pixel.green;
            imagem.matrizimagem[i][j].blue = pixel.blue;
            grayImage[i][j] = combLinear(&pixel);
        }
    }

    // Escreve um arquivo da imagem em escala de cinza.
    grayscale = fopen("escaladecinza.ppm" , "w"); // Abre o arquivo no modo escrita
    fprintf(grayscale,"P5\n"); // Escreve o cabecalho arquivo PGM
    fprintf(grayscale, "%d %d\n %d\n", imagem.largura, imagem.altura, imagem.max); // Escreve o cabecalho
  
    for(i = 0; i < imagem.altura; i++)
    {
        for(j = 0; j < imagem.largura; j++)
        {
            // Escreve os valores de cada pixel no arquivo
            fprintf(grayscale, "%c", (char) grayImage[i][j]);
        }
    }

    escreve_imagem(imagem, copyimage, "copiaimagem.ppm"); // Chamada da função que faz a cópia do arquivo.
  
    IMAGE imagem_rotate_right;  // Variável do tipo estrutura IMAGE que recebe a imagem rotacionada 90 graus à direita.
    imagem_rotate_right = rotate_90_clockwise(imagem);
    escreve_imagem(imagem_rotate_right, image90right, "imagem90direita.ppm");

    IMAGE imagem_rotate_left; // Variável do tipo estrutura IMAGE que recebe a imagem rotacionada 90 graus à esquerda.
    imagem_rotate_left = rotate_90_conterclockwise(imagem);
    escreve_imagem(imagem_rotate_left, image90left, "imagem90esquerda.ppm");

    IMAGE imagem_mirror_vertical; // Variável do tipo estrutura IMAGE que recebe a imagem espelhada verticalmente.
    imagem_mirror_vertical = vertical_mirroring(imagem); 
    escreve_imagem(imagem_mirror_vertical, imagemvertical,"imagemespelhadavertical.ppm");

    IMAGE imagem_mirror_horizontal;
    imagem_mirror_horizontal = horizontal_mirroring(imagem);
    escreve_imagem(imagem_mirror_horizontal, imagemhorizontal, "imagemespelhadahorizontal.ppm");
    
    fclose(image); // Fecha o arquivo da imagem original
    fclose(grayscale); // Fecha o arquivo da nova imagem

// Libera a mémoria alocada para a matriz grayimage, matrizimagem.
    for(i = 0; i < imagem.altura; i++)
    {
        free(grayImage[i]);
        free(imagem.matrizimagem[i]);
    }
    free(grayImage);
    return 0;
}

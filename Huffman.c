#include "Huffman.h"

void inicializaVariaveis()
{
    nNos = 0;
    fimFila = 1;
    fila = filaAux - 1;
    taxaCompressao = 0.0;
}

No *novoNo(int freq, char c, No *esq, No *dir)
{
    No *no = arvore + nNos++;
	
    if(freq){ //se frequencia != 0 guarda a letra e a frequencia no nó
      no->c = c; 
      no->freq = freq;
    }
    else{ //se não percorre a arvore 
      no->esq = esq; 
      no->dir = dir;
      no->freq = esq->freq + dir->freq;  // e a frequencia do nó (que nao é folha) é igual a soma das frequencias dos nós filhos
    }
	
    return no;
}
 

void inserirFila(No *no) //insere um nó na fila de prioridades
{
    int posicaoAtual, i; 
    
    i = fimFila++;
    posicaoAtual = i/2;
    
    while (posicaoAtual) {
        if(fila[posicaoAtual]->freq <= no->freq) //Compara as frequencias no vetor e no nó
            break; // se a frequencia é menor ou igual saimos do loop
        
        fila[i] = fila[posicaoAtual]; //se a frequencia for maior tem mais prioridade e portanto passamos para uma posicao menor no vetor
        
        i = posicaoAtual; 
        posicaoAtual = i/2; //j recebe i/2 (posicao atual da fila -1)
    }
    
    fila[i] = no; //nó é colocado na respectiva posicao dependendo da fila de prioridade
}
 
No *removeFila() //retira o nó da fila de prioridades
{
    int i = 1, posicaoAtual;
    No *no; 
    no = fila[i]; //nó recebe a maior frequencia da fila (1ª posicao)
    
    if (fimFila < 2) //se a fila tem apenas um elemento ou nenhum
        return 0; //abortar missão
	
    fimFila--;
    posicaoAtual = i*2; //posicaoAtual agora é 2
    
    while (posicaoAtual < fimFila) { //Enquanto posicao atual for menor do que o fimFila
        if ((posicaoAtual+1 < fimFila) && (fila[posicaoAtual+1]->freq < fila[posicaoAtual]->freq))  // se posicaoAtual não é a ultima posicao
            posicaoAtual++;                                                                         //E se a frequencia na posicao atual+1 é menor
                                                                                                    //que na posicaoAtual incrementamos a posicaoAtual
        fila[i] = fila[posicaoAtual]; //se é maiou ou igual fila[i] recebe o elemento de fila[posicaoAtual]
        i = posicaoAtual;
        posicaoAtual = i*2;
	}
	
    fila[i] = fila[fimFila];
    return no;
}
 
void codigoHuffman(No *no, char *str, int tam)//coloca 0s e 1s nas "arestas" das arvores
{
    static char *saida = buffer;
    
    if (no->c){ //se é um nó folha
        str[tam] = 0;
        strcpy(saida, str);
	codigo[no->c] = saida; //o valor da variavel saida é guardada na posicao equivalente ao codigo ascII da letra do nó
        saida += tam+1;
	return;
	}
 
    //se não é folha percorre o nó da direita e logo após o nó da esquerda
    str[tam] = '0'; //guarda 0 no vetor str[tam] - percorre o nó a esqueda e incrementa [tam]
    codigoHuffman(no->esq, str,tam+1);
    str[tam] = '1'; //guarda 1 no vetor str[tam]- percorre o nó a direita e incrementa [tam]
    codigoHuffman(no->dir,str,tam+1);
}
 
void inicializa(const char *str)
{
    int i, frequencia[128] = {0};
    char c[16];
    float tamanhoOriginal = 0.0;
    float tamanhoCompactado = 0.0;
    
    
    tamanhoOriginal = strlen(str)*8;
    
    while (*str){ //Enquanto houverem letras -- ((*s) guarda o código ASCII correspondente da letra atual)
        frequencia[(int)*str]++; //guarda no vetor (incrementado o valor na posicao[*s]) a quantidade de vezes que a letra aparece(guarda na posicao correspondente ao codigo ASCII da letra)
        *str++;//Próxima letra
    }
 
    for (i = 0; i < 128; i++)//128 (tamanho do alfabeto ASCII)
        if (frequencia[i]) //Se existe frequencia na posicao atual
            inserirFila(novoNo(frequencia[i], i, NULL, NULL));//inserimos a posicao atual i corresponde ao código ascII da letra
 
    while (fimFila > 2) 
        inserirFila(novoNo(NULL, NULL, removeFila(), removeFila())); //se a fila possui tamanho tiramos os nós da fila e montamos a arvore
 
    codigoHuffman(fila[1], c, 0); //percorre a arvore colocando 0s e 1s
    
    //Calcula a Taxa de compressão do texto
    i = 0;
    while(i<128){
        if(codigo[i]){
            tamanhoCompactado += strlen(codigo[i])*frequencia[i];
        }        
        i++;
    }
    
    taxaCompressao = ((float)(tamanhoOriginal-tamanhoCompactado)/tamanhoOriginal);
}
 
void codifica(const char *str, char *saida,FILE *arqSaida)
{
    while (*str) { //Enquanto houverem letras
        strcpy(saida, codigo[*str]); //copiamos o código equivalente a letra para 'saida'
	saida += strlen(codigo[*str]); //saida recebe o código de 'saida' mais o código da letra atual
        *str++;//próxima letra
    }
}
 
void decodifica(const char *str, No *no)
{
    No *aux = no;
    
    while (*str) {
        if (*str++ == '0')  //se a aresta vale 0 vai pro nó a esquerda
            aux = aux->esq;
        else //se não vai pro nó a direita
            aux = aux->dir;
        
        if (aux->c){ //se é nó folha
            printf("%c",aux->c); //imprime o caracter do nó
            aux = no;//aux recebe o nó atual
        }
    }
 
    printf("\n");
    
    if (no != aux) 
        printf("Lixo de memória !\n");
}

void menu(char *buf)
{
    FILE *arq,*arqSaida;
    int i,j,opc = 1;
    char *str,ch;
    char caminho[70],caminhoSaida[70];
    float porcentagemTaxa = 0.0;
    
    printf("Digite o caminho ate o texto que voce deseja comprimir\n");
    scanf("%s",caminho);
    fflush(stdin);
    printf("Digite o nome do arquivo de saida\n");
    scanf("%s",caminhoSaida);
    
    arq = fopen(caminho,"r");
    arqSaida = fopen(caminhoSaida,"wb");
    
    str = (char*)malloc(sizeof(char)*256);
    
    inicializaVariaveis();
    
    if(arq){        
        j=0;
        while((ch = fgetc(arq)) != EOF){ //Lendo o arquivo letra a letra
            str[j] = ch;
            j++;
        }
        
        inicializa(str);

        while(opc != 0){
            fflush(stdin);
            printf("1- Imprimir alfabeto codificado | 2- Comprimir Texto | 3- Descomprimir Texto | 4- Taxa de compressão do texto |0- Sair\n");
            scanf("%d",&opc);
            
            switch(opc){
                case 1:
                    for (i = 0; i < 128; i++)
                        if (codigo[i]) 
                            printf("'%c': %s\n", i, codigo[i]);
                    break;
                case 2:
                    codifica(str,buf,arqSaida); 
                    fprintf(arqSaida,"%s",buf);
                    printf("\nTexto comprimido: %s\n\n",buf);
                break;
                case 3:
                    printf("\nTexto descomprimido: ");
                    decodifica(buf, fila[1]);
                    break;
                case 4:
                    porcentagemTaxa = (float)(taxaCompressao*100);
                    printf("taxa de Compressao: %.2f%\n",porcentagemTaxa);
                    break;
                default:
                    printf("Essa função não existe\n");
                    break;
            }
        }
	fclose(arq);
        fclose(arqSaida);
    }
    else{
        printf("Arquivo nao encontrado !\n");
    }
}
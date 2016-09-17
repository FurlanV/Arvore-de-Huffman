#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  int freq;
  char c;  
  struct No *esq; 
  struct No *dir;
}No;

No *novoNo(int freq, char c, No *a, No *b);
No *removeFila();
void inserirFila(No *no);
void codigoHuffman(No *no,char *s,int tam);
void inicializa(const char *c);
void codifica(const char *c,char *saida,FILE *arqSaida);
void decodifica(const char *s, No *t);
 
No arvore[256];
No *filaAux[255], **fila;
int nNos, fimFila;
char *codigo[128],buffer[1024];
float taxaCompressao;
#ifndef ALBERI_H
#define ALBERI_H
#include "liste.h"

typedef struct nodoAlbero
{
    int data;
    struct nodoAlbero* sx;
    struct nodoAlbero* dx;
}nodoAlbero;

//ALBERI BINARI SEMPLICI

nodoAlbero* costruisciAlbero(int data, nodoAlbero* FiglioSx, nodoAlbero* figlioDx);
void stampa_PreOrder(nodoAlbero* radice);
void stampa_InOrder(nodoAlbero* radice);
void stampa_PostOrder(nodoAlbero* radice);

//ALBERI BINARI DI RICERCA

nodoAlbero* inserisciInABR(nodoAlbero* radice, int data);
int ricercaABR(nodoAlbero* radice, int data);
int minimoABR(nodoAlbero* radice);

//FUNZIONI ESERCIZI

nodoAlbero* sottraiMinimoABR(nodoAlbero* albero);
nodoLista* chiaviLivelliPariABR(nodoAlbero* albero,int livello);

#endif
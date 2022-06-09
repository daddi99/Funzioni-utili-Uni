#ifndef LISTE_H
#define LISTE_H

typedef struct nodoLista
{
    int data;
    struct nodoLista* next;
}nodoLista;

typedef struct nodoLista_DL
{
    struct nodoLista_DL* next;
    struct nodoLista_DL* prev;
    int data;
}nodoLista_DL;

//LISTE SEMPLICI

nodoLista* inserisciInTesta(nodoLista* lista, int data);
void stampaLista(nodoLista* lista);
nodoLista* inserisciInCoda(nodoLista* lista, int data);
nodoLista* creaNodoLista(int data);
nodoLista* creaListaDaVettore(int elementi[], int dimensioneVettore);
int ricercaInLista(nodoLista* lista, int data);
nodoLista* rimuoviElemento(nodoLista* lista, int data);


//LISTE DOPPIAMENTE PUNTATE 

void stampaLista_DL(nodoLista_DL* lista);
nodoLista_DL* inserisciInCoda_DL(nodoLista_DL* lista, int data);
nodoLista_DL* rimuoviElemento_DL(nodoLista_DL* lista, int data);

#endif
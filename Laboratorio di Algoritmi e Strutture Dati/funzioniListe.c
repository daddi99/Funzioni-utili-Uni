#include "liste.h"
#include <stdio.h>
#include <malloc.h>

//Crea un nodoLista con il dato in input
nodoLista* creanodoLista(int data)
{
    nodoLista* tmp = (nodoLista*)malloc(sizeof(nodoLista));
    tmp->data = data;
    tmp->next = NULL;

    return tmp;
}

//Inserisce un nuovo nodoLista in testa alla lista (Quindi la lista verrà fuori al contrario)
nodoLista* inserisciInTesta(nodoLista* lista, int data)
{
    nodoLista* tmp = creanodoLista(data);
    
    tmp->next = lista;
    lista = tmp;

    return lista;
}

//Scorre ricorsivamente la lista ed inserisce un elemento in coda (L'ordine di inserimento è corretto)
nodoLista* inserisciInCoda(nodoLista* lista, int data)
{
    if(lista == NULL)
        return creanodoLista(data);    
    else
    {
        lista->next = inserisciInCoda(lista->next,data);
        return lista;
    }
}

//Funzione ricorsiva che stampa la lista in input
void stampaLista(nodoLista* lista)
{
    if(lista == NULL)
        printf("NULL");
    else
    {
        printf("%d -> ",lista->data);
        stampaLista(lista->next);
    }    
}

//Restituisce una lista con esattemente gli stessi elementi e nello stesso ordine del vettore in input.
nodoLista* creaListaDaVettore(int elementi[],int dimensioneVettore)
{
    nodoLista* lista = NULL;

    for(int i=0; i<dimensioneVettore; i++)
    {
        lista = inserisciInCoda(lista,elementi[i]);
    }

    return lista;
}

//Scorre ricorsivamente la lista: se trova l'elemento ritorna 1 altrimenti 0.
int ricercaInLista(nodoLista* lista, int data)
{
    if(lista == NULL)
        return 0;
    else if(lista->data == data)
        return 1;
    else
        return ricercaInLista(lista->next,data);  
}

nodoLista* rimuoviElemento(nodoLista* lista, int data)
{
    if(lista == NULL)
        return NULL;
    //Se trova il valore si salva la posizione per poterlo deallocare dopo aver fatto i collegamenti
    else if(lista->data == data)
    {
        nodoLista* tmp = lista;
        lista = lista->next;
        free(tmp);
    }
    else
        lista->next = rimuoviElemento(lista->next,data);

    return lista;    
}

//Inserisce in maniera ricorsiva un nodo in coda alla lista (Quindi nell'ordine corretto)
nodoLista_DL* inserisciInCoda_DL(nodoLista_DL* lista, int data)
{
    nodoLista_DL* tmp = (nodoLista_DL*)malloc(sizeof(nodoLista_DL));
    tmp->data = data;

    if(lista == NULL)
    {
        lista = tmp;
        tmp->next = NULL;
        tmp->prev = NULL;
    }
    else if(lista->next == NULL)
    {
        lista->next = tmp;
        tmp->prev = lista;
        tmp->next = NULL;
    }
    else
        lista->next = inserisciInCoda_DL(lista->next,data);
    
    return lista;
}

/*Rimuove un elemento da una lista Double Linked. è più complicato delle liste semplici dato che dobbiamo accedere anche
ai campi prev e next, quindi distingueremo il caso in cui dobbiamo eliminare il primo nodo (lista->prev = NULL)
ed il caso dell'ultimo nodo (lista->next = NULL) */
nodoLista_DL* rimuoviElemento_DL(nodoLista_DL* lista, int data)
{
    if(lista == NULL)
        return NULL;
    else
    {
        if(lista->data == data)
        {
            //Se trova il valore si salva la posizione in una variabile tmp.
            nodoLista_DL* tmp = lista;
            //Se l'elemento è il primo della lista allora non puo accedere al campo prev, quindi lo imposta a NULL 
            if(lista->prev == NULL)
            {
                lista = lista->next;
                lista->next->prev = NULL;
                free(tmp);
            }
            //Se invece l'elemento è l'ultimo della lista non puo accedere al campo next, quindi lo imposta a NULL
            else if(lista->next == NULL)
            {
                lista->prev->next = NULL;
                lista = lista->next;
                free(tmp);
            }
            //Se l'elemento si trova tra due nodi effettua tutti i collegamenti e lo dealloca.
            else
            {
                lista->prev->next = lista->next;
                lista->next->prev = lista->prev;
                lista = lista->next;
                free(tmp);
            }
 
            return lista;
        }
        //Se invece non trova il valore nel nodo corrente effettua la chiamata ricorsiva.
        else
            lista->next = rimuoviElemento_DL(lista->next,data);
    }
}

void stampaLista_DL(nodoLista_DL* lista)
{
    if(lista == NULL)
        printf("NULL");
    else
    {
        printf("%d -> ",lista->data);
        stampaLista_DL(lista->next);
    }    
}
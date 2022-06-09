#include <stdio.h>
#include <malloc.h>
#include "alberi.h"

//Costruisce un albero a partire da due sottoalberi che possono essere anche vuoti (Quindi sarà semplicemente un nodo)
nodoAlbero* costruisciAlbero(int data, nodoAlbero* FiglioSx, nodoAlbero* figlioDx)
{
    nodoAlbero* tmp = (nodoAlbero*)malloc(sizeof(nodoAlbero));

    //Se l'allocazione va a buon fine collega i due figli
    if(tmp)
    {
        tmp->data = data;
        tmp->sx = FiglioSx;
        tmp->dx = figlioDx;
    }
    return tmp;
}

//Stampa i nodi di un albero utilizzando la visita in PreOrder (Radice, Sinistra, Destra)
void stampa_PreOrder(nodoAlbero* radice)
{
    if(radice)
    {
        printf("%d ",radice->data);
        stampa_PreOrder(radice->sx);
        stampa_PreOrder(radice->dx);
    }
}

//Stampa i nodi di un albero utilizzando la visita InOrder (Sinistra, Radice, Destra), è particolarmente utile con gli ABR
void stampa_InOrder(nodoAlbero* radice)
{
    if(radice)
    {          
        stampa_InOrder(radice->sx);
        printf("%d ",radice->data);
        stampa_InOrder(radice->dx);
    }
}

//Stampa i nodi di un albero utilizzando la visita PostOrder (Sinistra, Destra, Radice)
void stampa_PostOrder(nodoAlbero* radice)
{
    if(radice)
    {          
        stampa_PostOrder(radice->sx);
        stampa_PostOrder(radice->dx);
        printf("%d ",radice->data);
    }
}

//Inserisce un nodo mantenendo le caratteristice di un Albero Binario di Ricerca
nodoAlbero* inserisciInABR(nodoAlbero* radice, int data)
{
    //Se l'albero è vuoto semplicemente crea il primo elemento
    if(radice == NULL)
    {
        nodoAlbero* tmp = (nodoAlbero*)malloc(sizeof(nodoAlbero));

        if(tmp)
        {
            tmp->data = data;
            tmp->sx = NULL;
            tmp->dx = NULL;
            radice = tmp;
        }
        else
            printf("ERRORE: Impossibile allocare la memoria\n");
    }
    //Se l'elemento da inserire è minore dell'elemento corrente dovrà andare a sinistra
    else if(data < radice->data)
        radice->sx = inserisciInABR(radice->sx,data);
    //Se invece l'elemento da inserire è più grande andrà a destra
    else if(data > radice->data)
        radice->dx = inserisciInABR(radice->dx,data);

    return radice;    
}

/*Sfrutta la struttura degli ABR per effettuare la ricerca in tempo Logaritmico: Se l'elemento attuale è piu grande di quello da cercare 
allora procede nel sottoalbero sinistro altrimenti nel destro. Si continua in modo ricorsivo */
int ricercaABR(nodoAlbero* radice, int data)
{
    int trovato = 0;

    //Se l'albero non è vuoto inizia a cercare l'elemento (quando è vuoto ovviamente non è presente)
    if(!(radice == NULL))
    {
        if(radice->data == data)
            return 1;
        //Se l'elemento attuale è maggiore allora ricerco nel sottoalbero sinistro
        else if(radice->data > data)
            trovato = ricercaABR(radice->sx, data); 
        //Se è minore ricerco nel sottoalbero destro
        else if(radice->data < data)
            trovato = ricercaABR(radice->dx, data);
    }
    return trovato;
}

//Ritorna il minimo di un ABR che è semplicemente l'elemento piu "a sinistra" di tutti
int minimoABR(nodoAlbero* radice)
{
    int minimo = 0;

    if(!(radice == NULL))
    {
        //Se l'elemento corrente non ha un figlio sinistro vuol dire che è il minimo
        if(radice->sx == NULL)
            minimo = radice->data;
        else
            minimo = minimoABR(radice->sx);
    }
    return minimo;
}
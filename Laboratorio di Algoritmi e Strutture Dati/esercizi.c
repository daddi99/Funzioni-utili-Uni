#include <stdio.h>
#include <stdlib.h>
#include "alberi.h"
#include "grafi.h"
#include "liste.h"

//Funzione ricorsiva che ad ogni nodo di un ABR sottrae il valore del minimo
nodoAlbero* sottraiMinimoABR(nodoAlbero* albero)
{
    int minimo;

    if(albero)
    {
        minimo = minimoABR(albero);

        sottraiMinimoABR(albero->sx);
        albero->data = (albero->data) - minimo;
        sottraiMinimoABR(albero->dx);
    }
    return albero;
}

//Dato un ABR scrivere una funzione ricorsiva che inserisca in una lista le chiavi dell albero a livello pari
nodoLista* chiaviLivelliPariABR(nodoAlbero* albero, int livello)
{
    nodoLista* lista = NULL;

    if(livello % 2 == 0)
        lista = inserisciInCoda(lista, albero->data);

    lista->next = chiaviLivelliPariABR(albero->sx,livello++);
    lista->next = chiaviLivelliPariABR(albero->dx,livello++); 

    return lista;
}

//Traspone il grafo in input, cioè crea un grafo con archi "complementari" a quello precedente.
void grafoTrasposto(graph* grafo)
{
    /* Vettore che, per ogni vertice, tiene traccia di quali archi sono presenti, ad esempio {1,0,1,0,0} vuol dire che il vertice corrente
    è collegato ai nodi 0 e 2 */
    int* vettorePresenze = (int*)malloc(grafo->numeroVertici * sizeof(int)); 

    //Scorre tutto il vettoreListe del grafo
    for(int i=0; i<grafo->numeroVertici; i++)
    {
        //Riazzera il vettore delle presenze ad ogni ciclo
        for(int i=0; i<grafo->numeroVertici; i++)
            vettorePresenze[i] = 0;

        //Si posiziona all'inizio di ogni lista di adiacenza
        edge* tmpEdge = grafo->vettoreListe[i];

        //Scorre tutta la lista relativa al vertice i nel grafo
        while(tmpEdge != NULL)
        {
            vettorePresenze[tmpEdge->id] = 1; //Imposta ad 1 gli elementi del vettorePresenze per cui esiste un arco
            tmpEdge = tmpEdge->next;
        }

        //Rimuove tutti gli archi precedenti
        deallocaListaAdiacenza(&grafo->vettoreListe[i]);

        //Scorre il vettorePresenze per vedere quali archi deve aggiungere (gli indici degli elementi che contengono 0)
        for(int j=0; j<5; j++)
        {
            //Se l'elemento contiene 0 inserisce in testa nella lista il nuovo edge
            if(vettorePresenze[j] == 0)
            {
                edge* nuovoNodo = (edge*)malloc(sizeof(edge));
                nuovoNodo->id = j;
                nuovoNodo->next =grafo->vettoreListe[i];
                grafo->vettoreListe[i] =  nuovoNodo;
            }
        }
    }
}

void deallocaListaAdiacenza(edge** lista)
{
    edge* tmp;

    while(*lista != NULL)
    {
        tmp = *lista;
        *lista = (*lista)->next;
        free(tmp);
    }
}
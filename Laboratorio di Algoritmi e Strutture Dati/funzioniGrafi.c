#include "grafi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Funzione che crea un grafo privo di collegamenti (le liste di adiacenza sono tutte vuote)
graph* creaGrafoVuoto(int numeroVertici)
{
    graph* grafo = (graph*)malloc(sizeof(graph));

    //Controllo malloc
    if(grafo == NULL)
        printf("Impossibile allocare memoria per il grafo\n");
    else
    {
        //Alloca un vettore di puntatori a edge grande quanto il valore dato in input
        grafo->vettoreListe = (edge**)malloc(numeroVertici * sizeof(edge*));

        //Controllo malloc 
        if(grafo->vettoreListe == NULL)
        {
            printf("Impossibile allocare memoria per la lista di adiacenza del grafo\n");
            free(grafo);
            grafo = NULL;
        }
        else
        {
            grafo->numeroVertici = numeroVertici;

            for(int i=0; i<numeroVertici; i++)
                grafo->vettoreListe[i] = NULL; //Imposta a null tutti i campi del vettore di liste
        }
    } 
    return grafo;
}

void stampaGrafo(graph* grafo)
{
    edge* edgeTmp;
    int numeroArchi = 0;

    if(grafo != NULL) //Se il grafo non è vuoto
    {
        printf("Il grafo ha %d vertici\n",grafo->numeroVertici);

        //Scorre tutti il vettore del grafo
        for(int i=0; i<grafo->numeroVertici; i++)
        {
            printf("Nodi adiacenti al nodo %d -> ",i);
            edgeTmp = grafo->vettoreListe[i];

            //Per ogni vertice nel vettore si scorre la rispettiva lista di adiacenza, stampando quindi tutti i nodi adiacenti ad esso.
            while(edgeTmp != NULL)
            {
                printf("%d ",edgeTmp->id);
                numeroArchi++;
                edgeTmp = edgeTmp->next;
            }
            printf("\n");
        }
        printf("Il grafo ha %d archi\n", numeroArchi);
    }
}

//Funzione che inserisce l'arco (primoVertice, secondoVertice) nel grafo. PrimoVertice e secondoVertice rappresentano gli ID dei vertici, NON IL CONTENUTO
void aggiungiArco(graph* grafo, int primoVertice, int secondoVertice)
{
    edge* newEdge;
    edge* tmpEdge;

    //Controlla che il grafo non sia null, cioè che non esiste il vettore di liste di adiacenza
    if(grafo == NULL)
    {
        printf("ERRORE INSERIMENTO: inizializzare prima il grafo\n");
        return;
    }

    //Controlla che i vertici inseriti abbiano degli ID effettivamente validi (compresi tra 0 e numeroVertici -1)
    if(primoVertice < 0 || primoVertice >= grafo->numeroVertici || secondoVertice < 0 || secondoVertice >= grafo->numeroVertici)
    {
        printf("ERRORE INSERIMENTO: inserire dei vertici validi\n");
        return;
    }

    //Controlla che l'arco non sia gia presente nel grafo
    tmpEdge = grafo->vettoreListe[primoVertice];

    while(tmpEdge != NULL)
    {
        if(tmpEdge->id == secondoVertice)
        {
            printf("ERRORE INSERIMENTO: L'arco e' gia presente nel grafo\n");
            return;
        }
        tmpEdge = tmpEdge->next;
    }

    //Se tutti i controlli vanno a buon fine sarà arrivato a questo punto del codice
    newEdge = (edge*)malloc(sizeof(edge));

    if(newEdge == NULL)
        printf("Impossibile allocare memoria per il nuovo arco");
    else
    {
        //Crea un nuovo nodo contenente il secondo vertice che dovra essere aggiunto alla lista di adiacenza del primo vertice
        newEdge->id = secondoVertice;
        newEdge->next = NULL;

        if(grafo->vettoreListe[primoVertice] == NULL) //Il primo vertice non ha archi (non è collegato a nessuno altro vertice)
            grafo->vettoreListe[primoVertice] =  newEdge; //semplicemente lo aggiunge come primo elemento della lista
        else
        {
            //Si scorre tutta la lista ed aggiunge il nuovo nodo in coda
            tmpEdge = grafo->vettoreListe[primoVertice];

            while(tmpEdge->next != NULL)
                tmpEdge = tmpEdge->next;
            
            tmpEdge->next = newEdge;
        }
    }
}

void rimuoviArco(graph* grafo, int primoVertice, int secondoVertice)
{
   edge* prevEdge; //Arco precedente a quello da rimuovere
   edge* tmpEdge; //Arco da rimuovere
   
    //Controlla che il grafo sia inizializzato (deve esistere il vettore di liste di adiacenza)
    if(grafo == NULL)
    {
        printf("ERRORE RIMOZIONE: Inizializzare prima il grafo\n");
        return;
    }
    
    //Controlla che gli ID dei vertici siano validi
    if(primoVertice < 0 || primoVertice >= grafo->numeroVertici || secondoVertice < 0 || secondoVertice >= grafo->numeroVertici)
    {
        printf("ERRORE RIMOZIONE: inserire dei vertici validi\n");
        return;
    }

    tmpEdge = grafo->vettoreListe[primoVertice];

    if(tmpEdge->id == secondoVertice) //Caso in cui il nodo da rimuovere è il primo elemento della lista
        grafo->vettoreListe[primoVertice] = tmpEdge->next;
    else
    {
        prevEdge = tmpEdge;

        //Scorre tutta lista del primoVertice e se trova secondoVertice lo rimuove (effettuando tutti i collegamenti necessari)
        while(prevEdge->next != NULL)
        {
            if(prevEdge->next->id == secondoVertice)
            {
                tmpEdge = prevEdge->next;
                prevEdge->next = tmpEdge->next;
                free(tmpEdge);
                break;
            }
            prevEdge = prevEdge->next;
        }
    }
}

//Aggiunge un vertice al grafo che si troverò nella posizone [nv + 1] del grafo
void aggiungiVertice(graph* grafo)
{
    edge** nuovoVettoreListe;

    if(grafo == NULL)
        return;
    
    //Rialloca il vettore con un elemento in più
    nuovoVettoreListe = realloc(grafo->vettoreListe, (grafo->numeroVertici + 1) * sizeof(edge*));

    if(nuovoVettoreListe == NULL)
        printf("ERRORE AGGIUNTA VERTICE: Impossibile reallocare memoria per il vettore di liste\n");
    else
    {
        grafo->vettoreListe = nuovoVettoreListe;
        grafo->vettoreListe[grafo->numeroVertici] = NULL; //Imposta a null il nuovo elemento (Non ha archi)
        grafo->numeroVertici++;
    }
}

//FUNZIONI GRAFI PESATI

weightedGraph* creaGrafoPesatoVuoto(int numeroVertici)
{
    weightedGraph* grafo = (weightedGraph*)malloc(sizeof(weightedGraph));

    //Controllo malloc
    if(grafo == NULL)
        printf("Impossibile allocare memoria per il grafo\n");
    else
    {
        //Alloca un vettore di puntatori a edge grande quanto il valore dato in input
        grafo->vettoreListe = (weightedEdge**)malloc(numeroVertici * sizeof(weightedEdge*));

        //Controllo malloc 
        if(grafo->vettoreListe == NULL)
        {
            printf("Impossibile allocare memoria per la lista di adiacenza del grafo\n");
            free(grafo);
            grafo = NULL;
        }
        else
        {
            grafo->numeroVertici = numeroVertici;

            for(int i=0; i<numeroVertici; i++)
                grafo->vettoreListe[i] = NULL; //Imposta a null tutti i campi del vettore di liste
        }
    } 
    return grafo;
}

void stampaGrafoPesato(weightedGraph* grafo)
{
    weightedEdge* edgeTmp;
    int numeroArchi = 0;

    if(grafo != NULL) //Se il grafo non è vuoto
    {
        printf("Il grafo ha %d vertici\n",grafo->numeroVertici);

        //Scorre tutti il vettore del grafo
        for(int i=0; i<grafo->numeroVertici; i++)
        {
            printf("Nodi adiacenti al nodo %d -> ",i);
            edgeTmp = grafo->vettoreListe[i];

            //Per ogni vertice nel vettore si scorre la rispettiva lista di adiacenza, stampando quindi tutti i nodi adiacenti ad esso.
            while(edgeTmp != NULL)
            {
                printf("%d peso: %d, ",edgeTmp->id, edgeTmp->weight);
                numeroArchi++;
                edgeTmp = edgeTmp->next;
            }
            printf("\n");
        }
        printf("Il grafo ha %d archi\n", numeroArchi);
    }
}

void aggiungiArcoPesato(weightedGraph* grafo, int primoVertice, int secondoVertice, int peso)
{
    weightedEdge* newEdge;
    weightedEdge* tmpEdge;

    //Controlla che il grafo non sia null, cioè che non esiste il vettore di liste di adiacenza
    if(grafo == NULL)
    {
        printf("ERRORE INSERIMENTO: inizializzare prima il grafo\n");
        return;
    }

    //Controlla che i vertici inseriti abbiano degli ID effettivamente validi (compresi tra 0 e numeroVertici -1)
    if(primoVertice < 0 || primoVertice >= grafo->numeroVertici || secondoVertice < 0 || secondoVertice >= grafo->numeroVertici)
    {
        printf("ERRORE INSERIMENTO: inserire dei vertici validi\n");
        return;
    }

    //Controlla che l'arco non sia gia presente nel grafo
    tmpEdge = grafo->vettoreListe[primoVertice];

    while(tmpEdge != NULL)
    {
        if(tmpEdge->id == secondoVertice)
        {
            printf("ERRORE INSERIMENTO: L'arco e' gia presente nel grafo\n");
            return;
        }
        tmpEdge = tmpEdge->next;
    }

    //Se tutti i controlli vanno a buon fine sarà arrivato a questo punto del codice
    newEdge = (weightedEdge*)malloc(sizeof(weightedEdge));

    if(newEdge == NULL)
        printf("Impossibile allocare memoria per il nuovo arco");
    else
    {
        //Crea un nuovo nodo contenente il secondo vertice che dovra essere aggiunto alla lista di adiacenza del primo vertice
        newEdge->id = secondoVertice;
        newEdge->weight = peso;
        newEdge->next = NULL;

        if(grafo->vettoreListe[primoVertice] == NULL) //Il primo vertice non ha archi (non è collegato a nessuno altro vertice)
            grafo->vettoreListe[primoVertice] =  newEdge; //semplicemente lo aggiunge come primo elemento della lista
        else
        {
            //Si scorre tutta la lista ed aggiunge il nuovo nodo in coda
            tmpEdge = grafo->vettoreListe[primoVertice];

            while(tmpEdge->next != NULL)
                tmpEdge = tmpEdge->next;
            
            tmpEdge->next = newEdge;
        }
    }
}

void rimuoviArcoPesato(weightedGraph* grafo, int primoVertice, int secondoVertice)
{
   weightedEdge* prevEdge; //Arco precedente a quello da rimuovere
   weightedEdge* tmpEdge; //Arco da rimuovere
   
    //Controlla che il grafo sia inizializzato (deve esistere il vettore di liste di adiacenza)
    if(grafo == NULL)
    {
        printf("ERRORE RIMOZIONE: Inizializzare prima il grafo\n");
        return;
    }
    
    //Controlla che gli ID dei vertici siano validi
    if(primoVertice < 0 || primoVertice >= grafo->numeroVertici || secondoVertice < 0 || secondoVertice >= grafo->numeroVertici)
    {
        printf("ERRORE RIMOZIONE: inserire dei vertici validi\n");
        return;
    }

    tmpEdge = grafo->vettoreListe[primoVertice];

    if(tmpEdge->id == secondoVertice) //Caso in cui il nodo da rimuovere è il primo elemento della lista
        grafo->vettoreListe[primoVertice] = tmpEdge->next;
    else
    {
        prevEdge = tmpEdge;

        //Scorre tutta lista del primoVertice e se trova secondoVertice lo rimuove (effettuando tutti i collegamenti necessari)
        while(prevEdge->next != NULL)
        {
            if(prevEdge->next->id == secondoVertice)
            {
                tmpEdge = prevEdge->next;
                prevEdge->next = tmpEdge->next;
                free(tmpEdge);
                break;
            }
            prevEdge = prevEdge->next;
        }
    }
}

void aggiungiVerticeGrafoPesato(weightedGraph* grafo)
{
    weightedEdge** nuovoVettoreListe;

    if(grafo == NULL)
        return;
    
    //Rialloca il vettore con un elemento in più
    nuovoVettoreListe = realloc(grafo->vettoreListe, (grafo->numeroVertici + 1) * sizeof(weightedEdge*));

    if(nuovoVettoreListe == NULL)
        printf("ERRORE AGGIUNTA VERTICE: Impossibile reallocare memoria per il vettore di liste\n");
    else
    {
        grafo->vettoreListe = nuovoVettoreListe;
        grafo->vettoreListe[grafo->numeroVertici] = NULL; //Imposta a null il nuovo elemento (Non ha archi)
        grafo->numeroVertici++;
    }
}
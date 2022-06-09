#ifndef GRAFI_H
#define GRAFI_H

typedef struct edge
{
    int id;
    struct edge* next;
}edge;

typedef struct graph
{
    int numeroVertici;
    struct edge** vettoreListe; //Vettore di liste che contengono i vertici adiacenti
}graph;

typedef struct weightedEdge
{
    int id;
    int weight;
    struct weightedEdge* next;
}weightedEdge;

typedef struct weightedGraph
{
    int numeroVertici;
    struct weightedEdge** vettoreListe;
}weightedGraph;

//FUNZIONI GRAFI SEMPLICI
graph* creaGrafoVuoto(int numeroVertici);
void stampaGrafo(graph* grafo);
void aggiungiArco(graph* grafo, int primoVertice, int secondoVertice);
void rimuoviArco(graph* grafo, int primoVertice, int secondoVertice);
void aggiungiVertice(graph* grafo);

//FUNZIONI GRAFI PESATI
weightedGraph* creaGrafoPesatoVuoto(int numeroVertici);
void stampaGrafoPesato(weightedGraph* grafo);
void aggiungiArcoPesato(weightedGraph* grafo, int primoVertice, int secondoVertice, int peso);
void rimuoviArcoPesato(weightedGraph* grafo, int primoVertice, int secondoVertice);
void aggiungiVerticeGrafoPesato(weightedGraph* grafo);

//FUNZIONI ESERCIZI
void grafoTrasposto(graph* grafo);
void deallocaListaAdiacenza(edge** lista);


#endif

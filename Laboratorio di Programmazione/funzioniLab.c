#include <stdio.h>
#include <stdlib.h>

struct Dato{
    char stringa[20];
    int intero;
};

struct NodoLista{
    struct Dato data;
    struct NodoLista* next;
};

//PROTOTIPI FUNZIONI
void stampaLista(struct NodoLista* lista);
struct NodoLista* inserisciInCoda(struct NodoLista* lista, struct Dato nuovoElemento);
void inserisciInCoda2(struct NodoLista** lista, struct Dato nuovoElemento);
void popolaListaDaFile(struct NodoLista** lista, char nomeFile[20]);
void filtraLista(struct NodoLista** lista, int valoreMinimo);
int lunghezzaLista(struct NodoLista* lista);
void scriviOrdinatoSuFile(struct NodoLista* lista, char nomeFile[20]);
void deallocaLista(struct NodoLista* lista);

//------------------------------------------------------------

//MAIN
int main()
{
    struct NodoLista* lista = NULL;

    popolaListaDaFile(&lista,"fileInput.txt");

    //filtraLista(&lista, 20);

    stampaLista(lista);

    scriviOrdinatoSuFile(lista,"fileOutput.txt");

    deallocaLista(lista);
}


//------------------------------------------------------------
//FUNZIONI UTILI
void stampaLista(struct NodoLista* lista)
{
    if(lista == NULL)
        printf("NULL\n");
    else
    {
        printf("{%s %d} -> ",lista->data.stringa, lista->data.intero);
        stampaLista(lista->next);
    }
}

//Ritorna la lista modificata
struct NodoLista* inserisciInCoda(struct NodoLista* lista, struct Dato nuovoElemento)
{
    if(lista == NULL)
    {
        //Alloca memoria nell'heap per un nuovo nodo che andrò ad inserire nella lista
        struct NodoLista* nuovoNodo = (struct NodoLista*)malloc(sizeof(struct NodoLista));

        //Copia all'interno di questo nuovo nodo i valori presi in input da nuovoElemento
        nuovoNodo->data = nuovoElemento;
        nuovoNodo->next = NULL;
        
        return nuovoNodo; //La nuova testa diventa il nodo appena creato
    }
    else
        //"Collega" la lista con il valore che verrà ritornato dalla chiamata ricorsiva su lista->next
        lista->next = inserisciInCoda(lista->next, nuovoElemento); 

    return lista;
}

void popolaListaDaFile(struct NodoLista** lista, char nomeFile[20])
{
    FILE* fileInput = fopen(nomeFile,"r"); //Apre il file in modalità lettura
    struct Dato datoTmp; //Struct di appoggio dove verranno messi i valori della fscanf

    //Controllo errori di apertura file
    if(fileInput == NULL)
    {
        printf("ERRORE: Impossibile aprire il file di input\n");
        return;
    }
    else
    {
        //Fin quando la fscanf è riuscita a leggere una riga completa
        while(fscanf(fileInput,"%s %d",datoTmp.stringa, &(datoTmp.intero)) == 2)
            inserisciInCoda2(lista,datoTmp);
    }

    fclose(fileInput);
}

//Elimina dalla lista tutti i nodi che hanno valore minore di valoreMinimo
void filtraLista(struct NodoLista** lista, int valoreMinimo)
{
    struct NodoLista* nodoCorrente = *lista; //Lo utilizzo per scorrere la lista in modo da non modificare il puntatore alla testa
    struct NodoLista* predecessore = NULL; //Serve per mantenere i collegamenti nella lista dopo aver eliminato il nodo
    struct NodoLista* nodoDaEliminare;


    while(nodoCorrente != NULL)
    {
        if(nodoCorrente->data.intero < valoreMinimo)
        {
            nodoDaEliminare = nodoCorrente;

            if(predecessore == NULL) //Vuol dire che nodoCorrente è il primo nodo della lista
                *lista = nodoCorrente->next;
            else
                predecessore->next = nodoCorrente->next; //Collega il predecessore al nodo successivo a quello da eliminare

            nodoCorrente = nodoCorrente->next;
            free(nodoDaEliminare);
        }
        else //Se il nodo non deve essere eliminato semplicemente continua a scorrere
        {
            predecessore = nodoCorrente;
            nodoCorrente = nodoCorrente->next;
        }
    }
}

void scriviOrdinatoSuFile(struct NodoLista* lista, char nomeFile[20])
{
    FILE* fileOutput = fopen(nomeFile,"w"); //Apre il file in modalità scrittura
    int* vettoreFlags = calloc(lunghezzaLista(lista),sizeof(int)); //Crea un vettore della stessa dimensione della lista per tenere traccia dei massimi precedenti (1 = già utilizzato, 0 = mai utilizzato)
    struct NodoLista* cursore = lista; //Serve per scorrere la lista
    struct Dato nodoMassimo; //Mantiene tutti i dati del nodo con valore massimo
    int valoreMassimo = 0; //Contiene il valore massimo
    int indice; //Tiene traccia dell'indice del nodo massimo, in modo da poter poi aggiornare le flags 

    //Controllo errori
    if(fileOutput == NULL)
    {
        printf("ERRORE: Impossibile aprire il file di Output\n");
        return;
    }
    else
    {
        for(int i=0; i < lunghezzaLista(lista); i++)
        {
            for(int j=0; j< lunghezzaLista(lista); j++)
            {
                //Fa la classica ricerca del massimo e in più controlla che quell'elemento non sia stato già un massimo in precedenza
                if(cursore->data.intero > valoreMassimo && vettoreFlags[j] == 0)
                {
                    valoreMassimo = cursore->data.intero; //Aggiorna il valore massimo
                    nodoMassimo = cursore->data; //Aggiorna la struct che contiene i dati del nodo massimo
                    indice = j; //Si salva l'indice del nodo massimo
                }
                
                cursore = cursore->next;
            }

            //A questo punto nodoMassimo conterrà effettivamente l'elemento desiderato e quindi lo scrive sul file
            fprintf(fileOutput,"%s %d\n",nodoMassimo.stringa, nodoMassimo.intero); 

            valoreMassimo = 0;  //Riazzera il massimo per la prossima iterazione
            vettoreFlags[indice] = 1; //Segna l'elemento come già utlizzato

            cursore = lista; //Reimposta il cursore all inizio della lista per la nuova iterazione
        }
    }

    fclose(fileOutput);
    free(vettoreFlags);
}

//Sfrutta i doppi puntatori in modo da andare a modificare direttamente la struttura della lista senza doverla ritornare.
void inserisciInCoda2(struct NodoLista** lista, struct Dato nuovoElemento)
{
    if(*lista == NULL)
    {
        //Alloca memoria nell'heap per un nuovo nodo che andrò ad inserire nella lista
        struct NodoLista* nuovoNodo = (struct NodoLista*)malloc(sizeof(struct NodoLista));

        //Copia all'interno di questo nuovo nodo i valori presi in input da nuovoElemento
        nuovoNodo->data = nuovoElemento;
        nuovoNodo->next = NULL;
        
        *lista = nuovoNodo; //La nuova testa diventa il nodo appena creato
        
        return; 
    }
    else
        //Richiama la funzione ricorsivamente sul resto della lista
        inserisciInCoda2(&(*lista)->next, nuovoElemento); 
}

int lunghezzaLista(struct NodoLista* lista)
{
    int lunghezza = 0;

    while(lista != NULL)
    {
        lunghezza++;
        lista = lista->next;
    }

    return lunghezza;
}

void deallocaLista(struct NodoLista* lista)
{
    struct NodoLista* tmp;

    while(lista != NULL)
    {
        tmp = lista;
        lista = lista->next;
        free(tmp);
    }
}
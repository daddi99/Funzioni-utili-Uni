#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Dato{
    char stringa[20];
    int intero;
};

struct NodoLista{
    struct Dato data;
    struct NodoLista* next;
};

//Si potrebbe anche fare solo con il front ma per comodità tengo traccia anche del rear
struct Coda
{
    struct NodoLista* front;
    struct NodoLista* rear;
};

//Per modularità creo una struct apposta anche se contiene semplicemente un puntatore a NodoLista
struct Pila
{
    struct NodoLista* head;
};

//LISTE
void stampaLista(struct NodoLista* lista);
struct NodoLista* inserisciInCoda(struct NodoLista* lista, struct Dato nuovoElemento);
void inserisciInCoda2(struct NodoLista** lista, struct Dato nuovoElemento);
void popolaListaDaFile(struct NodoLista** lista, char nomeFile[20]);
void filtraLista(struct NodoLista** lista, int valoreMinimo);
struct NodoLista* filtraLista_noEdit(struct NodoLista* lista);
int lunghezzaLista(struct NodoLista* lista);
void scriviOrdinatoSuFile(struct NodoLista* lista, char nomeFile[20]);
void deallocaLista(struct NodoLista* lista);

//MATRICI DINAMICHE
int** popolaMatriceDaFile(char nomeFile[20]);
void stampaMatrice(float** matrice, int numeroRighe, int numeroColonne);

//CODE
struct Coda* creaCoda();
void enqueue(struct Coda* coda, struct Dato nuovoElemento);
void dequeue(struct Coda* coda);

//PILE
void inizializzaStack(struct Pila* pila);
void push(struct Pila* pila, struct Dato nuovoDato);
struct Dato pop(struct Pila* pila);

//ALTRE FUNZIONI
int generaNumeroCasuale(int valoreMassimo);

//------------------------------------------------------------

//MAIN
int main()
{
    struct Dato davide = {"Davide",22};
    struct Dato mario = {"Mario",56};
    struct Dato rit;

    struct Pila* pila;
    inizializzaStack(pila);

    push(pila,davide);
    push(pila,mario);
    rit = pop(pila);

    printf("%s %d\n", rit.stringa, rit.intero);

    stampaLista(pila->head);

    printf("%d\n",generaNumeroCasuale(10));
}


//------------------------------------------------------------
//FUNZIONI LISTE
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

//Filtra la lista ma senza modificarla, infatti ritorna una nuova lista contenente solo i nodi che ci interessano
struct NodoLista* filtraLista_noEdit(struct NodoLista* lista)
{
    struct NodoLista* listaFiltrata = NULL;

    while(lista != NULL)
    {
        //CONDIZIONE DA VERIFICARE PER ESSERE INSERITO NELLA LISTA FILTRATA
        if(lista->data.intero < 20)
        {
            inserisciInCoda2(&listaFiltrata, lista->data);
        }
        lista = lista->next;
    }

    return listaFiltrata;
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

//FUNZIONI MATRICI

//Popola la matrice dal file tenendo conto che la prima riga contiene il numero di righe e colonne
int** popolaMatriceDaFile(char nomeFile[20])
{
    FILE* fileInput = fopen(nomeFile,"r");
    int numeroRighe, numeroColonne;
    int** matriceOutput = NULL;
    int valoreTmp;

    //Controllo errori di apertura file
    if(fileInput == NULL)
    {
        printf("ERRORE: Impossibile leggere il file di input\n");
        return NULL;
    }
    else
    {
        //Estrae dalla prima riga del file il numero di righe e colonne
        fscanf(fileInput,"%d %d",&numeroRighe,&numeroColonne);

        //Alloca lo spazio per numeroRighe puntatori a intero
        matriceOutput = calloc(numeroRighe, sizeof(int*));

        //Ognuno di questi puntatori punterà ad un vettore di numeroColonne interi
        for(int i=0; i< numeroRighe;i++)
            matriceOutput[i] = calloc(numeroColonne,sizeof(int));

        //Continua a scorrere il file e mette tutti gli elementi nella matrice
        for(int i=0; i<numeroRighe; i++)
            for(int j=0;j<numeroColonne;j++)
                fscanf(fileInput,"%d",&matriceOutput[i][j]);
                
    }

    fclose(fileInput);

    return matriceOutput;
}

//Stampa la matrice in una versione un pò più bella esteticamente
void stampaMatrice(float** matrice, int numeroRighe, int numeroColonne)
{
    printf("\n");
    for(int i=0; i<numeroRighe; i++)
    {
        printf("%d  ",i); //Stampa l'indice di riga
        for(int j=0; j<numeroColonne; j++)
        {
            //Se il valore ha una sola cifra prima della virgola stampa uno spazio in più per avere il disegno allineato
            if(matrice[i][j] < 10) 
                printf("| %.2f  |",matrice[i][j]);
            else
                printf("| %.2f |",matrice[i][j]);
        }
        printf("\n");
    }        
}

//FUNZIONI CODE

struct Coda* creaCoda()
{
    //Alloco la nuova coda e setto sia il front che il rear a NULL
    struct Coda* nuovaCoda = (struct Coda*)malloc(sizeof(struct Coda));
    nuovaCoda->front = NULL;
    nuovaCoda->rear = NULL;
    
    return nuovaCoda;
}

//Inserisce il nuovo elemento i coda
void enqueue(struct Coda* coda, struct Dato nuovoElemento)
{
    //Alloco il nuovo nodo ed inizializzo i campi
    struct NodoLista* nuovoNodo = (struct NodoLista*)malloc(sizeof(struct NodoLista));
    nuovoNodo->data = nuovoElemento;
    nuovoNodo->next = NULL;

    //Caso coda vuota
    if(coda->rear == NULL)
    {
        coda->front = nuovoNodo;
        coda->rear = nuovoNodo;
        return;
    }
    
    coda->rear->next = nuovoNodo;
    coda->rear = nuovoNodo; // nuovoNodo diventa la nuova rear
}

void dequeue(struct Coda* coda)
{
    struct NodoLista* tmp;

    //Caso coda vuota
    if(coda->front == NULL)
        return;
    
    //Si salva il front precedente per poterlo deallocare dopo
    tmp = coda->front;

    coda->front = coda->front->next; //Aggiorna il front

    //Se il front diventa NULL allora vuol dire che la coda aveva un solo elemento, quindi impostiamo anche il rear a NULL
    if(coda->front == NULL)
        coda->rear = NULL;

    free(tmp);
}

//FUNZIONI PILE

void inizializzaStack(struct Pila* pila)
{
    pila->head = NULL;
}

void push(struct Pila* pila, struct Dato nuovoDato)
{
    struct NodoLista* nuovoNodo = (struct NodoLista*)malloc(sizeof(struct NodoLista));
    nuovoNodo->data = nuovoDato;
    nuovoNodo->next = NULL;

    if(pila->head == NULL)
        pila->head = nuovoNodo;
    else
    {
        nuovoNodo->next = pila->head;
        pila->head = nuovoNodo;
    }
}

struct Dato pop(struct Pila* pila)
{
    if(pila->head != NULL)
    {
        struct Dato valore = pila->head->data;
        struct NodoLista* daEliminare = pila->head;

        pila->head = pila->head->next;  
        free(daEliminare);

        return(valore);
    }
    else
        printf("ERRORE POP: La coda e' vuota\n");
}

//ALTRE FUNZIONI

/* La funzione rand() normalmente genera dei numeri pseudo casuali, cioè sempre nella stessa sequenza.
Questa funzione sfrutta srand() per cambiare il seme da cui viene generata la sequenza, lo imposta al numero di secondi trascorsi dal 
1970 in modo da avere sempre un seed diverso */
int generaNumeroCasuale(int valoreMassimo)
{
    srand(time(NULL)); //Imposta il seed al numero di secondi passati dal 1970, quindi sarà un valore che cambia ogni secondo

    int risultato = rand() % (valoreMassimo + 1); //Utlizzo l'operatore modulo per ottenere un numero nell'intervallo desiderato

    return risultato;
}
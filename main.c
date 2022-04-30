#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "Map.h"

typedef struct 
{
    char * nombre;
    char * marca;
    char * tipo;
    int stock;
    int precio;
}Producto;

//funciones 
//void buscarArtista(List*, char*);
//void buscarGenero(List*, char*);
//void mostrarListas(List*);
//void buscarNombre(List *, char *);
//void mostrarTodasCan(List*);
//void mostrarUnaList(List* , char *);
void exportarArchivo(char *, Map * );

int is_equal_string(void * key1, void * key2) 
{
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

/* funcion para leer los datos ingreados por el usuario
    para realizar la funcion eliminar y agregar
Producto * leerCancion(int funcion)
{
    
    //proceso de la funcion 
    cancion*c=(Producto *) malloc (sizeof(cancion));

    char aux[100];
    printf("Ingrese el nombre de la cancion\n");
    getchar();
    fgets(aux, 100, stdin);
    aux[strcspn(aux, "\n" )] = '\0';
    c->nombCancion = strdup(aux);
    

    printf("Ingrese el artista de la cancion\n");
    
    fgets(aux, 100, stdin);
    aux[strcspn( aux, "\n" )] = '\0';
    c->artista = strdup(aux);

    printf("Ingrese el año de la cancion\n");
    
    fgets(aux, 100, stdin);
    aux[strcspn( aux, "\n" )] = '\0';
    c->anyo = strdup(aux);

    //condicion para solo ser utilizada en la funcion agregar funcion
    if(funcion == 3)
    {
        printf("Ingrese el/los generos de la cancion(si son mas de 1 separelos por comas)\n");
        
        fgets(aux, 100, stdin);
        aux[strcspn( aux, "\n" )] = '\0';
        c->genero = strdup(aux);

        printf("Ingrese la lista a la que pertenece esta cancion\n");
        
        fgets(aux, 100, stdin);
        aux[strcspn(aux, "\n" )] = '\0';
        c->lista= strdup(aux);
    }
    return c;
}*/
//funcion para ir separando por las "," todas las lineas del archivo
char * leeProductosArchi(FILE *entrada, Producto *prod, char *token, char *aux)
{
    char * dump;
    token = strtok(aux, ",");
    prod->nombre = strdup(token);
    token = strtok(NULL, ",");
    prod->marca = strdup(token);
    token = strtok(NULL, ",");
    prod->tipo = strdup(token);
    token = strtok(NULL, ",");
    prod->stock = (int) strtol(token, &dump, 10);
    token = strtok(NULL, "\n");
    prod->precio = (int) strtol(token, &dump, 10);

    return token;

}
//funcion que inserta canciones a su lista correspondiente 
/*void insertarNuevaCanci(Producto * canci, List * lista1, List * lista2, List * lista3)
{
    //proceso de la funcion
    if (strcmp(canci->lista, "Lista 1") == 0)
    {
        pushBack(lista1,canci);
    }
    else if (strcmp(canci->lista, "Lista 2") == 0)
    {
        pushBack(lista2,canci);
    }
    else if (strcmp(canci->lista, "Lista 3") == 0)
    {
        pushBack(lista3,canci);
    }
}*/
//funcion para poder leer el archvo crear las listas de canciones y ingresarla a una lista mayor
Map * procesarArchivo(FILE *entrada)
{
    char aux[101];
    char *token;
    Map * listaProd = createMap(is_equal_string);

    while(fgets(aux, 100, entrada) != NULL)//condicion para ir leyendo cancion 1 por 1 e insertarlas a su lista correspondiente 
    {
        Producto *prod = (Producto *) malloc (sizeof(Producto));
        token = leeProductosArchi(entrada, prod, token, aux);
        insertMap(listaProd, prod->nombre, prod);
    }
    Producto * p = firstMap(listaProd);
    printf("%s\n", p->nombre);
    //insertar al final de la lista de canciones
    return listaProd;
}
//funcion para indicar que funcion quiere utilizar el ususario
void eleccionFunciones(int *funcion)
{
    printf("seleccione 0 para salir\n");
    printf("Seleccione 1 para importar un archivo\n");//funcion esencial para la funci0nalidad del codigo
    printf("Seleccione 2 para exportar a un archivo\n");
    printf("Seleccione 3 si quiere agregar una cancion a una lista\n");
    printf("Seleccione 4 si quiere buscar una cancion por su nombre\n");
    printf("Seleccione 5 si quiere buscar una cancion por su artista\n");
    printf("Seleccione 6 si quiere buscar una cancion por su genero\n");
    printf("Seleccione 7 si quiere eliminar una cancion\n");
    printf("Seleccione 8 si quiere ver los nombre de las listas de reproduccion y su cantidad de canciones\n");
    printf("Seleccione 9 si quiere ver una lista de reproduciones y sus canciones\n");
    printf("Seleccione 10 si quiere ver todas las canciones de todas las listas\n");
    scanf("%i",funcion);//funcion que quiere ingresar
    

}
//funcion para agregar canciones de una lista ya existente o si no existe crear esa lista y agregarla 
/*void agregarCancion(List *listaProd, Producto *c)
{
    short aux = 1;
    List *listaAux;
    Producto * prodAux;
    int checks=0;

    listaAux = firstList(listaProd);
    prodAux = firstList(listaAux); 
    
    while (listaAux != NULL && aux != 0)//ciclo repetitivo para encontrar la lista a la que se tiene que agregar la nueva cancion 
    {
        if (strcmp(c->lista, prodAux->lista) == 0)
        {
            aux = 0;
            break;
        }
        listaAux = nextList(listaProd);
        if(listaAux == NULL) break;
        prodAux = firstList(listaAux);
    }

    if(aux == 1)//si aux==1 la lista que se tiene que agregar no existe 
    {
        List * lista4 = createList();
        pushBack(lista4,c);
        pushBack(listaProd,lista4);
        printf("Se ha creado una nueva lista con la cancion ingresada\n");
    }
    else
    {
        prodAux = firstList(listaAux);
        while(prodAux != NULL)
        {
            //(202)if para comparar si la cancion existe ya dentor de la lista o no
            if(strcmp(c->nombCancion, prodAux->nombCancion)==0 && strcmp(c->artista, prodAux->artista)==0 && strcmp(c->anyo, prodAux->anyo)==0 )
            {
                printf("La cancion que desea ingresar ya se en cuentra en la lista\n");
                checks++;
            }
            prodAux = nextList(listaAux);
        }
        if(checks==0){
            pushBack(listaAux,c);
            printf("La cancion a sido ingresada con exito\n");
        }
       
    }

}*/
//funcion para eliminar una cancion de todas las listas existentes
/*void eliminarCancion(List *Lista, Producto *c)
{
    List * listaAct = firstList(Lista);
    Producto * posLista = firstList(listaAct);
    int contListas = 0;
    int check = 0;
    while (listaAct != NULL)
    {
        listaAct = nextList(Lista);
        contListas++;
    }
    listaAct = firstList(Lista);
    while (listaAct != NULL)//
    {
        
        if (strcmp(c->nombCancion, posLista->nombCancion) == 0 && strcmp(c->artista, posLista->artista)==0)
        {
            printf("La cancion ha sido eliminada de %s\n", posLista->lista);
            popCurrent(listaAct);
            
            posLista = nextList(listaAct);
            check++;
            
            if (posLista == NULL)
            {
                listaAct = nextList(Lista);
                if (listaAct == NULL) break;
                posLista = firstList(listaAct);
            }
        }
        else
        {
            posLista = nextList(listaAct);
            if (posLista == NULL)
            {
                listaAct = nextList(Lista);
                if (listaAct == NULL) break;
                posLista = firstList(listaAct);
            }
        }
        //listaAct = nextList(Lista);
    }
    if (check == 0) 
    {
        printf("No se encontro la canciones especificada\n");
    }
    else
    {
        printf("La cancion a sido eliminada con exito de todas las listas\n");
    }
}*/

int main()
{
    int funcion;
    Producto *p;
    
    char Nlista[10];
    char busq[100];
    
    FILE *entrada;

    Map* listaProd;

    eleccionFunciones(&funcion);//funcion que da el numero de la funcion que se quiere ingresar
    
    
    while (funcion != 0)//ciclo repetitivo que permite seleccionar la funcion que quiere utilizar y con un 0 salir de estas 
    {
        //switch que permite seleccionar la funcion segun el numero ingresado
        switch (funcion)
        {
            case 1:
            printf("Ingrese nombre del archivo a importar: ");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn( busq, "\n" )] = '\0';
            entrada = fopen(busq, "rt");
            listaProd = procesarArchivo(entrada);
            printf("Canciones Procesadas\n");
            break;

            case 2:
            printf("Ingrese nombre del archivo al cual exportar:");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn(busq, "\n" )] = '\0';
            exportarArchivo(busq, listaProd);
            break;
            
            /*case 3:
            p=leerCancion(funcion);
            agregarCancion(listaProd, p);
            break;

            case 4:
            printf("Ingrese nombre de la cancion a buscar: ");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn( busq, "\n" )] = '\0';
            buscarNombre(listaProd, busq);
            break;

            case 5:
            printf("Ingrese nombre del artista a buscar: ");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn( busq, "\n" )] = '\0';
            buscarArtista(listaProd, busq);
            break;

            case 6:
            printf("Ingrese nombre del genero a buscar: ");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn( busq, "\n" )] = '\0';
            buscarGenero(listaProd, busq);
            break;
            

            case 7:
            p=leerCancion(funcion);
            eliminarCancion(listaProd, p);
            break;

            case 8:
            mostrarListas(listaProd);
            break;

            case 9:
            printf("ingrese el nombre de la lista(Lista 1,Lista 2,Lista 3)\n");
            getchar();
            fgets(Nlista,10,stdin);
            if ((strlen(Nlista) > 0) && (Nlista[strlen (Nlista) - 1] == '\n'))
            Nlista[strlen (Nlista) - 1] = '\0';
            mostrarUnaList(listaProd, Nlista);
            break;

            case 10:
            mostrarTodasCan(listaProd);
            break;*/

            default:
            printf("Numero ingresado no valido. Ingrese un numero valido\n");
            break;
            return 0;
        }
        eleccionFunciones(&funcion);
        if(funcion==0)
        {
            printf("A finalizado el proceso");
        }
    }
}
//funcion que busca una cancion segun el artista mostrando todas las canciones de esta 
/*void buscarArtista(List *Lista, char *nombre)
{
    List * listaAct = firstList(Lista);
    Producto * posLista = firstList(listaAct);
    int contListas = 0;
    int check = 0;
    while (listaAct != NULL)
    {
        listaAct = nextList(Lista);
        contListas++;
    }
    listaAct = firstList(Lista);
    while (listaAct != NULL)
    {
        if (strcmp(nombre, posLista->artista) == 0)
        {
            printf("Nombre: %s\n", posLista->nombCancion);
            printf("Artista: %s\n", posLista->artista);
            printf("Genero: %s\n", posLista->genero);
            printf("Año: %s\n", posLista->anyo);
            printf("Lista: %s\n", posLista->lista);
            posLista = nextList(listaAct);
            check++;
            if (posLista == NULL)
            {
                listaAct = nextList(Lista);
                if (listaAct == NULL) break;
                posLista = firstList(listaAct);
            }
        }
        else
        {
            posLista = nextList(listaAct);
            if (posLista == NULL)
            {
                listaAct = nextList(Lista);
                if (listaAct == NULL) break;
                posLista = firstList(listaAct);
            }
        }
    }
    if (check == 0) printf("No se encontraron canciones del artista %s.\n",nombre);
}*/
//funcion que busca una cancion segun el genero que esta tenga 
/*void buscarGenero(List *Lista, char *genero)
{
    List * listaAct = firstList(Lista);
    Producto * posLista = firstList(listaAct);
    int contListas = 0;
    int check = 0;
    while (listaAct != NULL)
    {
        listaAct = nextList(Lista);
        contListas++;
    }
    listaAct = firstList(Lista);
    while (listaAct != NULL)
    {
        if (strcmp(genero, posLista->genero) == 0)
        {
            printf("Nombre: %s\n", posLista->nombCancion);
            printf("Artista: %s\n", posLista->artista);
            printf("Genero: %s\n", posLista->genero);
            printf("Año: %s\n", posLista->anyo);
            printf("Lista: %s\n", posLista->lista);
            posLista = nextList(listaAct);
            check++;
            if (posLista == NULL)
            {
                listaAct = nextList(Lista);
                if (listaAct == NULL) break;
                posLista = firstList(listaAct);
            }
        }
        else
        {
            posLista = nextList(listaAct);
            if (posLista == NULL)
            {
                listaAct = nextList(Lista);
                if (listaAct == NULL) break;
                posLista = firstList(listaAct);
            }
        }
    }
    if (check == 0) printf("No se encontraron canciones del genero %s.\n",genero);
}*/
//funcion que genera un archivo y exporta todas la listas de canciones a el
void exportarArchivo(char *nombre, Map * lista)
{
    FILE *fpt;
    fpt = fopen(nombre, "w+"); // Si no existe el archivo, fopen lo crea con el nombre y extension especificados
    Producto * listaAct = firstMap(lista);
    while (listaAct != NULL) // Se itera por toda la lista
    {
        fprintf(fpt, "%s,", listaAct->nombre); // Copia todo lo que haya en la lista al archivo nuevo
        fprintf(fpt, "%s,", listaAct->marca);
        fprintf(fpt, "%s,", listaAct->tipo);
        fprintf(fpt, "%d,", listaAct->stock);
        fprintf(fpt, "%d\n", listaAct->precio);
        listaAct = nextMap(lista); // Siguiente elemento de la lista
    }    
    fclose(fpt);
}
//funcion que busca canciones segun el nombre que tengan 
/*void buscarNombre(List *Lista, char *nombre)
{
    List * listaAct = firstList(Lista);
    Producto * posLista = firstList(listaAct);
    int contListas = 0;
    int check = 0;
    while (listaAct != NULL)
    {
        listaAct = nextList(Lista);
        contListas++;
    }
    listaAct = firstList(Lista);
    for (int i = 0; i < contListas; i++)
    {
        if (check != 0) break;
        while (posLista != NULL)
        {
            if (strcmp(nombre, posLista->nombCancion) == 0)
            {
                printf("Nombre: %s\n", posLista->nombCancion);
                printf("Artista: %s\n", posLista->artista);
                printf("Genero: %s\n", posLista->genero);
                printf("Año: %s\n", posLista->anyo);
                printf("Lista: %s\n", posLista->lista);
                check++;
                break;
            }
            else
            {
                posLista = nextList(listaAct);
                if (posLista == NULL)
                {
                    listaAct = nextList(Lista);
                    if (listaAct == NULL)
                    {
                        printf("No se encontro la cancion especificada.\n");
                        break;
                    }
                    posLista = firstList(listaAct);
                }
            }
        }
    }
}*/
//muestra la cantidad de canciones que tiene esta lista 
/*void mostrarListas(List *Lista)
{
    List * listaAct = firstList(Lista);
    Producto * posLista = firstList(listaAct);
    int contCanciones = 0;
    int contListas = 0;
    while (listaAct != NULL)
    {
        listaAct = nextList(Lista);
        contListas++;
    }
    listaAct = firstList(Lista);
    for (int i = 0; i < contListas; i++)
    {
        contCanciones = 0;
        char *nombLista = strdup(posLista->lista);
        while (posLista != NULL)
        {
            posLista = nextList(listaAct);
            contCanciones++;
        }
        printf("Nombre de Lista : %s, Cantidad de Canciones : %d\n", nombLista, contCanciones);
        listaAct = nextList(Lista);
        if (listaAct == NULL) break;
        posLista = firstList(listaAct);
    }
}*/
//funcion que muestra todas la canciones de la lista de canciones sin importar el nombre de la lista 
void mostrarTodasCan(List *Lista)
{
    
    List * list_ = firstList(Lista);
    cancion* aux = firstList(list_);
    printf("lista 1\n");
    //recorre la lista mostrando las canciones que hay en ellas
    while(aux!=NULL){
        
        printf("%s,", aux->nombCancion);
        printf("%s,", aux->artista);
        printf("%s,", aux->genero);
        printf("%s\n", aux->anyo);
        aux=nextList(list_);
        

    }
    
    
    list_ = nextList(Lista);
    aux = firstList(list_);
    printf("\n");
    printf("lista 2\n");
    while(aux != NULL)
    {
        printf("%s,", aux->nombCancion);
        printf("%s,", aux->artista);
        printf("%s,", aux->genero);
        printf("%s\n", aux->anyo);
        aux=nextList(list_);
    }
    
    list_ = nextList(Lista);
    aux = firstList(list_);
    printf("\n");
    printf("lista 3\n");
    while(aux!= NULL)
    {
        printf("%s,", aux->nombCancion);
        printf("%s,", aux->artista);
        printf("%s,", aux->genero);
        printf("%s\n", aux->anyo);
        aux=nextList(list_);
 
    }
    printf("\n");
}
//muestra una lista segun el nombre de la lista que ingreso el usuario 
/*void mostrarUnaList(List* lista, char* Nlista)
{
    List * hola = firstList(lista);
    cancion* aux = firstList(hola);
    int cont=0;
    
    
    while(aux!=NULL){
        if(strcmp(Nlista,aux->lista)==0)//busca que los nombres de las listas coincidan
        {
            if(cont==0)
            {
                printf("Lista 1\n");
                printf("\n");
            }
            printf("%s,", aux->nombCancion);
            printf("%s,", aux->artista);
            printf("%s,", aux->genero);
            printf("%s\n", aux->anyo);
            cont++;
            
        }
        aux=nextList(hola);
        

    }
    
    
    hola = nextList(lista);
    aux = firstList(hola);
    cont=0;
    while(aux != NULL)
    {
        if(strcmp(aux->lista,Nlista)==0)
        {
            if(cont==0){
                printf("Lista 2\n");
                printf("\n"); 
            }
            printf("%s,", aux->nombCancion);
            printf("%s,", aux->artista);
            printf("%s,", aux->genero);
            printf("%s\n", aux->anyo);
            cont++;
        }
        aux=nextList(hola);
    }
    
    hola = nextList(lista);
    aux = firstList(hola);
    cont=0;
    
    while(aux!= NULL)
    {
        if(strcmp(aux->lista,Nlista)==0)
        {
            if(cont==0)
            {
                printf("Lista 3\n");
                printf("\n"); 

            }    
            printf("%s,", aux->nombCancion);
            printf("%s,", aux->artista);
            printf("%s,", aux->genero);
            printf("%s\n", aux->anyo);
            cont++;
        }
        aux=nextList(hola);
 
    }
    printf("\n"); 
}*/


//sdfs
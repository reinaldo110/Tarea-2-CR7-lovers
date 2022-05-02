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

typedef struct
{
    char *nombrePro;
    int cantidad;
    int precio; 
}Pro_Carrito;

typedef struct{
    char *NombreCarr;
    List *Productos;
    int precio;
}Carrito;

//funciones 
void exportarArchivo(char *, Map * );
void buscarTipo(Map*, char*);
void buscarMarca(Map*, char*);
void buscarNombre(Map *, char *);
void mostrarTodosProductos(Map*);

int is_equal_string(void * key1, void * key2) 
{
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

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
//funcion para procesar el archivo
Map * procesarArchivo(FILE *entrada)
{
    char aux[101];
    char * token;
    Map * listaProd = createMap(is_equal_string);

    while(fgets(aux, 100, entrada) != NULL)//condicion para ir leyendo productos uno en uno para agregar a la lista
    {
        Producto * prod = (Producto *) malloc (sizeof(Producto));
        token = leeProductosArchi(entrada, prod, token, aux);
        insertMap(listaProd, prod->nombre, prod);
    }
    return listaProd;
}
//funcion para preguntarle al usuario que funcion desea ejecutar
void eleccionFunciones(int *funcion)
{
    printf("seleccione 0 para salir\n");
    printf("Seleccione 1 para importar un archivo\n");//funcion esencial para la funcionalidad del codigo, listo
    printf("Seleccione 2 para exportar a un archivo\n");//listo
    printf("Seleccione 3 si quiere agregar un producto a la lista\n");
    printf("Seleccione 4 si quiere buscar un producto por su tipo\n");//listo
    printf("Seleccione 5 si quiere buscar por marca del/los productos\n");//listo
    printf("Seleccione 6 si quiere buscar un producto por su nombre\n");//listo
    printf("Seleccione 7 si quiere mostrar todos los productos\n");//listo
    printf("Seleccione 8 si quiere agregar un producto a su carrito\n");
    printf("Seleccione 9 si quiere eliminar un producto de su carrito\n");
    printf("Seleccione 10 si quiere concretar su compra\n");
    printf("Seleccione 11 si quiere mostrar los carritos de compra\n");
    scanf("%i",funcion);//funcion que quiere ingresar
}
//funcion que inserta el producto en el carrito
void insertarEnCarrito(List *carro, Pro_Carrito *producto){
    Pro_Carrito *aux = firstList(carro);
    printf("a");
    if(aux == producto){
        aux->cantidad += producto->cantidad;
        return;
    }
    while(aux != producto)
    {
        aux = (Pro_Carrito*)nextList(carro); //Error IS JIR
        printf("b");
        if(aux == producto){
            aux->cantidad += producto->cantidad;
            break;
        }
        if(aux == NULL)
        {
           break;
        }
        printf("c");
    }
    if(aux == NULL)
    {
        pushBack(carro, producto);
    }
}
//funcion para agregar un elemento al carrito
void AgregarAlCarrito(Map* carro, char busq[], Map* lista){
    
    char nombrePro[100];
    int cantidad;   
                          
    Pro_Carrito* producto = (Pro_Carrito*) malloc (sizeof(Pro_Carrito));
    Carrito *ptr;

    printf("ingrese el nombre del producto: \n");
    fgets(nombrePro, 100, stdin);
    nombrePro[strcspn( nombrePro, "\n" )] = '\0';
    printf("ingrese la cantidad de unidades: \n");
    scanf("%d", &cantidad);

    //Producto *auxPro=searchMap(lista, nombrePro);

    producto->cantidad = cantidad;
    producto->nombrePro = strdup(nombrePro);
    //producto->precio=auxPro->precio;
    //printf("%d\n", producto->precio);
    
    //Archivo_100productos.csv
    //Salchichas de pavo 1 Kg
    ptr = (Carrito*) searchMap(carro, busq);
    
    if(ptr != NULL)
    { 
        List *carrActual = ptr->Productos;
        insertarEnCarrito(carrActual, producto);
        printf("su producto ha sido ingresado correctamente");
    }
    else
    {
        List *ListPro = createList();
        Carrito *aux = malloc(sizeof(Carrito));
        pushFront(ListPro, producto);
        aux->Productos = ListPro;
        aux->NombreCarr = strdup(busq);
        insertMap(carro, aux->NombreCarr, aux);
        aux = firstMap(carro);
        printf("%s\n", aux->NombreCarr);
        Pro_Carrito *aux2 = firstList(aux->Productos);
        printf("%s, %d\n", aux2->nombrePro, aux2->cantidad);
        printf("su producto ha sido ingresado correctamente\n");
    }

 }
//funcion para eliminar un elemento del carrito
void eliminarUlt(carro, busq){
    List *ptr;
    Carrito *aux = searchMap(carro, busq);
    ptr = aux->Productos;
    popBack(ptr);
    printf("El producto se ha eliminado con exito\n");
}
//main
int main()
{
    int funcion;
    Producto *p;
    char Nlista[10];
    char busq[100];
    FILE *entrada;
    Map* listaProd;
    Map* carro;
    eleccionFunciones(&funcion);
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
            printf("Productos procesados.\n");
            break;

            case 2:
            printf("Ingrese nombre del archivo al cual exportar:");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn(busq, "\n" )] = '\0';
            exportarArchivo(busq, listaProd);
            break;
            
            case 3:
            //agregarProducto()
            break;

            case 4:
            printf("Ingrese el tipo de producto: ");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn( busq, "\n" )] = '\0';
            buscarTipo(listaProd, busq);
            break;

            case 5:
            printf("Ingrese nombre de la marca: ");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn( busq, "\n" )] = '\0';
            buscarMarca(listaProd, busq);
            break;

            case 6:
            printf("Ingrese nombre del producto: ");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn( busq, "\n" )] = '\0';
            buscarNombre(listaProd, busq);
            break;

            case 7:
            mostrarTodosProductos(listaProd);
            break;

            case 8:
            printf("ingrese el nombre del Carrito de compra: ");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn( busq, "\n" )] = '\0';
            AgregarAlCarrito(carro, busq, listaProd);
            break;

            case 9:
            printf("Ingrese el nombre del carro: \n");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn( busq, "\n" )] = '\0';
            eliminarUlt(carro, busq);
            break;

            case 10:
            //concretarCompra();
            break;
            
            case 11:
            //mostarCarritos();
            break;

            default:
            printf("Numero ingresado no valido. Ingrese un numero valido\n");
            break;
            return 0;
        }
        eleccionFunciones(&funcion);
        if(funcion == 0)
        {
            printf("Ha finalizado el proceso.");
        }
    }
}
//funcion que genera un archivo y exporta a este los productos presentes en la lista
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
//funcion que busca un producto segun la categoria a la que pertenece y lo imprime en pantalla
void buscarTipo(Map *Mapa, char *tipo)
{
    int a = 0;
    Producto * p = firstMap(Mapa);
    while(p != NULL)
    {
        if(stricmp(p->tipo,tipo)==0)
        {
            printf("Nombre: %s\n", p->nombre);
            printf("Marca: %s\n", p->marca);
            printf("Tipo: %s\n", p->tipo);
            printf("Stock: %i\n", p->stock); 
            printf("Precio: $%i\n\n", p->precio);
            a++;
        }
        p = nextMap(Mapa);
    }
    if(a == 0) printf("El tipo de producto buscado no existe\n");
}
//funcion que busca un producto segun la marca de este y lo imprime en pantalla
void buscarMarca(Map *Mapa, char *marca)
{
    int a = 0;
    Producto * p = firstMap(Mapa);
    while(p != NULL)
    {
        if(stricmp(p->marca,marca)==0)
        {
            printf("Nombre: %s\n", p->nombre);
            printf("Marca: %s\n", p->marca);
            printf("Tipo: %s\n", p->tipo);
            printf("Stock: %i\n", p->stock); 
            printf("Precio: $%i\n\n", p->precio);
            a++;
        }
        p = nextMap(Mapa);
    }
    if(a == 0) printf("La marca ingresada no existe\n");
}
//funcion que busca productos segun el nombre que tengan 
void buscarNombre(Map *Mapa, char *nombre)
{
    int a = 0;
    Producto * p = firstMap(Mapa);
    while(p!=NULL)
    {
        if(stricmp(p->nombre,nombre)==0)
        {
            printf("Nombre: %s\n", p->nombre);
            printf("Marca: %s\n", p->marca);
            printf("Tipo: %s\n", p->tipo);
            printf("Stock: %i\n", p->stock); 
            printf("Precio: $%i\n", p->precio);
            a++;
        }
        p = nextMap(Mapa); 
    }
    if(a == 0) printf("El producto solicitado no existe\n");
}
//funcion que muestra todos los productos presentes en la lista
void mostrarTodosProductos(Map *Mapa)
{
    Producto * p = firstMap(Mapa);
    while(p != NULL)
    {
        printf("Nombre: %s\n", p->nombre);
        printf("Marca: %s\n", p->marca);
        printf("Tipo: %s\n", p->tipo);
        printf("Stock: %i\n", p->stock); 
        printf("Precio: $%i\n\n", p->precio); 
        p = nextMap(Mapa);
    }
}
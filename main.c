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
    int cantidad;
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
    printf("Seleccione 2 para exportar a un archivo\n");
    printf("Seleccione 3 si quiere agregar un producto a la lista\n");
    printf("Seleccione 4 si quiere buscar un producto por su tipo\n");
    printf("Seleccione 5 si quiere buscar por marca del/los productos\n");
    printf("Seleccione 6 si quiere buscar un producto por su nombre\n");
    printf("Seleccione 7 si quiere mostrar todos los productos\n");
    printf("Seleccione 8 si quiere agregar un producto a su carrito\n");
    printf("Seleccione 9 si quiere eliminar un producto de su carrito\n");
    printf("Seleccione 10 si quiere concretar su compra\n");
    printf("Seleccione 11 si quiere mostrar los carritos de compra\n");
    scanf("%i",funcion);//funcion que quiere ingresar
}
//funcion para agregar productos a un carrito ya existente
void insertarEnCarrito(List *carro, Pro_Carrito *producto,Carrito*ptr,Producto* auxpro){
    Pro_Carrito *aux = firstList(carro);

    if(aux == producto){
        aux->cantidad += producto->cantidad;
        while(auxpro->stock<aux->cantidad)
        {
            aux->cantidad -= producto->cantidad;
            printf("no hay stock suficiente para la cantidad que desea agregar al carrito \n");
            printf("el stock disponible es %i,ingrese una cantidad dentro del stock\n",auxpro->stock);
            scanf("%d", &producto->cantidad);

        }
        aux->cantidad += producto->cantidad;
        return;
    }
    while (aux != producto)//while para confirma si el producto existe o no dentro del carrito
    {
        aux = (Pro_Carrito*) nextList(carro);
        if(aux == producto)//producto ya dentro del carrito
        {
            aux->cantidad += producto->cantidad;
            while(auxpro->stock<aux->cantidad)
            {
                aux->cantidad -= producto->cantidad;
                printf("no hay stock suficiente para la cantidad que desea agregar al carrito \n");
                printf("el stock disponible es %i,ingrese una cantidad dentro del stock\n",auxpro->stock);
                scanf("%d", &producto->cantidad);

            }
            aux->cantidad += producto->cantidad;
            break;
        }
        if(aux == NULL)//si llega al final de la lista finaliza el while
        {
           break;
        }
    }
    if(aux == NULL)//if no existe el producto en el carrito y lo agrega
    {
        pushBack(carro, producto);
        ptr->cantidad+=producto->cantidad;
        
    }
}
//funcion mayor para agregar productos al carrito

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

    Producto *auxPro=searchMap(lista, nombrePro);
    while(auxPro->stock<cantidad)
    {
        printf("no hay stock suficiente para la cantidad que desea agregar al carrito \n");
        printf("el stock disponible es %i,ingrese una cantidad dentro del stock\n",auxPro->stock);
        scanf("%d", &cantidad);

    }
    producto->cantidad = cantidad;
    producto->nombrePro = strdup(nombrePro);
    producto->precio=auxPro->precio;

    ptr = (Carrito*) searchMap(carro, busq);
    //condiciones para saber si al carrito que se desia ngresar existe o no
    if(ptr != NULL)//si se cumple existe el carrito
    { 
        List *carrActual = ptr->Productos;
        insertarEnCarrito(carrActual, producto,ptr,auxPro);
        printf("su producto a sido ingresado correctamente");
    }
    else//no existe el carrito
    {

        List *ListPro = createList();
        Carrito *aux = malloc(sizeof(Carrito));
        pushFront(ListPro, producto);
        aux->Productos = ListPro;
        aux->NombreCarr = strdup(busq);
        insertMap(carro, aux->NombreCarr, aux);
        aux=firstMap(carro);
        Pro_Carrito *aux2 = firstList(aux->Productos);
        aux->cantidad=cantidad;
        printf("su producto a sido ingresado correctamente\n");

    }
 }
//funcion que elimina el ultimo producto ingresado al carrito

void eliminarUlt(Map* carro, char busq[]){
    List *ptr;
    
    Carrito *aux = searchMap(carro, busq);
    Pro_Carrito *aux2=lastList(aux->Productos);
    aux->cantidad-=aux2->cantidad;
    ptr = aux->Productos;
    popBack(ptr);
    printf("EL producto se a eliminado con exito\n");
}
//funcion que agrega un nuevo producto al mapa de productos 
void agregarProducto(Map *Mapa)
{
    char * dump;
    Producto *nuevoProducto = (Producto *) malloc (sizeof(Producto));
    char nombre[100], marca[100], tipo[100];
    int stock = 0, precio = 0, variable = 0;
    Producto * p = firstMap(Mapa);

    printf("ingrese nombre del producto: ");
    getchar();
    fgets(nombre, 100, stdin);
    nombre[strcspn( nombre, "\n" )] = '\0';
    nuevoProducto->nombre = strdup(nombre);

    printf("ingrese marca del producto: ");
    getchar();
    fgets(marca, 100, stdin);
    marca[strcspn( marca, "\n" )] = '\0';
    nuevoProducto->marca = strdup(marca);

    printf("ingrese tipo del producto: ");
    getchar();
    fgets(tipo, 100, stdin);
    tipo[strcspn( tipo, "\n" )] = '\0';
    nuevoProducto->tipo = strdup(tipo);

    printf("ingrese stock del producto: ");
    scanf("%d", &stock);
    nuevoProducto->stock = stock;
    printf("ingrese precio del producto: ");
    scanf("%d", &precio);
    nuevoProducto->precio = precio;

    while(p != NULL)//comprobar si el producto ingresado ya existe o no
    {
        if(stricmp(p->nombre,nombre) == 0)
        {
            p->stock+=stock;
            variable++;
        }
        p = nextMap(Mapa);
    }
    if(variable==0)//si no existe se inserta en el mapa
    {
        insertMap(Mapa,nuevoProducto->nombre,nuevoProducto);
    }
}

//funcion que concreta la compra del carrito eliminando el carrito y restando el stock
void concretarCompra(Map*carro,char busq[],Map* listprod)
{   
    Carrito *aux = searchMap(carro,busq);
    Pro_Carrito *ptr = firstList(aux->Productos);
    Producto* proaux;
    while(ptr!=NULL)//while para mostrar los productos que contiene el carrito
    {
        printf("%s\n",ptr->nombrePro);
        
        proaux=searchMap(listprod,ptr->nombrePro);
        proaux->stock-=ptr->cantidad;
        aux->precio=aux->precio+(ptr->precio*ptr->cantidad);
        ptr = nextList(aux->Productos);

    }
    printf("total a pagar %i\n",aux->precio);
    eraseMap(carro,busq);




}

//funcion que muestra todos los carritos mostrando al cantidad total de 
//productos del carro y el nombre de dicho producto utilizando dos punteros
//para recorrer el mapa y luego la lista dentro del mapa
void mostarCarritos(Map* carro)
{
    Carrito *aux = firstMap(carro);
    while(aux!=NULL)//while que muestra carro por carro
    {
        printf("Nombre carrito: %s\n",aux->NombreCarr);
        printf("Cantidad productos: %i\n",aux->cantidad);
        Pro_Carrito *ptr = firstList(aux->Productos);
       

        if(ptr==NULL){
            printf("El carrito esta vacio\n");
            break;
        }
        while(ptr!=NULL)//while que muestra los productos que tiene el carro
        {
            
            printf("Nombre: %s\n", ptr->nombrePro);
            ptr=nextList(aux->Productos);
        }
        aux=nextMap(carro);
    }
}

//funcion main 
int main()
{
    int funcion;
    Producto *p;
    char Nlista[10];
    char busq[100];
    FILE *entrada;
    Map* listaProd;
    Map* carro=createMap(is_equal_string);
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
            agregarProducto(listaProd);
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
            printf("Ingrese el nombre del carro: \n");
            getchar();
            fgets(busq, 100, stdin);
            busq[strcspn( busq, "\n" )] = '\0';
            concretarCompra(carro, busq,listaProd);
            break;
            
            case 11:
            mostarCarritos(carro);
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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define TIPO_PANCHO 0
#define TIPO_HAMBURGUESA 1

typedef struct {
    bool chedar;
    bool provoleta;
    bool bacon;
    bool tomate;
    bool lechuga;
    bool quesoDambo;
} ingredientes;

typedef struct {
    bool mostaza;
    bool barbacoa;
    bool cesar;
    bool ketchup;
    bool mayonesa;
    bool salsGolf;
    bool aderezoEspecial;
} condimentos;

typedef struct Nodo {
    int tipo;
    char variante[60];
    char descripcion[100];
    float precio;
    ingredientes ingredientes;
    condimentos condimentos;
    int opcion;
    struct Nodo *siguiente;
} Nodo;


typedef struct {
    int tipo;
    char variante[60];
    char descripcion[100];
    float precio;
    ingredientes ingredientes;
    condimentos condimentos;
    int opcion;
} RegistroProducto;

typedef struct ItemPedido {
    struct Nodo *producto;
    int cantidad;
    float subtotal;
    struct ItemPedido *siguiente;
} ItemPedido;


typedef struct Pedido {
    int id_pedido;
    char cliente[40];
    ItemPedido *items;
    float total;
    bool metodo_pago;
    struct Pedido *siguiente;
} Pedido;




Nodo *crearNodo(int tipo, char *variante, char *descripcion, float precio, ingredientes ing, condimentos cond, int opcion);
void insertar(Nodo **cabeza, int tipo, char *variante, char *descripcion, float precio, ingredientes ing, condimentos cond, int opcion);
void cargarCatalogoInicial(Nodo **cabeza, FILE **catalogo, char *nombreArchivo);
void recorrerArchivo_filtrado(FILE **pf, char *nombreArchivo, int tipoFiltrar);
Nodo* buscarPorOpcion(Nodo *cabeza, int opcion);
void agregarItemAlPedido(Pedido *pedido, Nodo *prod, int cantidad);
void encolarPedido(Pedido **primero, Pedido **ultimo, Pedido *nuevo);
Pedido* armarPedido(Nodo *catalogo, FILE **pf, char *nombreArchivo);
void entregarPedido(Pedido **primero, Pedido **ultimo);
void mostrarColaPedidos(Pedido *primero);
void menu();


int main(void)
{
    srand(time(NULL));
    FILE *archivoCatalogo = NULL;
    char *nombreArchivoCatalogo = "catalogo.bin";
    Nodo *catalogo = NULL;


    cargarCatalogoInicial(&catalogo, &archivoCatalogo, nombreArchivoCatalogo);

    printf("tipo de HAMBURGUESAS \n");
    recorrerArchivo_filtrado(&archivoCatalogo, nombreArchivoCatalogo, TIPO_HAMBURGUESA);

    printf("tipo de PANCHOS \n");
    recorrerArchivo_filtrado(&archivoCatalogo, nombreArchivoCatalogo, TIPO_PANCHO);

    Pedido *p = armarPedido(catalogo, &archivoCatalogo, nombreArchivoCatalogo);
    int entregar=0;
    printf("queres entregar un pedido? \n");
    scanf("%d",&entregar);
    if(entregar==1) {
        entregarPedido(&p, &catalogo);
        mostrarColaPedidos(p);
    }



    return 0;
}




Nodo *crearNodo(int tipo, char *variante, char *descripcion, float precio, ingredientes ing, condimentos cond, int opcion)
{
    Nodo *n = (Nodo*)malloc(sizeof(Nodo));
    if (!n) {
        printf("no se pudo asignar memoria\n");
        return NULL;
    }

    n->tipo = tipo;

    strncpy(n->variante, variante, sizeof(n->variante) - 1);
    strncpy(n->descripcion, descripcion, sizeof(n->descripcion) - 1);
    n->precio = precio;
    n->ingredientes = ing;
    n->condimentos  = cond;
    n->opcion = opcion;
    n->siguiente = NULL;
    return n;
}

void insertar(Nodo **cabeza, int tipo, char *variante, char *descripcion, float precio, ingredientes ing, condimentos cond, int opcion)
{
    Nodo *nuevo = crearNodo(tipo, variante, descripcion, precio, ing, cond, opcion);
    if (!nuevo){
      printf("no se pudo crear el nodo  \n");
        return;
    }

    if (*cabeza == NULL) {
        *cabeza = nuevo;
        return;
    }
    Nodo *aux = *cabeza;
    while (aux->siguiente) {
        aux = aux->siguiente;
    }
    aux->siguiente = nuevo;

}


void cargarCatalogoInicial(Nodo **cabeza, FILE **catalogo, char *nombreArchivo)
{
    *catalogo = fopen(nombreArchivo, "wb");
    if (!*catalogo) {
        printf("no se pudo abrir el archivo  \n");
        return;
    }
    RegistroProducto r;
    ingredientes ing_1 = (ingredientes){ true, false, true, false, false, false };
    condimentos  cond_1 = (condimentos){ false, false, false, true, false, false, true };
    insertar(cabeza, TIPO_HAMBURGUESA, "Hamburguesa Juli crack",
             "Doble carne, doble cheddar, bacon y aderezo especial",
             15000, ing_1, cond_1, 1);
    r.tipo = TIPO_HAMBURGUESA;
    strcpy(r.variante, "Hamburguesa Juli crack");
    strcpy(r.descripcion, "Doble carne, doble cheddar, bacon y aderezo especial");
    r.precio = 15000; r.ingredientes = ing_1; r.condimentos = cond_1; r.opcion = 1;
    fwrite(&r, sizeof(RegistroProducto), 1, *catalogo);

    ingredientes ing_2 = (ingredientes){ false, true, false, true, true, false };
    condimentos  cond_2 = (condimentos){ false, false, false, false, true, false, false };
    insertar(cabeza, TIPO_HAMBURGUESA, "Hamburguesa King Gaston",
             "Doble carne, provoleta, tomate, lechuga y mayonesa",
             19000, ing_2, cond_2, 2);
    r.tipo = TIPO_HAMBURGUESA;
    strcpy(r.variante, "Hamburguesa King Gaston");
    strcpy(r.descripcion, "Doble carne, provoleta, tomate, lechuga y mayonesa");
    r.precio = 19000; r.ingredientes = ing_2; r.condimentos = cond_2; r.opcion = 2;
    fwrite(&r, sizeof(RegistroProducto), 1, *catalogo);

    ingredientes ing_3 = (ingredientes){ true, false, true, true, true, false };
    condimentos  cond_3 = (condimentos){ false, true, false, false, false, false, false };
    insertar(cabeza, TIPO_HAMBURGUESA, "Hamburguesa Junior Engineer",
             "Triple carne, triple cheddar, bacon, tomate, lechuga y barbacoa",
             18500, ing_3, cond_3, 3);
    r.tipo = TIPO_HAMBURGUESA;
    strcpy(r.variante, "Hamburguesa Junior Engineer");
    strcpy(r.descripcion, "Triple carne, triple cheddar, bacon, tomate, lechuga y barbacoa");
    r.precio = 18500; r.ingredientes = ing_3; r.condimentos = cond_3; r.opcion = 3;
    fwrite(&r, sizeof(RegistroProducto), 1, *catalogo);

    ingredientes ing_p1 = (ingredientes){ true, false, true, false, false, false };
    condimentos  cond_p1 = (condimentos){ false, false, false, false, false, false, false };
    insertar(cabeza, TIPO_PANCHO, "Panchito hoy se salio",
             "Cheddar y lluvia de bacon", 6000, ing_p1, cond_p1, 4);
    r.tipo = TIPO_PANCHO;
    strcpy(r.variante, "Panchito hoy se salio");
    strcpy(r.descripcion, "Cheddar y lluvia de bacon");
    r.precio = 6000; r.ingredientes = ing_p1; r.condimentos = cond_p1; r.opcion = 4;
    fwrite(&r, sizeof(RegistroProducto), 1, *catalogo);

    ingredientes ing_p2 = (ingredientes){ false, true, true, false, false, false };
    condimentos  cond_p2 = (condimentos){ false, false, false, false, false, false, true };
    insertar(cabeza, TIPO_PANCHO, "Panchito subte E",
             "Provoleta, aderezo especial y bacon", 7500, ing_p2, cond_p2, 5);
    r.tipo = TIPO_PANCHO;
    strcpy(r.variante, "Panchito subte E");
    strcpy(r.descripcion, "Provoleta, aderezo especial y bacon");
    r.precio = 7500; r.ingredientes = ing_p2; r.condimentos = cond_p2; r.opcion = 5;
    fwrite(&r, sizeof(RegistroProducto), 1, *catalogo);

    ingredientes ing_p3 = (ingredientes){ true, false, false, false, false, false };
    condimentos  cond_p3 = (condimentos){ false, false, false, false, false, false, false };
    insertar(cabeza, TIPO_PANCHO, "Panchito soy Ucemer",
             "Cheddar + frase motivadora", 8200, ing_p3, cond_p3, 6);
    r.tipo = TIPO_PANCHO;
    strcpy(r.variante, "Panchito soy Ucemer");
    strcpy(r.descripcion, "Cheddar + frase motivadora");
    r.precio = 8200; r.ingredientes = ing_p3; r.condimentos = cond_p3; r.opcion = 6;
    fwrite(&r, sizeof(RegistroProducto), 1, *catalogo);

    fclose(*catalogo);

}


void recorrerArchivo_filtrado(FILE **catalogo, char *nombreArchivo, int tipoFiltrar)
{
    *catalogo = fopen(nombreArchivo, "rb");
    if (!*catalogo) {
        printf("no se pudo abrir el archivo \n");
        return;
    }

    RegistroProducto r;
    int hay = 0;
    while (fread(&r, sizeof(RegistroProducto), 1, *catalogo) == 1) {
        if (tipoFiltrar == -1 || r.tipo == tipoFiltrar) {
            hay = 1;
            if (r.tipo == TIPO_HAMBURGUESA)
                printf("--- HAMBURGUESA OPCION %d ---\n", r.opcion);
            else
                printf("--- PANCHO OPCION %d ---\n", r.opcion);
            printf("%s\n%s\n$%.2f\n\n", r.variante, r.descripcion, r.precio);
        }
    }

    if (!hay) {
        if (tipoFiltrar == TIPO_HAMBURGUESA) {
            printf("lpn no se guardo bien\n");
        }
        else if (tipoFiltrar == TIPO_PANCHO) {
            printf("lpn no se guardo bien\n");
        }
        else {
            printf("bua no hay ni producos\n");
        }
    }

    fclose(*catalogo);

}

Nodo* buscarPorOpcion(Nodo *cabeza, int opcion)
{
    while (cabeza) {
        if (cabeza->opcion == opcion) {
            return cabeza;
        }
        cabeza = cabeza->siguiente;
    }
    return NULL;
}

void agregarItemAlPedido(Pedido *pedido, Nodo *prod, int cantidad)
{
    if (!pedido || !prod || cantidad <= 0) {
        return;
    }

    ItemPedido *nuevo = (ItemPedido*)malloc(sizeof(ItemPedido));
    if (!nuevo) {
        printf("no se pudo asignar memoria para item\n");
        return;
    }

    nuevo->producto = prod;
    nuevo->cantidad = cantidad;
    nuevo->subtotal = prod->precio * cantidad;
    nuevo->siguiente = NULL;

    if (!pedido->items) {
        pedido->items = nuevo;
    } else {
        ItemPedido *aux = pedido->items;
        while (aux->siguiente) aux = aux->siguiente;
        aux->siguiente = nuevo;
    }

    pedido->total += nuevo->subtotal;
}

void encolarPedido(Pedido **primero, Pedido **ultimo, Pedido *nuevo)
{
    if (!nuevo) return;
    nuevo->siguiente = NULL;

    if (!*primero) {
        *primero = *ultimo = nuevo;
    } else {
        (*ultimo)->siguiente = nuevo;
        *ultimo = nuevo;
    }
}


Pedido* armarPedido(Nodo *catalogo, FILE **pf, char *nombreArchivo)
{
    if (!catalogo) {
        printf("no hay catalogo en memoria\n");
        return NULL;
    }


    Pedido *p = (Pedido*)malloc(sizeof(Pedido));
    if (!p) {
        printf("no se pudo asignar memoria para pedido\n");
        return NULL;
    }

    p->id_pedido = rand()%1000;
    p->items = NULL;
    p->total = 0.0f;
    p->metodo_pago = false;
    p->siguiente = NULL;

    printf("ingrese su nombre: ");
    fflush(stdin);
    fgets(p->cliente, sizeof(p->cliente), stdin);
    printf("HOLA %s BIENVENIDO A -MC KITTEN-! TU ID DE PEDIDO ES #%d\n", p->cliente, p->id_pedido);

    int seguir = 1;
    while (seguir) {
        int tipo;
        printf("que queres pedir? (0 = PANCHOS, 1 = HAMBURGUESAS): ");
        scanf("%d", &tipo);

        if (tipo == TIPO_PANCHO) {
            printf("=== CATALOGO PANCHOS ===\n");
            recorrerArchivo_filtrado(pf, nombreArchivo, TIPO_PANCHO);
        } else if (tipo == TIPO_HAMBURGUESA) {
            printf("=== CATALOGO HAMBURGUESAS ===\n");
            recorrerArchivo_filtrado(pf, nombreArchivo, TIPO_HAMBURGUESA);
        } else {
            printf("opcion invalida\n");
            continue;
        }

        int opcionElegida = 0, cantidad = 0;
        printf("ingrese el numero de OPCION del producto: ");
        scanf("%d", &opcionElegida);
        printf("cantidad: ");
        scanf("%d", &cantidad);

        Nodo *prod = buscarPorOpcion(catalogo, opcionElegida);
        if (!prod) {
            printf("no existe la opcion %d\n", opcionElegida);
        } else if (prod->tipo != tipo) {
            printf("la opcion %d no es del tipo elegido\n", opcionElegida);
        } else {
            agregarItemAlPedido(p, prod, cantidad);
            printf("agregado: %s x%d ($%.2f c/u) -> subtotal: $%.2f  total: $%.2f\n",
                   prod->variante, cantidad, prod->precio, prod->precio*cantidad, p->total);
        }

        printf("\nagregar otro producto? (1 = si, 0 = no): ");
        scanf("%d", &seguir);
    }

    printf("===== RESUMEN PEDIDO #%d =====\n", p->id_pedido);
    printf("Cliente: %s\n", p->cliente);
    ItemPedido *aux = p->items;
    if (!aux) printf("(sin items)\n");
    while (aux) {
        printf(" - %s x%d  ($%.2f c/u) -> $%.2f\n",
               aux->producto->variante, aux->cantidad, aux->producto->precio, aux->subtotal);
        aux = aux->siguiente;
    }
    printf("TOTAL: $%.2f\n", p->total);

    return p;
}
void entregarPedido(Pedido **primero, Pedido **ultimo)
{
    if (*primero == NULL) {
        printf("no tenemos pedidos \n");
        return;
    }

    Pedido *aEntregar = *primero;
    *primero = (*primero)->siguiente;

    if (*primero == NULL) {
        *ultimo = NULL;
    }

    printf("ENTREGANDO PEDIDO #%d\n", aEntregar->id_pedido);
    printf("Cliente: %s\n", aEntregar->cliente);
    printf("Total: $%.2f\n", aEntregar->total);


    ItemPedido *aux = aEntregar->items;
    while (aux) {
        printf(" - %s x%d  ($%.2f c/u)\n",
               aux->producto->variante, aux->cantidad, aux->producto->precio);
        aux = aux->siguiente;
    }

    printf("Pedido entregado con exito\n");

    free(aEntregar);
}

void mostrarColaPedidos(Pedido *primero)
{
    if (primero == NULL) {
        printf("no hay pedidos en la cola\n");
        return;
    }

    printf("PEDIDOS EN ESPERA:\n");
    int i = 1;

    while (primero != NULL) {
        printf("Pedido %d:\n", i);
        printf("ID pedido: #%d\n", primero->id_pedido);
        printf("Cliente: %s\n", primero->cliente);
        printf("Total a pagar: $%.2f\n", primero->total);

        printf("Productos:\n");
        ItemPedido *item = primero->items;
        while (item != NULL) {
            printf("  - %s x%d  $%.2f c/u \n",
                   item->producto->variante,
                   item->cantidad,
                   item->producto->precio);
            item = item->siguiente;
        }

        primero = primero->siguiente;
        i++;
    }

    printf("\n");
}

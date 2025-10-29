#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define TIPO_PANCHO 0
#define TIPO_HAMBURGUESA 1

typedef struct{
    bool chedar;
    bool provoleta;
    bool bacon;
    bool tomate;
    bool lechuga;
    bool quesoDambo;
}ingredientes;


typedef struct{
    bool mostaza;
    bool barbacoa;
    bool cesar;
    bool ketchup;
    bool mayonesa;
    bool salsGolf;
    bool aderezoEspecial;
}condimentos;

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
Nodo *crearNodo(int tipo, char *variante, char *descripcion, float precio, ingredientes ingredientes, condimentos condimentos, int opcion);
void insertar(Nodo **cabeza, int tipo, char *variante, char *descripcion, float precio, ingredientes ingredientes, condimentos condimentos, int opcion);
void cargarCatalogoInicial(Nodo **cabeza);

int main(void)
{
    srand(time(NULL));
    Nodo *catalogo = NULL;
    cargarCatalogoInicial(&catalogo);
    return 0;
}
Nodo *crearNodo(int tipo, char *variante, char *descripcion, float precio, ingredientes ingredientes, condimentos condimentos, int opcion)
{
    Nodo *n = (Nodo*)malloc(sizeof(Nodo));
    if (!n) {
        printf("No se pudo asignar memoria\n");
        return NULL;
    }
    n->tipo = tipo;



    strncpy(n->variante,  variante,  sizeof(n->variante)  - 1);

    strncpy(n->descripcion, descripcion, sizeof(n->descripcion) - 1);

    n->precio= precio;
    n->ingredientes = ingredientes;
    n->condimentos = condimentos;
    n->opcion = opcion;

    n->siguiente = NULL;
    return n;
}

void insertar(Nodo **cabeza, int tipo, char *variante, char *descripcion, float precio, ingredientes ingredientes, condimentos condimentos, int opcion)
{
    Nodo *nuevo = crearNodo(tipo, variante, descripcion, precio, ingredientes, condimentos, opcion);
    if (!nuevo) {
        return;
    }

    if (*cabeza == NULL) {
        *cabeza = nuevo;
        return;
    }

    Nodo *aux = *cabeza;
    if (*cabeza == NULL) {
        *cabeza = nuevo;
        return;
    }
    while (aux->siguiente != NULL) {
        aux = aux->siguiente;
    }
    aux->siguiente = nuevo;
}
void cargarCatalogoInicial(Nodo **cabeza){
    ingredientes ing_1 = { true, false, true, false, false, false };
    condimentos  cond_1 = { false, false, false, true, false, false, true };
    insertar(cabeza, TIPO_HAMBURGUESA,
             "Hamburguesa Juli crack",
             "Nuestra hamburguesa estrella, la favorita de messi, trae doble Carne, doble cheddar, bacon y nuestro aderezo especial",
             15000, ing_1, cond_1, 1);

    ingredientes ing_2 = { false, true, false, true, true, false };
    condimentos  cond_2 = { false, false, false, false, true, false, false };
    insertar(cabeza, TIPO_HAMBURGUESA,
             "Hamburguesa King Gaston ",
             "La hamburguesa favorita de la Reina Isabel segunda, tiene doble carne, provoleta, tomate, lechuga y mayonesa",
             19000, ing_2, cond_2, 2);

    ingredientes ing_3 = { true, false, true, true, true, false };
    condimentos  cond_3 = { false, true, false, false, false, false, false };
    insertar(cabeza, TIPO_HAMBURGUESA,
             "Hamburguesa Junior Engenieer",
             "Triple carne, triple cheddar, bacon, tomate, lechuga y barbacoa",
             18500.0f, ing_3, cond_3, 3);

    ingredientes ing_p1 = { true, false, true, false, false, false };
    condimentos  cond_p1 = { false, false, false, false, false, false, false };
    insertar(cabeza,  TIPO_PANCHO,
             "Panchito hoy se salio",
             "Cheddar y lluvia de bacon",
             6000, ing_p1, cond_p1, 4);

    ingredientes ing_p2 = { false, true, true, false, false, false };
    condimentos  cond_p2 = { false, false, false, false, false, false, true };
    insertar(cabeza,  TIPO_PANCHO,
             "Panchito subte e",
             "Provoleta, aderezo especial y bacon",
             7500, ing_p2, cond_p2, 5);

    ingredientes ing_p3 = { true, false, false, false, false, false };
    condimentos  cond_p3 = { false, false, false, false, false, false, false };
    insertar(cabeza, TIPO_PANCHO,
             "Panchito soy Ucemer",
             "Cheddar + frase motivadora",
             8200, ing_p3, cond_p3, 6);
}
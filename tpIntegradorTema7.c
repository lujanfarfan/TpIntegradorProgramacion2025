#include <stdio.h>

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
    struct Nodo *siguiente;
} Nodo;
Nodo *crearNodo(int tipo, char *variante, char *descripcion, float precio, ingredientes ingredientes, condimentos condimentos);
void insertar(Nodo **cabeza, int tipo, char *variante, char *descripcion, float precio, ingredientes ingredientes, condimentos condimentos);
void recorrer(Nodo **cabeza);
void votar(Nodo **cabeza);
void cerrarVotacion(Nodo *cabeza);
void cargarCatalogoInicial(Nodo **cabeza);
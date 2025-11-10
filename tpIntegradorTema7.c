#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

/*
 1) comentar codigo
 2) identar (por ahora hecho)
 3) diagramaa
 */
// los definimos como cuando lo haciamos con la cant de registros permitidos
// para poder modificar fcilmente el sitema sin tocar el codigo
// definimos los tipos de productos disponibles
// si el sistema se amplía (para agregar bebidas o postreso cookies)
// solo habría que agregar los aca
#define TIPO_PANCHO 0
#define TIPO_HAMBURGUESA 1

//represnta los ingredientes que itene cada producto si esta en true lo tiene si esta en false no
typedef struct {
    bool chedar;
    bool provoleta;
    bool bacon;
    bool tomate;
    bool lechuga;
    bool quesoDambo;
} ingredientes;

//representa los condimentos quer tiene cada producto del catalogo si esta en true lo tiene si esta en false no
typedef struct {
    bool mostaza;
    bool barbacoa;
    bool cesar;
    bool ketchup;
    bool mayonesa;
    bool salsGolf;
    bool aderezoEspecial;
} condimentos;

// representa un nodo de la lista enlazada que conforma el catlogo de productos, cada nodo es un producto
// fontiene la info completa de un producto y un puntero al siguiente nodo
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

// struct auxiliar usada para guardar los productos en el archivo bin
// contiene la misma información que Nodo pero sin el puntero siguiente
typedef struct {
    int tipo;
    char variante[60];
    char descripcion[100];
    float precio;
    ingredientes ingredientes;
    condimentos condimentos;
    int opcion;
} RegistroProducto;

//representa cada producto qu eel client pido dentro de un pedido
//sirve para guardar todos los procutos que cliente selecciono dentor de la struct para dsp copiarlos en el archivo
// cada item guarda el producto, la cantidad pedida y su subtotal
typedef struct ItemPedido {
    struct Nodo *producto;  // puntero al producto seleccionado del catslogo
    int cantidad;
    float subtotal;  // total parcial (precio * cantidad)
    struct ItemPedido *siguiente; // puntero al sigiente item dentro del pedido
} ItemPedido;

//tepresenta un pedido completo dentro del sistema
//contiene los datos del cliente, los items seleccionados y el estado del pedido
typedef struct Pedido {
    int id_pedido;
    char cliente[40];
    ItemPedido *items;   //puntero a la lista de productos dentro del pedido
    float total;
    bool metodo_pago;
    struct Pedido *siguiente;   //puntero al siguiente pedido en la lista
    bool entregado;  // estado del pedido (true: entregado, false: pendiente)
} Pedido;

// crea y devuelve un nodo de catalogo Parametros: datos del producto Devuelve: puntero al Nodo creado
Nodo *crearNodo(int tipo, char *variante, char *descripcion, float precio, ingredientes ing, condimentos cond, int opcion);

// inserta un producto al final de la lista del catslogo Parametros: **cabeza porque puede cambiarla direccion de inciio de la lista y datos del producto
//  enlaza el nuevo nodo en la lista y no devuelve valor
void insertar(Nodo **cabeza, int tipo, char *variante, char *descripcion, float precio, ingredientes ing, condimentos cond, int opcion);

// carga el catálogo inicial Parametros: **cabeza lista en memoria, **catalogo (FILE** para abrir/crear), nombre del archivo
// no devuelve valor
void cargarCatalogoInicial(Nodo **cabeza, FILE **catalogo, char *nombreArchivo);

// muestra por pantalla los productos del archiv Parametros: **pf (FILE** para abrir/leer), nombre del archivo, tipoFiltrar
// no devuelve valor
void recorrerArchivo_filtrado(FILE **pf, char *nombreArchivo, int tipoFiltrar);

// busca un producto por numero de opcion dentro del catalogo en memoria Parametros: cabeza de la lista y la opción buscada.
// devuelve: puntero al Nodo encontrado o NULL si no existr
Nodo* buscarPorOpcion(Nodo *cabeza, int opcion);

// agrega un ítem a un pedido existente y actualiza el total Parametros: pedido en construccion, producto elegido (Nodo), cantidad
// no devuelve valor
void agregarItemAlPedido(Pedido *pedido, Nodo *prod, int cantidad);

// interacta con el usuario para crear un pedido completo y lo guarda en historial.bin
// aarametros: catalogo en memoria y archivo historial
// devuelve: puntero al Pedido creado
Pedido* armarPedido(Nodo *catalogo, FILE **pf, char *nombreArchivo, FILE*historial, char*nombre_historial);

// marca como entregado el primer pedido pendiente en historial.bin Parametros: ninguno
// no devuelve valor
void entregarPedido();

// lista por pantalla la cola de pedidos pendientes leyendo historial.bin
void mostrarColaPedidos();

// menú principal que maneja dtodo Pametros: file, nombre del archivo del catálogo, lista del catálogo (Nodo*)
// no devuelve valor
void menu(FILE *archivoCatalogo, char *nombreArchivoCatalogo, Nodo *catalogo);

// genera log.txt con cantidad total vendida y dinero facturado (usa variables globales)
void registroVentas () ;

// muestra el historial (entregados y pendientes) leyendo historial.bi
// mo devuelve valor.
void verHistorialPedidos (FILE*historial, char*nombre_historial);

//libera toda la lista del catsdlogo en memoria Parametros: cabeza de la lista
void liberar_espacio (Nodo*p) ;

// libera la lista de ítems de un pedido Parametros: primer item
void liberar_espacio_item (ItemPedido*p) ;

// libera un pedido completo Parametros: puntero al pedido
void liberar_espacio_pedido (Pedido*p) ;

// cierra el programa liberando catalogo y la lista de pedidos encolados Parametros: catalogo y primer pedi
void salir (Nodo*catalogo, Pedido*primero) ;

// variables globales usadas para registrar las estadisticas del siste
//  cant total: cant total de pedidos realizados
//  dinero_facturado: acumula el monto total generado por todas las ventas
// se actualizan desde distintas funciones (armarPedido y registroVentas) y permiten generar reportes
int cant_total = 0;
float dinero_facturado = 0;

int main(void)
{
    //funcion la usamos para generar numeros aleatorios para el id del cleinte
    srand(time(NULL));
    // creamos e inciializamos el archivo del catalogo
    FILE *archivoCatalogo = NULL;
    char *nombreArchivoCatalogo = "catalogo.bin";
    // puntero a la estructura Nodo para representar el catalogo
    Nodo *catalogo = NULL;
    cargarCatalogoInicial(&catalogo, &archivoCatalogo, nombreArchivoCatalogo);
    // llamamos a la funcion menu pasando el archivo y el catslogo
    menu(archivoCatalogo, nombreArchivoCatalogo, catalogo);

    return 0;
}

void menu(FILE *archivoCatalogo, char *nombreArchivoCatalogo, Nodo *catalogo)
{
    FILE*historial= NULL ;
    char*nombre_historial= "historial.bin" ;
    int opcion;

    printf("Bienvenido a MC-KITTEN \n");

    do
    {
        printf("BIENVENIDO AL MENU \n");
        printf("elija su opcion: \n");
        printf("1) crear pedido \n");
        printf("2) entregar pedido \n");
        printf("3)  ver estadisticas \n");
        printf("4)  ver pedidos pendientes \n");
        printf("5)  salir   \n");


        scanf("%d", &opcion);
        switch (opcion)
        {
            case 1:
                printf("Tipo de HAMBURGUESAS\n");
                recorrerArchivo_filtrado(&archivoCatalogo, nombreArchivoCatalogo, TIPO_HAMBURGUESA);
                printf("Tipo de PANCHOS\n");
                recorrerArchivo_filtrado(&archivoCatalogo, nombreArchivoCatalogo, TIPO_PANCHO);
                armarPedido(catalogo, &archivoCatalogo, nombreArchivoCatalogo, historial, nombre_historial);
                break;

            case 2:

                entregarPedido();
                //mostrarColaPedidos();

            break;

            case 3:
                 registroVentas();
            break;
            case 4:
                mostrarColaPedidos();
            break;


            case 5:
                salir(catalogo, NULL);
            break;

            default:
                printf("Opcion no valida\n");
            break;
        }


    }
        while(opcion!=5);
}



Nodo *crearNodo(int tipo, char *variante, char *descripcion, float precio, ingredientes ing, condimentos cond, int opcion)
{
    Nodo *n = (Nodo*)malloc(sizeof(Nodo));
    if (!n)
    {
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
    if (!nuevo)
    {
        printf("no se pudo crear el nodo  \n");
        return;
    }

        if (*cabeza == NULL)
        {
            *cabeza = nuevo;
            return;
        }
            Nodo *aux = *cabeza;
            while (aux->siguiente)
            {
                aux = aux->siguiente;
            }
                aux->siguiente = nuevo;

}


void cargarCatalogoInicial(Nodo **cabeza, FILE **catalogo, char *nombreArchivo)
{
    *catalogo = fopen(nombreArchivo, "wb");
    if (!*catalogo)
    {
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
    if (!*catalogo)
    {
        printf("no se pudo abrir el archivo \n");
        return;
    }

        RegistroProducto r;
        int hay = 0;
        while (fread(&r, sizeof(RegistroProducto), 1, *catalogo) == 1)
        {
            if (tipoFiltrar == -1 || r.tipo == tipoFiltrar)
             {
                hay = 1;
                if (r.tipo == TIPO_HAMBURGUESA)
                    printf("--- HAMBURGUESA OPCION %d ---\n", r.opcion);
                else
                    printf("--- PANCHO OPCION %d ---\n", r.opcion);

                printf("%s\n %s\n $%.2f\n \n", r.variante, r.descripcion, r.precio);
            }
        }

            if (!hay)
            {
                if (tipoFiltrar == TIPO_HAMBURGUESA)
                {
                    printf("lpn no se guardo bien\n");
                }
                else if (tipoFiltrar == TIPO_PANCHO)
                {
                    printf("lpn no se guardo bien\n");
                }
                else
                {
                    printf("bua no hay ni producos\n");
                }
            }

    fclose(*catalogo);

}

Nodo* buscarPorOpcion(Nodo *cabeza, int opcion)
{
    while (cabeza)
    {
        if (cabeza->opcion == opcion)
        {
            return cabeza;
        }
            cabeza = cabeza->siguiente;
    }

    return NULL;
}

void agregarItemAlPedido(Pedido *pedido, Nodo *prod, int cantidad)
{
    if (!pedido || !prod || cantidad <= 0)
    {
        return;
    }

        ItemPedido *nuevo = (ItemPedido*)malloc(sizeof(ItemPedido));

        if (!nuevo)
        {
            printf("no se pudo asignar memoria para item\n");
            return;
        }

            nuevo->producto = prod;
            nuevo->cantidad = cantidad;
            nuevo->subtotal = prod->precio * cantidad;
            nuevo->siguiente = NULL;

            if (!pedido->items)
            {
                pedido->items = nuevo;
            }
            else
            {
                ItemPedido *aux = pedido->items;
                while (aux->siguiente) aux = aux->siguiente;
                aux->siguiente = nuevo;
            }

                pedido->total += nuevo->subtotal;

}




Pedido* armarPedido(Nodo *catalogo, FILE **pf, char *nombreArchivo, FILE* historial, char* nombre_historial)
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

    p->id_pedido   = rand() % 1000;
    p->items       = NULL;
    p->total       = 0.0f;
    p->metodo_pago = false;
    p->siguiente   = NULL;
    p->entregado   = false;

    printf("ingrese su nombre: ");
    scanf(" %39[^\n]", p->cliente);

    printf("HOLA %s BIENVENIDO A -MC KITTEN-! TU ID DE PEDIDO ES #%d\n", p->cliente, p->id_pedido);


    int seguir = 1;
    while (seguir) {
        int tipo = -1;
        int ok;

        printf("que queres pedir? (0 = PANCHOS, 1 = HAMBURGUESAS): ");
        ok = scanf("%d", &tipo);
        if (ok != 1 || (tipo != TIPO_PANCHO && tipo != TIPO_HAMBURGUESA)) {
            printf("opcion invalida (ingresa 0 o 1)\n");
            continue;
        }

        if (tipo == TIPO_PANCHO) {
            printf("=== CATALOGO PANCHOS ===\n");
            recorrerArchivo_filtrado(pf, nombreArchivo, TIPO_PANCHO);
        } else {
            printf("=== CATALOGO HAMBURGUESAS ===\n");
            recorrerArchivo_filtrado(pf, nombreArchivo, TIPO_HAMBURGUESA);
        }


        int opcionElegida = 0, cantidad = 0;

        printf("ingrese el numero de OPCION del producto: ");
        ok = scanf("%d", &opcionElegida);
        if (ok != 1) {
            printf("opcion invalida \n");
            continue;
        }

        printf("cantidad: ");
        //comprobamos que sea valido el numero que ingreo
        ok = scanf("%d", &cantidad);
        if (ok != 1 || cantidad <= 0) {
            printf("cantidad invalida\n");
            continue;
        }



        Nodo *prod = buscarPorOpcion(catalogo, opcionElegida);
        if (!prod) {
            printf("no existe la opcion \n");
            continue;
        }
        //verificamos que el tipo elegido coincida con lo que el usuario elije (panchos o burgers)

        else if (prod->tipo != tipo) {
            printf("la opcion no es del tipo elegido\n");
            continue;
        } else {
            agregarItemAlPedido(p, prod, cantidad);
            cant_total += cantidad;
            printf("agregado: %s x%d ($%.2f c/u) \n -- subtotal: $%.2f  total: $%.2f\n", prod->variante, cantidad, prod->precio, prod->precio * cantidad, p->total);
        }

        printf("agregar otro producto? (1 = si, 0 = no): ");
        scanf("%d", &seguir);
        if (seguir != 1) {
            seguir = 0;
        }
    }
    historial = fopen(nombre_historial, "ab");
    if (!historial) {
        printf("error al abrir historial\n");
    } else {
        int cant_items = 0;
        ItemPedido *auxc = p->items;
        while (auxc) {
            cant_items++;
            auxc = auxc->siguiente;
        }
        //ecribimos el pedido en el historial para que quede registro
        //aca esta la info general
        fwrite(&p->id_pedido, sizeof(int),   1, historial);
        fwrite(p->cliente,   sizeof(p->cliente), 1, historial);
        fwrite(&p->total,    sizeof(float),  1, historial);
        fwrite(&p->metodo_pago, sizeof(bool), 1, historial);
        fwrite(&p->entregado,  sizeof(bool), 1, historial);
        fwrite(&cant_items,  sizeof(int),    1, historial);

        //aca lo que hacemos es guardar los items del pedido del cleinte dentro del struct
        //esto se repite hasta que no queden mas items
        ItemPedido *aux = p->items;
        while (aux) {
            RegistroProducto r;
            r.tipo  = aux->producto->tipo;
            strcpy(r.variante,    aux->producto->variante);
            strcpy(r.descripcion, aux->producto->descripcion);
            r.precio  = aux->producto->precio;
            r.ingredientes = aux->producto->ingredientes;
            r.condimentos  = aux->producto->condimentos;
            r.opcion  = aux->producto->opcion;

            // linea top, guarda en el archviotodo lo que antes guardamos en la strcut r  para que alla aparezcan los items
            fwrite(&r,sizeof(RegistroProducto), 1, historial);
            fwrite(&aux->cantidad, sizeof(int), 1, historial);
            fwrite(&aux->subtotal, sizeof(float),1, historial);

            aux = aux->siguiente;
        }

        fclose(historial);
    }


    printf("--- RESUMEN PEDIDO #%d ---\n", p->id_pedido);
    printf("Cliente: %s\n", p->cliente);
    //la usamos para recorrer la lista de productos que pido el cliente
    //recorre la struct cargada en memoria proque no queria reocrrer historial de vuelta, mas abajo lo hare!!!!!!!!!
    ItemPedido *auxp = p->items;
    if (!auxp) printf("sin items\n");
    while (auxp) {
        printf(" - %s x%d  ($%.2f c/u) -> $%.2f\n",
               auxp->producto->variante, auxp->cantidad, auxp->producto->precio, auxp->subtotal);
        auxp = auxp->siguiente;
    }
    printf("TOTAL: $%.2f\n", p->total);
    dinero_facturado += p->total;

    //devuelve el puntero al pedido p que lo vamos ausar dsp para encolarlo
    return p;
}



void registroVentas ()
{
    FILE*archivo= NULL ;
    char *nombre_archivo= "log.txt" ;
    archivo= fopen (nombre_archivo, "w") ;
    if (archivo== NULL)
    {
        printf ("error") ;
        return ;
    }
        fprintf (archivo, " CANTIDAD DE PRODCTOS VENDIDOS:%d \n TOTAL DINERO FACTURADO: %f \n", cant_total, dinero_facturado) ;
        fclose (archivo) ;
}
void entregarPedido()
{
    char *nombre_historial = "historial.bin";
    FILE *f = fopen(nombre_historial, "r+b");
    if (!f) {
        printf("no hay historial o no se pudo abrir\n");
        return;
    }

    //HACEMOS EL WHILE INFINITO PORQUE NO SABEMOS CUENTOS REGISTROS TENEMOS, PEOR CUANDO YA NO LEAMOS MAS LO CORTAMOS CON BREAK

    while (1) {
        //nos sirven para guardar temporalmente lo que leemos del archvio
        int id_pedido;
        char cliente[40];
        float total;
        bool metodo_pago;
        bool entregado;
        int cant_items;

        //recordamos en que posicion del archvoo se gusrda entregado para dsp saber donde modificar
        long pos_entregado;

//leemos una parte del pedido desde el archvio y la guardamos en las variables que hicmos arriba
        //si alguna lectura nos falla es porque llego a su fin o hay error  y entocnes cortamos tod\o y salimos dle while
        if (fread(&id_pedido, sizeof(int), 1, f) != 1){
            break;
            }
        if (fread(cliente, sizeof(cliente), 1, f) != 1) {
            break;
        }
        if (fread(&total, sizeof(float), 1, f) != 1) {
            break;
        }
        //al final no lo usamos para nada, dsp lo tenemos que borrar!!!!!!!!!!!!!!
        if (fread(&metodo_pago, sizeof(bool), 1, f) != 1) {
            break;
        }

//nos devuelve donde estamos parados, antes de que empiece entregado
        pos_entregado = ftell(f);
//leemos el valor de entregado y comprobamos que no falle (sin esto me falla )
        if (fread(&entregado, sizeof(bool), 1, f) != 1) {
            break;
        }
        if (fread(&cant_items, sizeof(int), 1, f) != 1) {
            break;
        }

        if (!entregado) {
            //var con valor true para dsp pasarsela alar chvio
            bool nuevo = true;
            //movemos el cursor a donde esta el campo entregdo
            fseek(f, pos_entregado, SEEK_SET);
            //sescribimos true en ese lugar
            fwrite(&nuevo, sizeof(bool), 1, f);
            fflush(f);

//MUY IMPORTANTE con esto movemos el cursor hasta el inicio de los items del pedido actual
//pos entregado apunta a el campo entregado asi que le sumaos el bool (entregado=fasle/true)
// y tmb el tamano de cant items para poner el cursor donde arranca ek 1er item
            fseek(f, pos_entregado + sizeof(bool) + sizeof(int), SEEK_SET);
            printf("ENTREGANDO PEDIDO #%d\n", id_pedido);
            printf("Cliente: %s", cliente);
            printf("Total: $%.2f\n", total);
            printf("Items (%d):\n", cant_items);

            int i;
            //recorremos todos los items
            for (i = 0; i < cant_items; i++) {
                RegistroProducto r;
                int cantidad;
                float subtotal;
                //los imprimmos
                fread(&r, sizeof(RegistroProducto), 1, f);
                fread(&cantidad, sizeof(int), 1, f);
                fread(&subtotal, sizeof(float), 1, f);

                printf("  - %s x%d ($%.2f c/u) -- $%.2f\n",r.variante, cantidad, r.precio, subtotal);
            }

            printf("oedido entregado con exito\n");
            fclose(f);
            return;
        } else {
//si el pedido ya se entrego tenemos que mover el cursor al siguiente pedido
            int i;
            for (i = 0; i < cant_items; i++) {
                //MUY IMPORTAMNTE tnemos que calcular cuanto ocupa para saltarlo y poner el cursor al inicio del otro pedido
                //saltea la struct completa dle producto, y el total
                fseek(f, (int)sizeof(RegistroProducto) + sizeof(int) + sizeof(float), SEEK_CUR);
            }

        }
    }

    printf("no se encontro  pedido pendiente para entrega\n");
    fclose(f);
}


void mostrarColaPedidos()
{
    char *nombre_historial = "historial.bin";
    FILE *f = fopen(nombre_historial, "rb");
    if (!f) {
        printf("no hay historial o no se pudo abrir\n");
        return;
    }
    //contador para saber cuantos pedidos pendietnes tenemos en total
    int totalPendientes = 0;
    //numeraraodr de pedidos
    int nro = 1;

    printf(" PEDIDOS pendientes \n");

    //se corta cuando no hay mas anda para leer
    while (1) {
        //nos sirven para guardar temporalmente lo que leemos del archvio
        int id_pedido;
        char cliente[40];
        float total;
        bool metodo_pago;
        bool entregado;
        int cant_items;

        //leemos una parte del pedido desde el archvio y la guardamos en
        //las variables que hicmos arriba
        //si alguna lectura nos falla es porque llego a su fin o hay error
        //y entocnes cortamos tod\o y salimos dle while
        if (fread(&id_pedido, sizeof(int), 1, f) != 1) {
            break;
        }
        if (fread(cliente, sizeof(cliente), 1, f) != 1) {
            break;
        }
        if (fread(&total, sizeof(float), 1, f) != 1) {
            break;
        }
        if (fread(&metodo_pago, sizeof(bool), 1, f) != 1) {
            break;
        }
        if (fread(&entregado, sizeof(bool), 1, f) != 1) {
            break;
        }
        if (fread(&cant_items, sizeof(int), 1, f) != 1) {
            break;
        }

        //si no se entrego imprimos la info
        if (!entregado) {
            printf("Pedido %d:\n", nro);
            printf("ID pedido: #%d\n", id_pedido);
            printf("Cliente: %s", cliente);
            printf("Total a pagar: $%.2f\n", total);
            printf("Items (%d):\n", cant_items);


            int i;
            //si no se entrego recorremos todsos los items
            for (i = 0; i < cant_items; i++) {
                //variables temprales para guardfar lo que se leee del archivo
                RegistroProducto r;
                int cantidad;
                float subtotal;

                fread(&r, sizeof(RegistroProducto), 1, f);
                fread(&cantidad, sizeof(int), 1, f);
                fread(&subtotal, sizeof(float), 1, f);

                printf("  - %s x%d ($%.2f c/u) -- $%.2f\n", r.variante, cantidad, r.precio, subtotal);
            }
            printf("\n");
            totalPendientes++;
            nro++;
        }
        //si ya se enrego
        else {

            int i;
            for (i = 0; i < cant_items; i++) {
                //avanzamos el cursos, es = a la funcion anterior
                fseek(f, (int)sizeof(RegistroProducto) + sizeof(int) + sizeof(float), SEEK_CUR);
            }
        }
    }

    if (totalPendientes == 0) {
        printf("sin pedidos pendientesc\n");
    }


    printf("TOTAL DE PEDIDOS EN COLA: %d\n", totalPendientes);

    fclose(f);
}

void verHistorialPedidos (FILE*historial, char*nombre_historial)
{
    historial = fopen(nombre_historial, "rb");
    if (!historial) {
        printf("error\n");
        return;
    }

    printf(" HISTORIAL DE PEDIDOS \n");
    //IDEM FUNBCIOES ANTERIORES
    while (1) {
        int id_pedido;
        char cliente[40];
        float total;
        bool metodo_pago;
        bool entregado;
        int cant_items;

        if (fread(&id_pedido, sizeof(int), 1, historial) != 1) {
            break;
        }
        if (fread(cliente, sizeof(cliente), 1, historial) != 1) {
            break;
        }
        if (fread(&total, sizeof(float), 1, historial) != 1) {
            break;
        }
        if (fread(&metodo_pago, sizeof(bool), 1, historial) != 1) {
            break;
        }
        if (fread(&entregado, sizeof(bool), 1, historial) != 1) {
            break;
        }
        if (fread(&cant_items, sizeof(int), 1, historial) != 1) {
            break;
        }

        //imrpimimoslos datos
        printf("Pedido #%d | Cliente: %sTotal: $%.2f | Estado: ", id_pedido, cliente, total);

        //si se entrego lo imprimos entregado y si no pendiente
        if (entregado) {
            printf("ENTREGADO");
        }
        else {
            printf("PENDIENTE");

        }
        //imprime cant d eitems de ese peiddo
        printf(" Items: %d\n", cant_items);


        int i;
        //recoremops todos los itewms del pedido
        for (i = 0; i < cant_items; i++) {
            //como siemore lo imprimos
            RegistroProducto r;
            int cantidad;
            float subtotal;

            fread(&r, sizeof(RegistroProducto), 1, historial);
            fread(&cantidad, sizeof(int), 1, historial);
            fread(&subtotal, sizeof(float), 1, historial);

            printf("  %s x%d ($%.2f c/u) -> $%.2f\n", r.variante, cantidad, r.precio, subtotal);
        }
        printf("\n");
        }


    fclose(historial);
}

void liberar_espacio (Nodo*p)
{
    Nodo*aux ;
    while (p)
    {
        aux= p ;
        p= p->siguiente ;
        free (aux) ;
    }
}
void liberar_espacio_item (ItemPedido*p)
{
  ItemPedido*aux ;
    while (p)
    {
        aux= p ;
        p= p->siguiente ;
        free (aux) ;
    }
}
void liberar_espacio_pedido (Pedido*p)
{
  if (!p) return ;
    liberar_espacio_item (p->items) ;
    free (p) ;
}
void salir (Nodo*catalogo, Pedido*primero)
{
    printf("Liberando memoria y cerrando\n");
    liberar_espacio(catalogo) ;
    while (primero)
    {
        Pedido *p = primero;
        primero = primero->siguiente;
        liberar_espacio_pedido(p);
    }
        system ("pause") ;
        printf ("Gracias por usar nuestro sistema de pedidos \n") ;

}

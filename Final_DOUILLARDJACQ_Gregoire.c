
/*===========================================================================================================*/
/*
PREGUNTA TEORICA:
Podemos usar una formula basica dada como: T = (Cantidad de ciclos de reloj)/(Frecuencia de reloj)
Para MCU1: T = 4/700Mhz = 5.714 E-6
Para MCU2: T = 1/200Mhz = 5.0 E-6

5.0E-6 < 5.714E-6 -> Tmcu2 < Tmcu1

Entonces, MCU2 es el mas rapido porque necesita hacer solamente 1 cyclo y no 4 como MCU1 con sus 8 bits para hacer une operacion con 32bits (8*4bits).
El tiempo para hacer esta operacion es menor con MCU2.
*/
/*================================================ INICIALIZACION ====================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo{
    unsigned char valor;
    struct Nodo *sig;
}Nodo;

/*================================================ FUNCIONES ====================================================*/

/*----------------------------------------------------------------*/

FILE* AbrirArchivo(const char* filename, const char* modo){ //char es usado si hay solemente 1 caracteria. char* est pour une chaine de caractère (tableau avec le pointeur). const char* dit que la fonction ne doit pas modifier la chaine de caractere.
    FILE* f;

    f = fopen(filename, modo);

    if(f == NULL){
        printf("Error al abrir el archivo\n");
        return NULL;
    }
    printf("Archivo cargado ok\n");

    return f;
}

/*----------------------------------------------------------------*/

void CerrarArchivo(FILE* f){
    if (fclose(f)!=0){
        printf("Error al cerrar\n");
        exit(1);
    }
    printf("archivo cerrado ok\n");
}

/*----------------------------------------------------------------*/

// funcion para escribir una cadena en un archivo
void EscribirEnArchivo(FILE* f, unsigned char v){ //size_t est utilisé pour définir les tailles d'éléments car un mode size_t ne peut pas être négatif (spécification : %zu)
    size_t n;

    n = fwrite(&v, sizeof(unsigned char),1, f); // sizeof(char) devuelve 1 (tamano en bits de un char), quantidad = char*quantidad de letras. n es la longitud de lo que es escrito

    if (n==0){
        printf("No se escribo bien !\n");
        exit(1);
    }
}

/*----------------------------------------------------------------*/

Nodo* Ingresar_FIFO(Nodo* l, unsigned char a){
    Nodo* nuevo, *aux;

    nuevo = (Nodo*)malloc(sizeof(Nodo));

    nuevo -> valor = a;
    nuevo->sig = NULL;

    if(l==NULL){
        return nuevo;
    }

    aux = l;
    while(aux->sig != NULL){
        aux = aux->sig;
    }
    aux->sig = nuevo;
    return l;
}

/*----------------------------------------------------------------*/

Nodo *destruir (Nodo *l){
    Nodo *aux;

    while (l != NULL) {
        aux = l;
        l = l->sig;
        free (aux);
    }
    printf("Lista destruida.\n");
    return NULL;
}

/*----------------------------------------------------------------*/

void Mostrar_lista(Nodo* l){
    Nodo* aux;
    int i = 0;

    if(l==NULL){
        printf("Lista vacia.\n");
        return;
    }

    aux=l;
    printf("--- LISTA ---\n");
    while(aux != NULL){
        i++;
        printf("Nodo %d -> VALOR: %hhu",i, (unsigned char)aux->valor); // remettre en unsigned char car sinon il y a des bits qui se rajoutent.
        printf("\n");
        aux = aux->sig;
    }
    printf("TOTAL de nodos : %d\n", i++);
}

/*----------------------------------------------------------------*/

void Hallar_Impares(Nodo* l){
    Nodo* aux;
    int i=0;
    int suma_impaires = 0, cantidad_impares = 0;
    double promedio = 0;

    if(l==NULL){
        printf("Lista vacia.\n");
        return;
    }

    aux=l;
    printf("--- Elemento impares ---\n");
    while(aux != NULL){
        i++;
        if(i % 2 !=0){
            suma_impaires += aux->valor;
            cantidad_impares++;
        }
        aux = aux->sig;
    }

    if (cantidad_impares > 0) {
        promedio = (double)suma_impaires / cantidad_impares;
        printf("\nPromedio de les elementos impares : %.2f\n", promedio);
    } else {
        printf("\nNingun elementos impares.\n");
    }

    return;
}

/*================================================ PREGUNTAS ====================================================*/

Nodo* Pregunta1(unsigned char* v, int cant, Nodo* l){

    if (l == NULL){

        for(int i=0;i<cant;i++){
            l = Ingresar_FIFO(l, *v);
            v++;
        }

        Mostrar_lista(l);
        Hallar_Impares(l);
    }
    else{
        printf("La lista ya tiene datos. No puede hacer ninguna accion.\n");
    }


    return l;
}

/*----------------------------------------------------------------*/

FILE* Pregunta2(FILE* f, Nodo* l) {
    Nodo* aux;
    f = AbrirArchivo("filtrado.dat", "wb");

    aux = l;
    while (aux != NULL) {
        if (aux->sig != NULL && aux->sig->valor % 2 == 0) {
            EscribirEnArchivo(f, aux->valor);
        }
        aux = aux->sig;
    }
    CerrarArchivo(f);

    return f;
}

/*----------------------------------------------------------------*/

void Pregunta3(unsigned char* v, int cant){
    int count, count2 = 0, cantidadcero = 0;
    float porcentaje;

    for (int i=0; i<cant;i++){
        count = 0;

        for(int j=0; j<8;j++){

            if(((v[i] >> j) & 1) == 0){
                count++;
            }
        }

        if (count >= 3){
            count2++;
            printf("%hhu tiene al menos 3 bits de valor 0\n", v[i]);
        }

        cantidadcero += count;
    }

    printf("------------------------------\n");
    printf("Hay %d valores que cumplen esta condicion en v\n", count2);

    porcentaje = (100*cantidadcero)/(8*cant);
    printf("El porcentaje total es de %f\n", porcentaje);
    printf("------------------------------\n");

    return;
}

/*===========================================================================================================*/
/*================================================= MAIN ====================================================*/
/*===========================================================================================================*/

int main(){
    int op;
    int cant;
    unsigned char* v;
    Nodo* l = NULL;
    FILE* f;

    printf("Ingrese la cantidad deseada:\n");
    scanf("%d", &cant);

    v = (unsigned char*) malloc (sizeof (unsigned char)*cant);

    if (v == NULL) {
        printf("Error de asignacion de memoria\n");
        return 1;
    }

    for(int j=0; j<cant;j++){
        v[j] = (3*j+7)%256;
    }

    do{
        printf("\n---------Menu-----------\n");
        printf("1) Anadir en una lista los valores de v y calcular el promedio.\n");
        printf("2) Escribir en 'filtrado.dat' los valores que tienen su siguiente nodo contiene un valor par.\n");
        printf("3) Cuentar los valores que tienen al menos 3 bits en 0 y calcular el porcentaje total de bit igual a 0 en el vector.\n");
        printf("4) Salir.\n");
        scanf("%d", &op);

        switch (op) {
        case 1:
            l = Pregunta1(v, cant, l);
            break;
        case 2:
            f = Pregunta2(f, l);
            break;
        case 3:
            Pregunta3(v, cant);
            break;
        default:
            printf("Opcion incorrecta\n");
            break;
        }
    }while (op != 4);
    destruir(l);
    free(v);

    return 0;
}

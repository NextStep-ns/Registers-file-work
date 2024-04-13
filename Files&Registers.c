#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo{
    int binario;
    int posicion;
    struct Nodo* sig;
    struct Nodo* pre;
}Nodo;

/*----------------------------------------------------------------*/
/*----------------------------------------------------------------*/

Nodo* Ingresar_FIFO(Nodo* l, int binario, int count){
    Nodo* nuevo, *aux;

    nuevo = (Nodo*)malloc(sizeof(Nodo));

    nuevo->binario = binario;
    nuevo->posicion = count;
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

void Mostrar_lista_inverso(Nodo* l) {
    Nodo *aux;

    if(l==NULL){
        printf("Lista vacia.\n");
        return;
    }

    aux = l;
    printf("--- LISTA (inverso) ---\n");
    while (aux != NULL) {
        printf("pos: %d -> ", aux->posicion);
        printf("HEXA: 0x%02X", (unsigned char)aux->binario);
        printf("\n");
        aux = aux->pre;
    }
}

/*----------------------------------------------------------------*/

void Mostrar_lista(Nodo* l){
    Nodo* aux;

    if(l==NULL){
        printf("Lista vacia.\n");
        return;
    }

    aux=l;
    printf("--- LISTA ---\n");
    while(aux != NULL){
        printf("pos: %d -> ", aux->posicion);
        printf("HEXA: 0x%02X", (unsigned char)aux->binario); // remettre en unsigned char car sinon il y a des bits qui se rajoutent.
        printf("\n");
        aux = aux->sig;
    }
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

FILE* AbrirArchivo(const char* filename, const char* modo){ //char es usado si hay solemente 1 caracteria. char* est pour une chaine de caractère (tableau avec le pointeur). const char* dit que la fonction ne doit pas modifier la chaine de caractere.
    FILE* f;

    f = fopen(filename, modo);

    if(f == NULL){
        printf("Error al abrir el archivo\n");
        exit(1);
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
void EscribirEnArchivo(FILE* f, int v){ //size_t est utilisé pour définir les tailles d'éléments car un mode size_t ne peut pas être négatif (spécification : %zu)
    int n;

    n = fwrite(&v, sizeof(char),1, f); // sizeof(char) devuelve 1 (tamano en bits de un char), quantidad = char*quantidad de letras. n es la longitud de lo que es escrito

    if (n==0){
        printf("No se escribo bien !\n");
        CerrarArchivo(f);
        exit(1);
    }
}

/*----------------------------------------------------------------*/

Nodo *LeerUnArchivo(FILE* f, Nodo *l){
    char valor;
    int count = 0;
    Nodo* tail = NULL;

    //nuevo = (NodoSerial*)malloc(sizeof(NodoSerial));

    l = destruir(l);
    while(fread(&valor, sizeof(char), 1, f) == 1){
        l = Ingresar_FIFO(l,valor, count);

        if (tail == NULL) {
            tail = l;
        } else {
            tail->sig->pre = tail;
            tail = tail->sig;
        }
        count++;
    }

    if(l==NULL){
        printf("Error al leer del archivo\n");
        CerrarArchivo(f);
        exit(1);
    }
    else{
        printf("Leer archivo ok\n");
        Mostrar_lista(l);
        Mostrar_lista_inverso(tail);
        CerrarArchivo(f);
    }
    return l;
}

/*----------------------------------------------------------------*/
/*----------------------------------------------------------------*/

void Pregunta1(unsigned char *r){

    while(*r != '\0'){
        printf("%c -> HEXA: 0x%02X", *r, *r);

        if(((*r & 0x0F) <= 0x09) && ((*r & 0xF0) <= 0x90)){
            printf("  -  BCD : SI");
        }
        else{
            printf("  -  BCD : NO");
        }
        printf("\n");
        r++;
    }
    return;
}

/*----------------------------------------------------------------*/

void Pregunta2(unsigned char *r, FILE* f){
    int v;

    f = AbrirArchivo("datos.bin", "wb");

    while(*r != '\0'){
        //toBin2(*r, binario);
        //binario[0] = 1;

        v = *r;
        v = v | 0x80;

        //Mostrarlo en la pantalla
        printf("Nuevo valor : %x\n", v);

        //Grabarlo en un archivo
        EscribirEnArchivo(f, v);

        r++;
    }
    CerrarArchivo(f);
    return;
}

/*----------------------------------------------------------------*/

void Pregunta3(unsigned char *r){
    int i = 0;

    while(*r != '\0'){

        if(i == 12){

            if ((*r & 0x80) != 0){
                printf("IRQF activado.\n");
            }
            else{
                printf("IRQF no activado.\n");
            }
            if((*r & 0x40) != 0){
                printf("PF activado.\n");
            }
            else{
                printf("PF no activado.\n");
            }

            if((*r & 0x20) != 0){
                printf("AF activado.\n");
            }
            else{
                printf("AF no activado.\n");
            }

            if((*r & 0x10) != 0){
                printf("UF activado.\n");
            }
            else{
                printf("UF no activado.\n");
            }
        }
        r++;
        i++;
    }
    return;
}

/*----------------------------------------------------------------*/

void Pregunta4(FILE* f, Nodo* l){
    f = AbrirArchivo("datos.bin", "rb");
    LeerUnArchivo(f, l);

    return;
}

/*----------------------------------------------------------------*/
/*----------------------------------------------------------------*/
/*----------------------------------------------------------------*/

int main(){
    int op;
    FILE *f;
    Nodo* l = NULL;
    unsigned char *r = (unsigned char*) "NO TE LO PUEDO EXPLICAR porque no vas a entender";

    do{
        printf("\n---------Menu-----------\n");
        printf("1) Mostrar el vector.\n");
        printf("2) Binario.\n");
        printf("3) Registro C.\n");
        printf("4) Recuperar del archivo.\n");
        printf("5) Salir.\n");
        scanf("%d", &op);

        switch (op) {
        case 1:
            Pregunta1(r);
            break;
        case 2:
            Pregunta2(r, f);
            break;
        case 3:
            Pregunta3(r);
            break;
        case 4:
            Pregunta4(f, l);
            break;
        default:
            printf("Opcion incorrecta\n");
            break;
        }
    }while (op != 5);

    return 0;
}

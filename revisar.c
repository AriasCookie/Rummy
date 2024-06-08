struct Jugada **RomperJugadas(struct Jugada *jugada) {
    if (jugada == NULL || jugada->cabeza == NULL) {
        printf("La jugada está vacía.\n");
        return NULL;
    }

    printf("Jugada original:\n");
    imprimirJugada(jugada);

    int numJugadas;
    printf("¿En cuántas jugadas desea dividir la jugada original? ");
    scanf("%d", &numJugadas);

    // Validar el número de jugadas
    if (numJugadas <= 0) {
        printf("Número de jugadas no válido.\n");
        return NULL;
    }

    // Array para almacenar los límites de los índices de las jugadas
    int limites[numJugadas][2];

    // Pedir al usuario los límites de los índices para cada jugada
    for (int i = 0; i < numJugadas; i++) {
        printf("Para la jugada %d, ingrese los índices de inicio y fin (separados por espacio): ", i + 1);
        scanf("%d %d", &limites[i][0], &limites[i][1]);

        // Validar los índices ingresados
        if (limites[i][0] < 0 || limites[i][0] >= limites[i][1] || limites[i][1] >= jugada->tamanio) {
            printf("Índices no válidos.\n");
            return NULL;
        }
    }

    // Array para almacenar las subjugadas que cumplen algún caso
    struct Jugada **subjugadas = (struct Jugada **)malloc(numJugadas * sizeof(struct Jugada *));
    if (subjugadas == NULL) {
        printf("Error al asignar memoria para el arreglo de subjugadas.\n");
        return NULL;
    }

    // Crear las nuevas jugadas y dividir la jugada original
    int numSubjugadas = 0;
    for (int i = 0; i < numJugadas; i++) {
        struct Jugada *nuevaJugada = (struct Jugada *)malloc(sizeof(struct Jugada));
        if (nuevaJugada == NULL) {
            printf("Error al asignar memoria para la nueva jugada.\n");
            // Liberar memoria asignada previamente
            for (int j = 0; j < numSubjugadas; j++) {
                liberarJugada(subjugadas[j]);
            }
            free(subjugadas);
            return NULL;
        }

        nuevaJugada->cabeza = NULL;
        nuevaJugada->tamanio = 0;

        struct Nodo *current = jugada->cabeza;
        struct Nodo *prev = NULL;
        int count = 0;

        // Recorrer la jugada original hasta el límite del índice actual
        while (count < limites[i][0]) {
            prev = current;
            current = current->siguiente;
            count++;
        }

        // Dividir la jugada original hasta el límite del índice actual
        while (count <= limites[i][1]) {
            struct Nodo *nuevoNodo = (struct Nodo *)malloc(sizeof(struct Nodo));
            if (nuevoNodo == NULL) {
                printf("Error al asignar memoria para un nodo.\n");
                // Liberar memoria asignada previamente
                for (int j = 0; j < numSubjugadas; j++) {
                    liberarJugada(subjugadas[j]);
                }
                free(subjugadas);
                liberarJugada(nuevaJugada);
                return NULL;
            }

            nuevoNodo->ficha = current->ficha;
            nuevoNodo->siguiente = NULL;
            nuevoNodo->anterior = NULL;

            if (nuevaJugada->cabeza == NULL) {
                nuevaJugada->cabeza = nuevoNodo;
            } else {
                prev->siguiente = nuevoNodo;
                nuevoNodo->anterior = prev;
            }
            nuevaJugada->tamanio++;

            prev = current;
            current = current->siguiente;
            count++;
        }

        // Realizar una copia de la subjugada para verificar los casos sin modificar la original
        struct Jugada *copiaSubjugada = copiarJugada(nuevaJugada);
      
        // Modifica esta linea porfa es con lo de agregar linea por indice aqui se hace como una cierta prueba pero no se como quieres agregarlas 
        // Llamar a la función agregarFicha para agregar una ficha a la nueva jugada
        // Aquí llamamos a agregarFicha con los valores deseados
        struct Fichas fichaDeseada;
        fichaDeseada.color = "Azul"; // Color deseado
        fichaDeseada.numero = 8;     // Número deseado
        agregarFicha(nuevaJugada, fichaDeseada, 0); // Agregar al principio de la nueva jugada
        //Modificacion hasta aquiporfa 
      
        // Verificar si la copia de la subjugada cumple con el caso 1 o el caso 2
        if (cumpleCaso1(nuevaJugada) || cumpleCaso2(nuevaJugada)) {
            // Si la copia cumple con alguno de los casos, agregamos la subjugada al arreglo de subjugadas
            subjugadas[numSubjugadas++] = nuevaJugada;
            eliminarFichasDeJugada(jugada, copiaSubjugada);
        } else {
            // Si no cumple ni con el caso 1 ni con el caso 2, liberamos la memoria asignada para la nueva jugada
            liberarJugada(nuevaJugada);
        }
    }

    return subjugadas;
}




// Funciones //

void eliminarFichasDeJugada(struct Jugada *jugada_original, struct Jugada *subjugada) {
    if (jugada_original == NULL || subjugada == NULL) {
        return; // Manejo de error: no se pueden eliminar fichas de una jugada nula
    }

    struct Nodo *current_subjugada = subjugada->cabeza;

    while (current_subjugada != NULL) {
        struct Nodo *aux = current_subjugada;
        current_subjugada = current_subjugada->siguiente;
        
        // Buscar y eliminar la ficha correspondiente en la jugada original
        struct Nodo *current_jugada = jugada_original->cabeza;
        struct Nodo *prev_jugada = NULL;

        while (current_jugada != NULL && current_jugada->ficha.numero != aux->ficha.numero) {
            prev_jugada = current_jugada;
            current_jugada = current_jugada->siguiente;
        }

        // Si se encuentra la ficha en la jugada original, eliminarla
        if (current_jugada != NULL) {
            if (prev_jugada == NULL) {
                jugada_original->cabeza = current_jugada->siguiente;
            } else {
                prev_jugada->siguiente = current_jugada->siguiente;
            }

            free(current_jugada);
            jugada_original->tamanio--;
        }
    }
}
//Funciones 
struct Jugada *copiarJugada(const struct Jugada *jugada_original) {
    if (jugada_original == NULL) {
        return NULL;
    }

    // Crear una nueva estructura de Jugada para la copia
    struct Jugada *copia = (struct Jugada *)malloc(sizeof(struct Jugada));
    if (copia == NULL) {
        return NULL; // Manejo de error: no se pudo asignar memoria
    }

    copia->cabeza = NULL;
    copia->tamanio = 0;

    // Recorrer la jugada original y copiar cada nodo
    struct Nodo *current_original = jugada_original->cabeza;
    struct Nodo *prev_copia = NULL;
    while (current_original != NULL) {
        struct Nodo *nuevoNodo = (struct Nodo *)malloc(sizeof(struct Nodo));
        if (nuevoNodo == NULL) {
            // Manejo de error: no se pudo asignar memoria
            liberarJugada(copia); // Liberar la memoria asignada hasta ahora
            return NULL;
        }

        nuevoNodo->ficha = current_original->ficha;
        nuevoNodo->siguiente = NULL;

        if (copia->cabeza == NULL) {
            copia->cabeza = nuevoNodo;
        } else {
            prev_copia->siguiente = nuevoNodo;
        }

        prev_copia = nuevoNodo;
        copia->tamanio++;

        current_original = current_original->siguiente;
    }

    return copia;
}

// Funciones 

void liberarJugada(struct Jugada *jugada) {
    if (jugada == NULL) {
        return;
    }

    struct Nodo *current = jugada->cabeza;
    struct Nodo *next;

    // Liberar cada nodo en la jugada
    while (current != NULL) {
        next = current->siguiente;
        free(current);
        current = next;
    }

    // Liberar la estructura de la jugada
    free(jugada);
}

//Funciones Casos 

#include <stdbool.h>
#include <string.h>

// Función para verificar si una jugada cumple con el caso 1
bool cumpleCaso1(struct Jugada *jugada) {
    // Contadores para las fichas consecutivas y blancas
    int consecutivas = 0;
    int blancas = 0;
    char* color = NULL; // Variable para almacenar el color de las fichas no blancas

    // Puntero al nodo actual
    struct Nodo *current = jugada->cabeza;

    // Recorrer la jugada para contar las fichas consecutivas y blancas
    do {
        if (current->ficha.color == "Blanco") {
            blancas++;
        } else {
            // Verificar si el color de la ficha no blanca es igual al color anterior
            if (color != NULL && strcmp(current->ficha.color, color) != 0) {
                return false;
            }
            color = current->ficha.color;

            // Si hay una ficha no blanca después de una o dos blancas, se verifica si cumple con las condiciones
            if (blancas == 1) {
                // La ficha después de una blanca debe ser el número + 2 de la anterior a la blanca
                if (current->siguiente->ficha.numero != current->ficha.numero + 2) {
                    return false;
                }
            } else if (blancas == 2) {
                // La ficha después de la segunda blanca debe ser el número + 3 de la anterior a la primera blanca
                if (current->siguiente->ficha.numero != current->ficha.numero + 3) {
                    return false;
                }
            }

            // Reiniciar contador de blancas
            blancas = 0;
            consecutivas++;
        }
        current = current->siguiente;
    } while (current != jugada->cabeza);

    // Se cumplen las condiciones si hay al menos 3 fichas consecutivas y no más de 13
    return consecutivas >= 3 && consecutivas <= 13;
}

//Funciones Caso 2

// Función para verificar si una jugada cumple con el caso 2
bool cumpleCaso2(struct Jugada *jugada) {
    // Contadores para las fichas no blancas y fichas únicas
    int noBlancas = 0;
    int numerosUnicos = 0;
    int numero = -1;
    char* color = NULL;

    // Contador para el tamaño total de la jugada
    int tamanoTotal = 0;

    // Puntero al nodo actual
    struct Nodo *current = jugada->cabeza;

    // Recorrer la jugada para contar las fichas no blancas y verificar los números y colores
    do {
        if (current->ficha.color != "Blanco") {
            noBlancas++;
            if (numero == -1) {
                numero = current->ficha.numero;
                color = current->ficha.color;
            } else {
                // Verificar si el número es el mismo y el color es diferente
                if (current->ficha.numero != numero || strcmp(current->ficha.color, color) == 0) {
                    return false;
                }
            }
        }
        current = current->siguiente;
        // Incrementar el tamaño total de la jugada
        tamanoTotal++;
    } while (current != jugada->cabeza);

    // Se cumplen las condiciones si hay al menos 2 fichas no blancas y todas tienen el mismo número y diferentes colores,
    // y el tamaño total de la jugada es de 3 o 4 fichas
    return noBlancas >= 2 && noBlancas <= 4 && tamanoTotal >= 3 && tamanoTotal <= 4;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Structs.h"
#include "Functions.h"

int main() {
    int random;
    colorReset();
    int jugadoresActuales, totalJugadores = 0, jugadoresConsola = -1, bots = 0, opcion = 0;

    // Pedir al usuario el número total de jugadores
    while (totalJugadores < 2 || totalJugadores > 4) {
        printf("Ingrese el numero total de jugadores (entre 2 y 4): ");
        scanf("%d", &totalJugadores);
    }
    jugadoresActuales = totalJugadores;

    // Pedir al usuario el número de jugadores en consola
    while (jugadoresConsola < 0 || jugadoresConsola > totalJugadores) {
        printf("Ingrese el numero de jugadores en consola (entre 0 y %d): ", totalJugadores);
        scanf("%d", &jugadoresConsola);
    }

    // Calcular el número de bots
    bots = totalJugadores - jugadoresConsola;

    // Crear las estructuras del juego
    struct Fichas Baraja[4][26];
    struct Fichas Comodin[2];
    struct ColaJugadores cola, colaResultados;
    struct Pila pila;
    struct Tablero tablero;

    // Crear la baraja y los comodines
    createInitialDeck(Baraja);
    createJokers(Comodin);

    int jugadorActivo = 0;

    // Inicializar la cola de jugadores
    inicializarCola(&cola);
    inicializarCola(&colaResultados);
    inicializarPila(&pila);
    inicializarTablero(&tablero);

    // Insertar jugadores en la cola
    for (int i = 0; i < jugadoresConsola; i++) {
        char nombre[50];
        printf("Ingrese el nombre del jugador %d: ", i + 1);
        scanf("%49s", nombre);
        insertarJugador(&cola, nombre, false);
    }
    for (int i = 0; i < bots; i++) {
        char bot[50];
        sprintf(bot, "Bot %d", i + 1);
        insertarJugador(&cola, bot, true);
    }

    ClearPlayerTurn();
    printf("Orden de los jugadores\n");
    mezclarJugadores(&cola, totalJugadores);
    PCTurn(5);

    // Repartir cartas a cada jugador y llenar la pila con el resto
    repartirCartasYPila(&cola, Baraja, Comodin, totalJugadores, &pila);

    // Imprimir la pila
    while (cola.frente != NULL) {
        if (jugadoresConsola == 0)
            PCTurn(1);
        else
            ClearPlayerTurn();

        random = randomNumber();
        colorReset();

        // Imprimir las manos de los jugadores
        imprimirManos(&cola, jugadoresActuales);
        colorReset();

        if (pila.top != 0)
            printf("POZO: %d\n", pila.top);
        else
            printf("POZO VACIO!\n");

        colorReset();
        printf("Mesa:\n");
        imprimirTablero(&tablero);
        colorReset();

        if (cola.frente->esBot == 0) {
            cola.frente->jugadaRealizada = 0;
            opcion = 0;

            // Lógica del usuario
            if (cola.frente->jugadorActivo == 0) {
                jugadorActivo = 0;
                // Primera tirada
                while (opcion != 3) {
                    printf("\n1....Iniciar jugada\n");
                    printf("2...Ordenar fichas\n");
                    printf("3...Comer y pasar\n");
                    scanf("%d", &opcion);
                    switch (opcion) {
                        case 1:
                            ClearPlayerTurn();
                            jugadaInicial(&tablero, &cola, &pila);
                            colorReset();
                            opcion = 3;
                            break;
                        case 2:
                            ordenarMano(cola.frente->mano, cola.frente->numCartas);
                            printf("Mano ordenada\n");
                            imprimirManoActual(cola.frente);
                            colorReset();
                            break;
                        case 3:
                            comer(&cola, &pila);
                            break;
                        default:
                            printf("Opción incorrecta, inténtalo de nuevo\n");
                            PCTurn(2);
                            break;
                    }
                }
            } else {
                // Segunda tirada, todas las funciones desbloqueadas
                while (opcion != 4) {
                    printf("1....Iniciar jugada\n");
                    printf("2...Ordenar fichas\n");
                    printf("3...Modificar jugada existente\n");
                    if (cola.frente->jugadaRealizada == 0)
                        printf("4...Comer y pasar\n");
                    else
                        printf("4...Terminar turno\n");
                    scanf("%d", &opcion);
                    switch (opcion) {
                        case 1:
                            ClearPlayerTurn();
                            jugadaNormal(&tablero, &cola);
                            colorReset();
                            opcion = 3;
                            break;
                        case 2:
                            ordenarMano(cola.frente->mano, cola.frente->numCartas);
                            break;
                        case 3:
                            printf("Selecciona el tipo de jugada a realizar:\n");
                            printf("1.....Agregar ficha a jugada existente\n");
                            printf("2.....Robar ficha de jugada existente\n");
                            printf("3.....Romper jugada existente\n"); // Nueva opción para romper jugada

                            scanf("%d", &opcion);
                            if (opcion == 1) {
                                agregarFichaAJugadaExistente(&tablero, &cola);
                            } else if (opcion == 2) {
                                robarFichaAJugadaExistente(&tablero, &cola);
                            } else if (opcion == 3) {
                                struct Jugada *jugadaSeleccionada = seleccionarJugada(&tablero); // Función para seleccionar una jugada
                                struct Jugada *jugadas[10];
                                int numJugadas;

                                if (romperJugada(jugadaSeleccionada, jugadas, &numJugadas)) {
                                    printf("La jugada ha sido rota en %d jugadas:\n", numJugadas);
                                    for (int i = 0; i < numJugadas; i++) {
                                        printf("Jugada %d: ", i + 1);
                                        struct Nodo *temp = jugadas[i]->cabeza;
                                        do {
                                            printf("%d %s ", temp->ficha.numero, temp->ficha.color);
                                            temp = temp->siguiente;
                                        } while (temp != jugadas[i]->cabeza);
                                        printf("\n");
                                    }
                                } else {
                                    printf("No se pudo romper la jugada.\n");
                                }
                            } else {
                                printf("Opción inválida, inténtalo de nuevo\n");
                            }
                            break;
                        case 4:
                            if (cola.frente->jugadaRealizada == 0)
                                comer(&cola, &pila);
                            else
                                finTurno(&cola);
                            break;
                        default:
                            printf("Opción inválida, inténtalo de nuevo\n");
                            PCTurn(2);
                            break;
                    }
                }
                finTurno(&cola);
            }
        } else {
            jugadaBot(&tablero, &cola, &pila);
        }

        revisarSalida(&cola, &colaResultados, &jugadoresActuales);
        opcion = 0;
    }

    Leaderboard(&colaResultados, totalJugadores);

    return 0;
}


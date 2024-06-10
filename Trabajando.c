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

        // Llamar a la función agregarFicha para agregar una ficha a la nueva jugada
       int opcion;
 
        // Solicita al usuario que ingrese una opción
       printf("Desea agregar una ficha a la subjugada, Ingrese el numero de la opcion que desea\n");
       printf("1. Si\n");
       printf("2. No\n");

        // Lee la opción ingresada por el usuario
       scanf("%d", &opcion);

       // Verifica la opción ingresada
       if (opcion == 1) {
       // Si la opción es 1, imprime "trabajando" cambiar a llamar funcion agregar ficha 
        printf("trabajando\n");
         }

      // Continúa con la siguiente parte del código
         printf("Continuando con la siguiente parte del código...\n");
        
        // Revisar 

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

    // Verificar si la jugada original está vacía
    if (jugada->cabeza != NULL) {
        printf("Jugada inválida.\n");
        // Liberar memoria asignada previamente
        for (int i = 0; i < numSubjugadas; i++) {
            liberarJugada(subjugadas[i]);
        }
        free(subjugadas);
        return NULL;
    }

    return subjugadas;
}

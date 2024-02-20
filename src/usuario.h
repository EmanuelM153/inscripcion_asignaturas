#ifndef INCLUDE_USUARIO
#define INCLUDE_USUARIO

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "utils.h"
#include "format.h"

typedef struct {
        double id;
        int semestre;
        char *nombre;
        bool pago;
        narray_t *carreras;
        nmatrix_t *asignaturas;
} usuario_t;

usuario_t *leer_usuario(char *);
void liberar_usuario(usuario_t **);
char *escribir_usuario(usuario_t *);

#endif


#ifndef INCLUDE_CARRERA
#define INCLUDE_CARRERA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "utils.h"
#include "format.h"

typedef struct {
        double id;
        char *nombre;
        nmatrix_t *asignaturas;
        narray_t *creditos;
} carrera_t;

carrera_t *leer_carrera(char *);
void liberar_carrera(carrera_t **);

#endif


#ifndef INCLUDE_ASIGNATURA
#define INCLUDE_ASIGNATURA

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "utils.h"
#include "format.h"

typedef struct {
        double id;
        int cupos;
        int creditos;
        char *nombre;
        narray_t *horarios;
        narray_t *prerequisitos;
} asignatura_t;

asignatura_t *leer_asignatura(char *);
void liberar_asignatura(asignatura_t **);
char *escribir_asignatura(asignatura_t *);
#endif




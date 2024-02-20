#ifndef INCLUDE_UTILS
#define INCLUDE_UTILS

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>
#include "format.h"

typedef struct {
        int nelem;
        void *array;
} narray_t;

typedef struct {
        int nelem;
        narray_t **array;
} nmatrix_t;

narray_t *leer_delarray(char *, const char *);
void liberar_delarray(narray_t *);
void agregar_delelemento(narray_t *, char *);
void eliminar_delelemento(narray_t *, int);
void liberar_delmatrix(nmatrix_t *);
narray_t *inicializar_narray(int, size_t);
nmatrix_t *inicializar_nmatrix(int);
nmatrix_t *leer_delmatrix(char *, const char *, const char *);
void modificar_narray(narray_t *, size_t, int);
void agregar_nmatelemento(nmatrix_t *, int);
void eliminar_nmatelemento(nmatrix_t *, int);
void liberar_narray(narray_t *);
void liberar_nmatrix(nmatrix_t *);
narray_t *busqueda_id(narray_t, const char *, bool);
void escritura_id(double, const char *, const char *);
int busqueda_numnarray(narray_t, narray_t);

#endif

#include "carrera.h"

carrera_t *leer_carrera(char *line)
{
        char *workline = malloc(strlen(line) + 1);
        strcpy(workline, line);

        carrera_t *c = malloc(sizeof(carrera_t));

        narray_t *narr = leer_delarray(workline, VAL_DELIMITER);

        char **arr = narr->array;

        // obtener datos
        c->id = atof(*arr);

        c->nombre = (char *) malloc(strlen(*(arr + 1)) + 1);
        strcpy(c->nombre, *(arr + 1));

        // obtener arreglos
        double *arr_num;
        char **arr_arreglos;

        // obtener asignaturas
        narray_t *narr_arreglo;
        nmatrix_t *nmat_asignatura = leer_delmatrix(*(arr + 2), ARR_DELIMITER, MAT_DELIMITER);

        c->asignaturas = inicializar_nmatrix(nmat_asignatura->nelem);

        for (int i = 0; i < nmat_asignatura->nelem; i++) {
                narr_arreglo = *(nmat_asignatura-> array + i);

                *(c->asignaturas->array + i) = inicializar_narray(narr_arreglo->nelem, sizeof(double));

                arr_num = (*(c->asignaturas->array +i))->array;
                arr_arreglos = narr_arreglo->array;

                for (int j = 0; j < narr_arreglo->nelem; j++)
                        *(arr_num + j) = atof(*(arr_arreglos + j));
        }

        liberar_delmatrix(nmat_asignatura);

        // obtener creditos
        int *arr_num_int;
        narray_t *narr_creditos = leer_delarray(*(arr + 3), ARR_DELIMITER);

        c->creditos = inicializar_narray(narr_creditos->nelem, sizeof(int));

        arr_num_int = c->creditos->array;
        arr_arreglos = narr_creditos->array;

        for (int i = 0; i < narr_creditos->nelem; i++)
                *(arr_num_int + i) = atoi(*(arr_arreglos + i));

        liberar_delarray(narr_creditos);

        liberar_delarray(narr);

        free(workline);

        return c;
}

void liberar_carrera(carrera_t **c)
{
        liberar_nmatrix((*c)->asignaturas);
        liberar_narray((*c)->creditos);
        free((*c)->nombre);
        free(*c);
        *c = NULL;
}

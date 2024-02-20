#include "usuario.h"

usuario_t *leer_usuario(char *line)
{
        char *workline = malloc(strlen(line) + 1);
        strcpy(workline, line);

        usuario_t *u = malloc(sizeof(usuario_t));

        narray_t *narr = leer_delarray(workline, VAL_DELIMITER);

        char **arr = narr->array;

        // obtener datos
        u->id = atof(*arr);

        u->semestre = atoi(*(arr + 1));

        u->nombre = malloc(strlen(*(arr + 2)) + 1);
        strcpy(u->nombre, *(arr + 2));

        u->pago = atoi(*(arr + 3));

        // obtener arreglos
        double *arr_num;
        char **arr_arreglos;

        // obtener carreras
        narray_t *narr_carrera = leer_delarray(*(arr + 4), ARR_DELIMITER);

        u->carreras = inicializar_narray(narr_carrera->nelem, sizeof(double));

        arr_num = u->carreras->array;
        arr_arreglos = narr_carrera->array;

        for (int i = 0; i < narr_carrera->nelem; i++)
                *(arr_num + i) = atof(*(arr_arreglos + i));

        liberar_delarray(narr_carrera);

        // obtener asignaturas
        narray_t *narr_arreglo;
        nmatrix_t *nmat_asignatura = leer_delmatrix(*(arr + 5), ARR_DELIMITER, MAT_DELIMITER);

        u->asignaturas = inicializar_nmatrix(nmat_asignatura->nelem);

        for (int i = 0; i < nmat_asignatura->nelem; i++) {
                narr_arreglo = *(nmat_asignatura->array + i);

                *(u->asignaturas->array + i) = inicializar_narray(narr_arreglo->nelem, sizeof(double));

                arr_num = (*(u->asignaturas->array + i))->array;
                arr_arreglos = narr_arreglo->array;

                for (int j = 0; j < narr_arreglo->nelem; j++)
                        *(arr_num + j) = atof(*(arr_arreglos + j));
        }

        liberar_delmatrix(nmat_asignatura);

        liberar_delarray(narr);

        free(workline);

        return u;
}


void liberar_usuario(usuario_t **u)
{
        liberar_nmatrix((*u)->asignaturas);
        liberar_narray((*u)->carreras);
        free((*u)->nombre);
        free(*u);
        *u = NULL;
}

char *escribir_usuario(usuario_t *u)
{
        narray_t *narr = malloc(sizeof(narray_t));
        char *str;
        narr->nelem = 0;
        size_t buf_size = sizeof(double);
        narr->array = NULL;

        // para escribir numeros
        char format[6];
        char buff[buf_size];

        // escribir id
        strcpy(format, "%.0lf");
        strcat(format, VAL_DELIMITER);
        snprintf(buff, buf_size, format, u->id);
        modificar_narray(narr, sizeof(char), strlen(buff) + 1);
        strcpy(narr->array, buff);

        // escribir semestre
        strcpy(format, "%d");
        strcat(format, VAL_DELIMITER);
        snprintf(buff, buf_size, format, u->semestre);
        modificar_narray(narr, sizeof(char), strlen(buff) + 1 + narr->nelem);
        strcat(narr->array, buff);

        // escribir nombre
        modificar_narray(narr, sizeof(char), strlen(u->nombre) + 2 + narr->nelem);
        strcat(narr->array, u->nombre);
        strcat(narr->array, VAL_DELIMITER);

        // escribir pago
        snprintf(buff, buf_size, format, u->pago);
        modificar_narray(narr, sizeof(char), strlen(buff) + 1 + narr->nelem);
        strcat(narr->array, buff);

        // escribir carreras
        for (int i = 0; i < u->carreras->nelem; i++) {
                snprintf(buff, buf_size, "%.0lf", *((double *) u->carreras->array + i));
                modificar_narray(narr, sizeof(char), strlen(buff) + 2 + narr->nelem);
                strcat(narr->array, buff);

                if (i != u->carreras->nelem - 1)
                        strcat(narr->array, ARR_DELIMITER);
        }

        // escribir asignaturas
        double aux;
        for (int i = 0; i < u->asignaturas->nelem; i++) {
                for (int j = 0; j < (*(u->asignaturas->array + i))->nelem; j++) {
                        // arreglos
                        aux = *((double *) (*(u->asignaturas->array + i))->array + j);
                        snprintf(buff, buf_size, "%.1lf", aux);
                        modificar_narray(narr, sizeof(char), strlen(buff) + 2 + narr->nelem);
                        strcat(narr->array, buff);

                        if (j != (*(u->asignaturas->array + i))->nelem - 1)
                                strcat(narr->array, ARR_DELIMITER);
                }
                // matriz
                if (i != u->asignaturas->nelem - 1) {
                        modificar_narray(narr, sizeof(char), 1 + narr->nelem);
                        strcat(narr->array, MAT_DELIMITER);
                }
        }

        str = narr->array;
        free(narr);
        return str;
}

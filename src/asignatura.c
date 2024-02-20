#include "asignatura.h"

asignatura_t *leer_asignatura(char *line)
{
        char *workline = malloc(strlen(line) + 1);
        strcpy(workline, line);

        asignatura_t *a = malloc(sizeof(asignatura_t));

        narray_t *narr = leer_delarray(workline, VAL_DELIMITER);

        char **arr = narr->array;

        // obtener datos
        a->id = atof(*arr);

        a->cupos = atoi(*(arr + 1));

        a->creditos = atoi(*(arr + 2));

        a->nombre = (char *) malloc(strlen(*(arr + 3)) + 1);
        strcpy(a->nombre, *(arr + 3));

        // obtener arreglos
        double *arr_num;
        char **arr_arreglos;

        // obtener horarios
        struct tm *arr_horario;
        narray_t *narr_horario = leer_delarray(*(arr + 4), ARR_DELIMITER);

        a->horarios = inicializar_narray(narr_horario->nelem, sizeof(struct tm));
        memset(a->horarios->array, 0, sizeof(struct tm) * narr_horario->nelem);

        arr_horario = a->horarios->array;
        arr_arreglos = narr_horario->array;

        for (int i = 0; i < narr_horario->nelem; i++)
                strptime(*(arr_arreglos + i), "%a/%H", arr_horario + i);

        liberar_delarray(narr_horario);

        // obtener prerequisitos
        narray_t *narr_prereq = leer_delarray(*(arr + 5), ARR_DELIMITER);

        a->prerequisitos = inicializar_narray(narr_prereq->nelem, sizeof(double));

        arr_num = a->prerequisitos->array;
        arr_arreglos = narr_prereq->array;

        for (int i = 0; i < narr_prereq->nelem; i++)
                *(arr_num + i) = atof(*(arr_arreglos + i));

        liberar_delarray(narr_prereq);

        liberar_delarray(narr);

        free(workline);

        return a;
}

void liberar_asignatura(asignatura_t **a)
{
        free((*a)->nombre);
        liberar_narray((*a)->prerequisitos);
        liberar_narray((*a)->horarios);
        free(*a);
        *a = NULL;
}

char *escribir_asignatura(asignatura_t *a)
{
        narray_t *narr = malloc(sizeof(narray_t));
        char *str;
        narr->nelem = 0;
        size_t buf_size = sizeof(struct tm) / sizeof(char) + 1;
        narr->array = NULL;

        // para escribir numeros
        char format[6];
        char buff[buf_size];

        // escribir id
        strcpy(format, "%.1lf");
        strcat(format, VAL_DELIMITER);
        snprintf(buff, buf_size, format, a->id);
        modificar_narray(narr, sizeof(char), strlen(buff) + 1);
        strcpy(narr->array, buff);

        // escribir cupos
        strcpy(format, "%d");
        strcat(format, VAL_DELIMITER);
        snprintf(buff, buf_size, format, a->cupos);
        modificar_narray(narr, sizeof(char), strlen(buff) + 1 + narr->nelem);
        strcat(narr->array, buff);

        // escribir creditos
        snprintf(buff, buf_size, format, a->creditos);
        modificar_narray(narr, sizeof(char), strlen(buff) + 1 + narr->nelem);
        strcat(narr->array, buff);

        // escribir nombre
        modificar_narray(narr, sizeof(char), strlen(a->nombre) + 2 + narr->nelem);
        strcat(narr->array, a->nombre);
        strcat(narr->array, VAL_DELIMITER);

        // escribir horarios
        struct tm *arr_horario = a->horarios->array;
        for (int i = 0; i < a->horarios->nelem; i++) {
                strftime(buff, buf_size, "%a/%H", arr_horario + i);
                modificar_narray(narr, sizeof(char), strlen(buff) + 2 + narr->nelem);
                strcat(narr->array, buff);

                if (i != a->horarios->nelem - 1)
                        strcat(narr->array, ARR_DELIMITER);
                else
                        strcat(narr->array, VAL_DELIMITER);
        }

        // escribir prerequisitos
        for (int i = 0; i < a->prerequisitos->nelem; i++) {
                snprintf(buff, buf_size, "%.0lf", *((double *) a->prerequisitos->array + i));
                modificar_narray(narr, sizeof(char), strlen(buff) + 2 + narr->nelem);
                strcat(narr->array, buff);

                if (i != a->prerequisitos->nelem - 1)
                        strcat(narr->array, ARR_DELIMITER);
        }

        str = narr->array;
        free(narr);
        return str;
}

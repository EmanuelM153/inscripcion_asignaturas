#include "utils.h"

nmatrix_t *leer_delmatrix(char *array_txt, const char *arrdel, const char *matdel)
{
        narray_t *filas = leer_delarray(array_txt, matdel);

        nmatrix_t *nmat = inicializar_nmatrix(filas->nelem);

        char **arr_filas = filas->array;
        char **arr_cols;
        char **arr;
        narray_t *col;
        for (int i = 0; i < filas->nelem; i++) {
                col = leer_delarray(*(arr_filas + i), arrdel);

                *(nmat->array + i) = inicializar_narray(col->nelem, sizeof(char *));

                arr = (*(nmat->array + i))->array;
                arr_cols = col->array;
                for (int j = 0; j < col->nelem; j++) {
                        *(arr + j) = malloc(strlen(*(arr_cols + j)) + 1);
                        strcpy(*(arr + j), *(arr_cols + j));
                }

                liberar_delarray(col);
        }

        liberar_delarray(filas);

        return nmat;
}

narray_t *leer_delarray(char *array_txt, const char *delim)
{
        char *subtoken;
        char *subsave_token = NULL;

        narray_t *narr = malloc(sizeof(narray_t));

        char **arr = narr->array;
        arr = NULL;

        narr->nelem = 0;

        subtoken = strtok_r(array_txt, delim, &subsave_token);
        do {
                arr = realloc(arr, sizeof(char *) * (narr->nelem + 1));

                // guardar el valor
                *(arr + narr->nelem) = malloc(strlen(subtoken) + 1);
                strcpy(*(arr + narr->nelem), subtoken);

                narr->nelem++;
        } while (subtoken = strtok_r(NULL, delim, &subsave_token));

        narr->array = arr;

        return narr;
}


void liberar_delarray(narray_t *narr)
{
        char **arr = narr->array;
        for (int i = 0; i < narr->nelem; i++)
                free(*(arr + i));

        liberar_narray(narr);
}

void agregar_delelemento(narray_t *narr, char *src)
{
        modificar_narray(narr, sizeof(char *), narr->nelem + 1);
        char **dst = &*((char **) narr->array + narr->nelem - 1);

        *dst = malloc(strlen(src) + 1);
        strcpy(*dst, src);
}

void eliminar_delelemento(narray_t *narr, int i)
{
        char **del = &*((char **) narr->array + i);

        free(*del);

        if (i != narr->nelem - 1) {
                char *last = *((char **) narr->array + narr->nelem - 1);

                *del = malloc(strlen(last) + 1);
                strcpy(*del, last);
                free(last);
        }

        modificar_narray(narr, sizeof(char *), narr->nelem - 1);
}

void liberar_delmatrix(nmatrix_t *nmat)
{
        for (int i = 0; i < nmat->nelem; i++)
                liberar_delarray(*(nmat->array + i));

        free(nmat->array);
        free(nmat);
}

narray_t *inicializar_narray(int nelem, size_t size)
{
        narray_t *narr = malloc(sizeof(narray_t));
        narr->array = malloc(size * nelem);
        narr->nelem = nelem;
}

nmatrix_t *inicializar_nmatrix(int nelem)
{
        narray_t *nmat = malloc(sizeof(nmatrix_t));
        nmat->array = malloc(sizeof(narray_t *) * nelem);
        nmat->nelem = nelem;
}

void modificar_narray(narray_t *narr, size_t size, int nelem)
{
        narr->array = realloc(narr->array, size * nelem);
        narr->nelem = nelem;
}

void agregar_nmatelemento(nmatrix_t *mat, int nelem)
{
        mat->array = realloc(mat->array, sizeof(narray_t *) * (mat->nelem + nelem));

        for (int i = mat->nelem; i < mat->nelem + nelem; i++)
                *(mat->array + i) = NULL;

        mat->nelem += nelem;
}

void eliminar_nmatelemento(nmatrix_t *mat, int i)
{
        narray_t **del = &*(mat->array + i);

        liberar_narray(*del);

        if (i != mat->nelem - 1) {
                narray_t *last = *(mat->array + mat->nelem - 1);

                *del = malloc(sizeof(narray_t *));
                memcpy(*del, last, sizeof(narray_t *));
                liberar_narray(last);
        }

        mat->array = realloc(mat->array, sizeof(narray_t *) * (mat->nelem - 1));
        mat->nelem = mat->nelem - 1;
}

void liberar_narray(narray_t *narr)
{
        free(narr->array);
        free(narr);
}

void liberar_nmatrix(nmatrix_t *nmat)
{
        for (int i = 0; i < nmat->nelem; i++)
                liberar_narray(*(nmat->array + i));

        free(nmat->array);
        free(nmat);
}

int busqueda_numnarray(narray_t arr1, narray_t arr2)
{
        int coincidencias = 0;
        double aux;

        double *num_arr1 = arr1.array;
        double *num_arr2 = arr2.array;

        if (arr1.nelem < arr2.nelem) {
                aux = arr1.nelem;
                arr1.nelem = arr2.nelem;
                arr2.nelem = aux;

                num_arr1 = arr2.array;
                num_arr2 = arr1.array;
        }

        for (int i = 0; i < arr1.nelem; i++) {
                for (int j = 0; j < arr2.nelem; j++) {
                        double mant_numarr1;
                        double mant_numarr2;

                        modf(*(num_arr1 + i), &mant_numarr1);
                        modf(*(num_arr2 + j), &mant_numarr2);
                        if (mant_numarr1 == mant_numarr2) {

                                // mover el elemento al inicio
                                aux = *num_arr1;
                                *(num_arr1) = *(num_arr1 + i);
                                *(num_arr1 + i) = aux;
                                arr1.nelem--;

                                num_arr1 += 1;
                                coincidencias++;
                                break;
                        }
                }
        }

        return coincidencias;
}

narray_t *busqueda_id(narray_t ids, const char *archivo_nombre, bool repeat)
{
        FILE *archivo = fopen(archivo_nombre, "r");

        if (NULL == archivo) {
                perror("fopen");
                exit(-1);
        }

        narray_t *narr = malloc(sizeof(narray_t));

        double *num_arr = ids.array;
        char **arr = NULL;

        narr->nelem = 0;

        size_t len = 0;
        char *line = NULL;

        char *token;
        char *save_token;

        char *del;
        double aux;

        if (0 == ids.nelem)
                return narr;

        double aux_int;
        double num_arr_int;

        while (-1 != getline(&line, &len, archivo)) {
                del = strstr(line, VAL_DELIMITER);

                save_token = NULL;
                token = strtok_r(line, VAL_DELIMITER, &save_token);

                aux = atof(token);
                for (int i = 0; i < ids.nelem; i++) {
                        modf(aux, &aux_int);
                        modf(*(num_arr + i), &num_arr_int);
                        if (aux_int == num_arr_int) {
                                *del = *VAL_DELIMITER;

                                arr = realloc(arr, sizeof(char *) * (narr->nelem + 1));

                                *(arr + narr->nelem) = malloc(strlen(line) + 1);
                                strcpy(*(arr + narr->nelem), line);

                                narr->nelem++;

                                if (!repeat) {
                                        aux = *(num_arr);
                                        *(num_arr) = *(num_arr + i);
                                        *(num_arr + i) = aux;
                                        ids.nelem--;
                                        num_arr += 1;
                                        break;
                                }
                        }
                }

                free(line);
                line = NULL;
        }
        free(line);
        fclose(archivo);

        narr->array = arr;

        return narr;
}

void escritura_id(double id, const char *line, const char *archivo_nombre)
{
        FILE *flectura = fopen(archivo_nombre, "r");
        FILE *fescritura = fopen(archivo_nombre, "r+");

        if (NULL == flectura || NULL == fescritura) {
                perror("fopen");
                exit(-1);
        }

        double aux;

        size_t len = 0;
        char *fline = NULL;

        char *token;
        char *save_token;

        while (-1 != getline(&fline, &len, flectura)) {
                save_token = NULL;
                token = strtok_r(fline, VAL_DELIMITER, &save_token);

                aux = atof(token);

                if (aux == id) {
                        fputs(line, fescritura);
                        fputc('\n', fescritura);
                        break;
                }

                free(fline);
                fline = NULL;
                getline(&fline, &len, fescritura);
                free(fline);
                fline = NULL;
        }

        free(fline);

        fclose(flectura);
        fclose(fescritura);
}

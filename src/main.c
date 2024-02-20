#include "main.h"

char *archivos[NUM_ARCHIVOS];

int main (int argc, char **argv)
{
        if (NUM_ARCHIVOS + 1 != argc) {
                printf("%s uso: %s [base-usuarios] [base-carreras] [base-asignaturas]\n", *argv, *argv);
                return -1;
        }

        // guardar el nombre de los archivos
        for (size_t i = 0; i < NUM_ARCHIVOS; i++)
                *(archivos + i) = *(argv + i + 1);

        carrera_t **carreras;
        narray_t id = { .nelem = 1, .array = alloca(sizeof(double)) };

        int opcion;

        narray_t *line;
        do {
                mostrar_menu1();
                printf("\n");

                obtener_numero("", &opcion);

                switch (opcion) {
                case 1:
                        obtener_double("Ingrese su id\n", id.array);

                        line = busqueda_id(id, *(archivos + USUARIOS), 0);

                        if (0 == line->nelem) {
                                printf("Usuario no encontrado\n");
                                liberar_delarray(line);
                                break;
                        }

                        usuario_t *u = leer_usuario(*(char **) line->array);
                        liberar_delarray(line);

                        printf("Bienvenido %s\n", u->nombre);

                        // revisar pago
                        if (!u->pago) {
                                printf("No se ha registrado un pago de su parte\n");
                                goto liberaru;
                        }

                        // seleccion de la carrera
                        for (int i = 0; i < u->carreras->nelem; i++) {
                                carreras = malloc(sizeof(carrera_t *) * u->carreras->nelem);

                                *(double *) id.array = *((double *) u->carreras->array + i);
                                line = busqueda_id(id, *(archivos + CARRERAS), 0);

                                *(carreras + i) = leer_carrera(*(char **) line->array);

                                printf("%d. %s\n", i + 1, (*(carreras + i))->nombre);

                                liberar_delarray(line);
                        }

                        obtener_double("Ingrese el número de la carrera que inscribira\n", id.array);

                        // verificar el id de la carrera
                        if (*(double *) id.array - 1 >= u->carreras->nelem || *(double *) id.array <= 0) {
                                printf("Número de carrera inválido\n");
                                goto fin;
                        }

                        // inscripcion
                        double integer;
                        modf(*(double *) id.array, &integer);
                        inscripcion(u, *(carreras + (int) (integer) - 1));

                        fin:
                        // liberar carreras no seleccionadas
                        for (int i = 0; i < u->carreras->nelem; i++)
                                        liberar_carrera(carreras + i);
                        free(carreras);

                        liberaru:
                        liberar_usuario(&u);
                        break;
                default:
                }

                printf("\n");
        } while(0 != opcion);

        return 0;
}

void mostrar_menu1()
{
        printf("0. Salir del sistema\n");
        printf("1. Ingresar al sistema\n");
}

void mostrar_menu2()
{
        printf("0. Terminar la inscripcion\n");
        printf("1. Seleccionar semestre\n");
        printf("2. Inscribir una asignatura\n");
        printf("3. Eliminar una asignatura\n");
}

void obtener_numero(char *mensaje, int *var)
{
        printf("%s", mensaje);

        printf("> ");
        scanf("%u", var);

        printf("\n");
}

void obtener_double(char *mensaje, double *var)
{
        printf("%s", mensaje);

        printf("> ");
        scanf("%lf", var);

        printf("\n");
}

void inscripcion(usuario_t *u, carrera_t *c)
{
        int opcion;
        int iaux;
        int semestre = u->semestre;
        double aux;
        asignatura_t *asig;
        asignatura_t *asig_aux;
        char *caux;
        narray_t *semestre_actual = *(u->asignaturas->array + u->semestre - 1);

        narray_t *uasigs = busqueda_id(*semestre_actual, *(archivos + ASIGNATURAS), 0);
        narray_t *sasigs = busqueda_id(**(c->asignaturas->array + u->semestre - 1), *(archivos + ASIGNATURAS), 1);

        int creditos;

        do {
                // calcular creditos
                creditos = *((int *) c->creditos->array + u->semestre - 1);

                // agregar el proximo semestre en caso de que no exista
                if (u->asignaturas->nelem != u->semestre)
                        goto seguir;

                // ya inscribio algo
                // obtener el numero de creditos
                for (int i = 0; i < uasigs->nelem; i++) {
                        asig = leer_asignatura(*((char **) uasigs->array + i));
                        creditos -= asig->creditos;

                        liberar_asignatura(&asig);
                }

                // mostrar asignaturas inscritas
                for (int i = 0; i < uasigs->nelem; i++) {
                        asig = leer_asignatura(*((char **) uasigs->array + i));

                        printf("Ha inscrito: %s con id %.1f\n", asig->nombre, asig->id);

                        liberar_asignatura(&asig);
                }

                printf("Puede inscribir %d creditos\n", creditos);

                seguir:

                mostrar_menu2();
                printf("\n");

                obtener_numero("", &opcion);

                switch (opcion) {
                case 1:
                        obtener_numero("Ingrese el numero del semestre\n", &semestre);

                        if (semestre > c->asignaturas->nelem || semestre <= 0) {
                                semestre = u->semestre;
                                printf("El semestre no existe\n");
                                break;
                        }

                        // evita la busqueda de nuevo
                        // mostrar las asignaturas
                        liberar_delarray(sasigs);
                        sasigs = busqueda_id(**(c->asignaturas->array + semestre - 1), *(archivos + ASIGNATURAS), 1);


                        for (int i = 0; i < sasigs->nelem; i++) {
                                asig = leer_asignatura(*((char **) sasigs->array + i));
                                printf("%.1f. %s : %u cupos, %u creditos\n", asig->id, asig->nombre, asig->cupos, asig->creditos);
                                liberar_asignatura(&asig);
                        }

                        break;
                case 2:
                        obtener_double("Ingrese el id de la asignatura\n", &aux);

                        // verificar que la asignatura exista y obtenerla
                        for (int i = 0; i < sasigs->nelem; i++) {
                                asig = leer_asignatura(*((char **) sasigs->array + i));

                                if (aux == asig->id) {
                                        iaux = i;
                                        break;
                                }

                                liberar_asignatura(&asig);
                        }

                        // no se encontro
                        if (asig == NULL) {
                                printf("Asignatura no encontrada\n");
                                break;
                        }

                        // verificar creditos disponibles
                        if (asig->creditos > creditos) {
                                printf("No tiene los suficientes creditos :((\n");
                                goto noinscribir;
                        }

                        // verificar cupos
                        if (0 == asig->cupos) {
                                printf("No hay cupos :(\n");
                                goto noinscribir;
                        }

                        if (u->asignaturas->nelem != u->semestre) {
                                agregar_nmatelemento(u->asignaturas, 1);
                                *(u->asignaturas->array + u->semestre - 1) = inicializar_narray(0,0);
                                semestre_actual = *(u->asignaturas->array + u->semestre - 1);
                        }

                        aux = 0;
                        // verificar que no se haya inscrito ya
                        narray_t id = {.nelem = 1, .array = alloca(sizeof(double))};
                        *(double *) id.array = asig->id;
                        for (int i = 0; i < u->semestre - 1; i++)
                                aux += busqueda_numnarray(**(u->asignaturas->array + i), id);

                        // ya la inscribio
                        if (aux == 1) {
                                printf("Ya inscribio la materia @!#?@!\n");
                                goto noinscribir;
                        }

                        aux = 0;
                        // verificar prerequisitos si hay
                        if (0 == *((int *) asig->prerequisitos->array))
                                goto inscripcion;

                        for (int i = 0; i < u->semestre; i++)
                                aux += busqueda_numnarray(**(u->asignaturas->array + i), *asig->prerequisitos);

                        if (aux != asig->prerequisitos->nelem) {
                                printf("No cumples con los prerequisitos :(((\n");
                                goto noinscribir;
                        }

                        inscripcion:

                        printf("Asignatura inscrita :)\n");
                        creditos -= asig->creditos;
                        asig->cupos--;

                        modificar_narray(semestre_actual, sizeof(double),
                                             semestre_actual->nelem + 1);

                        *((double *) semestre_actual->array + semestre_actual->nelem - 1) = asig->id;

                        // agregar a uasigs
                        agregar_delelemento(uasigs, *((char **) sasigs->array + iaux));

                        caux = escribir_asignatura(asig);

                        // actualizar sasigs
                        eliminar_delelemento(sasigs, iaux);
                        agregar_delelemento(sasigs, caux);

                        // guardar la asignatura inscrita
                        escritura_id(asig->id, caux, *(archivos + ASIGNATURAS));

                        free(caux);
                        noinscribir:
                        liberar_asignatura(&asig);
                        break;
                case 3:
                        obtener_double("Ingrese el id de la asignatura\n", &aux);

                        // verificar que haya inscrito la asignatura
                        for (int i = 0; i < uasigs->nelem; i++) {
                                asig = leer_asignatura(*((char **) uasigs->array + i));

                                if (aux == asig->id) {
                                        iaux = i;
                                        break;
                                }

                                liberar_asignatura(&asig);
                        }

                        if (asig == NULL) {
                                printf("No ha inscrito esa asignatura\n");
                                break;
                        }

                        creditos += asig->creditos;
                        asig->cupos++;

                        for (int i = 0; i < (semestre_actual)->nelem; i++) {
                                if (asig->id == *((double *)
                                                (semestre_actual->array + i))) {

                                        iaux = i;
                                        break;
                                }

                        }

                        // intercambiar posicion con el ultimo elemento
                        aux = *((double *) semestre_actual->array + semestre_actual->nelem - 1);
                        *((double *) semestre_actual->array + semestre_actual->nelem - 1) =
                                *((double *) semestre_actual->array + iaux);
                        *((double *) semestre_actual->array + iaux) = aux;

                        modificar_narray(semestre_actual, sizeof(double), semestre_actual->nelem - 1);

                        // elimina el ultimo arreglo de la matriz
                        if (semestre_actual->nelem == 0)
                                eliminar_nmatelemento(u->asignaturas, u->asignaturas->nelem - 1);


                        // eliminar de uasigs
                        eliminar_delelemento(uasigs, iaux);

                        printf("Asignatura eliminada\n");

                        caux = escribir_asignatura(asig);

                        // guardar la asignatura borrada
                        escritura_id(asig->id, caux, *(archivos + ASIGNATURAS));

                        // actualizar sasigs solo si la asignatura esta ahi
                        // encontrar la posicion dentro de sasigs
                        for (int i = 0; i < sasigs->nelem; i++) {
                                asig_aux = leer_asignatura(*((char **) sasigs->array + i));
                                if (aux == asig_aux->id) {
                                        eliminar_delelemento(sasigs, i);
                                        agregar_delelemento(sasigs, caux);
                                }

                                liberar_asignatura(&asig_aux);
                        }

                        free(caux);
                        liberar_asignatura(&asig);
                        break;
                default:
                }

                printf("\n");
        } while (0 != opcion);

        // guardar datos de usuario
        caux = escribir_usuario(u);
        escritura_id(u->id, caux, *(archivos + USUARIOS));
        free(caux);

        liberar_delarray(uasigs);
        liberar_delarray(sasigs);
}

#include <stdio.h>
#include <stddef.h>

#include "asignatura.h"
#include "usuario.h"
#include "carrera.h"

#define NUM_ARCHIVOS 3
#define USUARIOS 0
#define CARRERAS 1
#define ASIGNATURAS 2

void mostrar_menu1();
void mostrar_menu2();
void obtener_numero(char *, int *);
void obtener_double(char *, double *);
void inscripcion(usuario_t *, carrera_t *c);

#include "grade_school.h"

void init_roster(roster_t *roster){
    roster->count = 0;          //Inicializar número de estudiantes en 0
    roster->max_students = 0;   //Tamaño máximo igual a 0
    roster->students = NULL;    //Inicializa el puntero al arreglo de estudiantes a NULL
}

void free_roster(roster_t *roster){
    //Iterar a través de cada estudiante en el arreglo
    for(size_t i = 0; i < roster->count; i++){
        //Liberar la memoria asignada para el nombre de cada estudiante
        free((roster->students + i)->name);
    }
    //Liberar la memoria asignada para el arreglo de estudiantes
    free(roster->students);
    roster->count = 0;          //Restablecer el contador de estudiantes a 0
    roster->max_students = 0;   //Restablecer el tamaño máximo a 0
}

int comparar(const void *a, const void *b){
    //Punteros a elementos a comparar
    student_t *s1 = (student_t *)a; 
    student_t *s2 = (student_t *)b;
    //si s1->grade es mayor que s2->grade, s1 debe ir después de s2
    if(s1->grade > s2->grade){
        return 1;
    }
    //si es al revés, s1 debe ir antes de s2 
    else if(s1->grade < s2-> grade){
        return -1;
    }
    //Si el valor de la comparación es positivo, indica que s1 va después de s2
    if(strcmp(s1->name, s2->name) > 0){
        return 1;
    } 
    //si es negativo, s1 va antes de s2
    else if(strcmp(s1->name, s2->name) < 0){
        return -1;
    }
    //si s1 y s2 son equivalentes para los fines de la ordenación, devuelve 0
    return 0;
}

bool add_student(roster_t *roster, const char *name, uint8_t grade){
    //Verificar si el estudiante ya está inscrito en algún grado
    for(size_t i = 0; i < roster->count; i++){
        //Comparar nombre 
        if(strcmp((roster->students + i)->name, name) == 0){
            //Retornar false si el estudiante ya se encuentra
            return false;
        }
    }

    //Duplicar el tamaño del arreglo cuando se llene
    if(roster->count == roster->max_students){
        size_t max_students_2;  //Variable para almacenar el nuevo tamaño máximo del arreglo
        //Verificar si el arreglo ya tiene un tamaño definido
        if (roster->max_students > 0) {
            //Si el tamaño es mayor a 0, duplicarlo
            max_students_2 = roster->max_students * 2;
        } else {
            //Si el tamaño es 0, establecerlo en 1, para poder empezar a agregar estudiantes
            max_students_2 = 1;
        }
        student_t *aux = realloc(roster->students, max_students_2 * sizeof(student_t));    // Redimensionar el bloque de memoria
        // Verificar si realloc falló
        if (aux == NULL) {
            return false;                   // Retornar false si no se pudo redimensionar
        }
        roster->students = aux;         // Restaurar el puntero original
        roster->max_students = max_students_2;          //Actualizar la variable del tamaño maximo del arreglo
    }

    (roster->students + roster->count)->name = malloc(strlen(name) + 1);    //Reservar memoria para el nombre
    if((roster->students + roster->count)->name == NULL){
        return false;   //Si no se pudo reservar memoria, se devulve false
    }
    strcpy((roster->students + roster->count)->name, name);     //Copiar el nombre del estudiante a la posición
    (roster->students + roster->count)->grade = grade;          //Agregar el estudiante al final del arreglo
    roster->count++;                                            //Incrementar el conteo de estudiantes

    qsort(roster->students, roster->count, sizeof(student_t), comparar);

    return true;        //ndicar que el estudiante fue agregado satisfactoriamente
}

bool eliminar_estudiante(roster_t *roster, const char *name){
    //Variable para almacenar la posición del estudiante en el arreglo
    //Por ahora está inicializada con índice que no es posible, porque está fuera de rango
    //esto debido a que si lo inicializo en algún valor en específico, la función podría
    //interpretar que ese valor es la posición del estudiante a eliminar y podría haber errores
    size_t estudiante_eliminar = roster->count;
    //Encuentro la posición del alumno y la almaceno en la variable estudiante_eliminar
    for(size_t i = 0; i < roster->count; i++){
        if(strcmp((roster->students + i)->name, name) == 0){
            estudiante_eliminar = i;
            break;
        }
    }
    //Si el estudiante no se encuentra, no se puede eliminar, se retorna false
    if(estudiante_eliminar == roster->count){
        return false;
    }
    //se libera el espacio de memoria reservado para el estudiante a eliminar
    free((roster->students + estudiante_eliminar)->name);
    //se recorren los elementos del arreglo para que no queden huecos o posiciones vacías
    //debido al estudiante eliminado
    for(size_t i = estudiante_eliminar; i < roster->count - 1; i++){
        *(roster->students + i) = *(roster->students + i + 1);
    }
    //se reduce en uno la cantidad de alumnos en el arreglo
    roster->count--;
    //se retorna true si la eliminación fue exitosa
    return true;
}

roster_t get_grade(const roster_t *roster, uint8_t desired_grade){

    roster_t aux;           //Crear un nuevo arreglo para almacenar los estudiantes del mismo grado
    init_roster(&aux);      //Inicializar el arreglo
    
    //Iterar sobre el arreglo original
    for(size_t i = 0; i < roster->count; i++){
        //Verificar si el grado del estudiante es igual al grado que se va a ingresar
        if((roster->students + i)->grade == desired_grade){
            //Agregar el estudiante al arreglo
            add_student(&aux, (roster->students + i)->name, (roster->students + i)->grade);
        }
    }

    return aux;     //Devolver el nuevo arreglo con los estudiantes del grado deseado
}
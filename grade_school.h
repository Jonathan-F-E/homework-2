#ifndef GRADE_SCHOOL_H
#define GRADE_SCHOOL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
   uint8_t grade;
   char *name;
} student_t;

typedef struct {
   size_t count;
   size_t max_students;
   student_t *students;
} roster_t;

void init_roster(roster_t *roster);
void free_roster(roster_t *roster);
int comparar(const void *a, const void *b);
bool add_student(roster_t *roster, const char *name, uint8_t grade);
bool eliminar_estudiante(roster_t *roster, const char *name);
roster_t get_grade(const roster_t *roster, uint8_t desired_grade);

#endif

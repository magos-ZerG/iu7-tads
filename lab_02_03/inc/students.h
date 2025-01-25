#ifndef STUDENTS_H__
#define STUDENTS_H__

#include "student.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>

#define EPS 1e-8

#define MAX_STUDENTS_NUM 1000

#define MAX_FILENAME_LEN 40

#define INVALID_FILENAME 1
#define INVALID_FILE 2
#define INVALID_DATA 3
#define EMPTY_FILE 4
#define EXCEEDED_MAX_STUDENTS_NUM 5

#define STUDENTS_OVERFLOW 1
#define INVALID_STUDENT 2

#define INVALID_PREFIX 1

#define INVALID_YEAR 1
#define INVALID_RENT 2

#define MAX_ARR_LEN 10000
#define TESTS_NUM 7
#define REPEATS_NUM 50

int load_data(student_t students[MAX_STUDENTS_NUM], size_t *students_num);

int print_students_table(student_t students[MAX_STUDENTS_NUM], size_t *students_num);

int print_sorted_key_table(student_t students[MAX_STUDENTS_NUM], size_t *students_num);

int add_student(student_t students[MAX_STUDENTS_NUM], size_t *students_num);

int del_students_by_surname(student_t students[MAX_STUDENTS_NUM], size_t *students_num);

int bubble_sort_students(student_t students[MAX_STUDENTS_NUM], size_t *students_num);

int qsort_students(student_t students[MAX_STUDENTS_NUM], size_t *students_num);

int bubble_sort_students_by_key(student_t students[MAX_STUDENTS_NUM], size_t *students_num);

int qsort_students_by_key(student_t students[MAX_STUDENTS_NUM], size_t *students_num);

int my_filter(student_t students[MAX_STUDENTS_NUM], size_t *students_num);

int time_measurement(void);

#endif  // STUDENTS_H__

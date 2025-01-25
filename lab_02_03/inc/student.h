#ifndef STUDENT_H__
#define STUDENT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFF_LEN 100

#define MAX_STREET_LEN 20
#define MAX_SURNAME_LEN 15
#define MAX_NAME_LEN 15
#define MAX_GROUP_LEN 10
#define MIN_AGE 0
#define MAX_AGE 100
#define MIN_MARK 0
#define MAX_MARK 100
#define MIN_ADMISSION_YEAR 1900
#define MAX_ADMISSION_YEAR 2024

#define MAX_HOUSE_NUM 9999
#define MAX_FLAT_NUM 9999
#define MAX_DORMITORY_NUM 9999
#define MAX_ROOM_NUM 9999

#define INVALID_STREET 1
#define INVALID_HOUSE 2
#define INVALID_FLAT 3
#define INVALID_DORMITORY 4
#define INVALID_ROOM 5
#define INVALID_PRICE 6

#define INVALID_SURNAME 1
#define INVALID_NAME 2
#define INVALID_GROUP 3
#define INVALID_SEX 4
#define INVALID_AGE 5
#define INVALID_MARK 6
#define INVALID_ADMISSION_YEAR 7
#define INVALID_ACCOMODATION_TYPE 8
#define INVALID_ACCOMODATION 9

typedef enum
{
    MALE,
    FEMALE
} sex_t;

typedef enum
{   
    HOUSE,
    DORMITORY,
    RENTED
} accomodation_type;

typedef struct
{
    char street[MAX_STREET_LEN + 1];
    int house;
    int flat;
} house_t;

typedef struct
{
    int dormitory;
    int room;
} dormitory_t;

typedef struct
{
    char street[MAX_STREET_LEN + 1];
    int house;
    int flat;
    double price;
} rented_accomodation_t;

typedef union
{
    house_t house;
    dormitory_t dormitory;
    rented_accomodation_t rented;
} accomodation_t;

typedef struct
{
    char surname[MAX_SURNAME_LEN + 1];
    char name[MAX_NAME_LEN + 1];
    char group[MAX_GROUP_LEN + 1];
    sex_t sex;
    int age;
    double avg_mark;
    int admission_year;
    accomodation_type type;
    accomodation_t accomodation;
} student_t;

int fscan_string(FILE *src_file, char *dst, size_t max_len);

int scan_accomodation(accomodation_t *accomodation, accomodation_type type);

int scan_student(student_t *student);

int fscan_accomodation(FILE *file, accomodation_t *accomodation, accomodation_type type);

int fscan_student(FILE *file, student_t *student);

#endif  // STUDENT_H__

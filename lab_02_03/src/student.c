#include "student.h"

int fscan_string(FILE *src_file, char *dst, size_t max_len)
{
    char buff[MAX_BUFF_LEN];

    if (!fgets(buff, (int)max_len, src_file) || buff[strlen(buff) - 1] != '\n')
        return EXIT_FAILURE;

    buff[strlen(buff) - 1] = '\0';
    strcpy(dst, buff);

    return EXIT_SUCCESS;
}

int scan_accomodation(accomodation_t *accomodation, accomodation_type type)
{
    if (type == HOUSE)
    {
        printf("Введите улицу: ");
        if (fscan_string(stdin, (accomodation->house).street, MAX_STREET_LEN) != 0)
        {
            printf("Введена невалидная улица\n");
            return INVALID_STREET;
        }

        printf("Введите номер дома: ");
        if (scanf("%d", &((accomodation->house).house)) != 1 || (accomodation->house).house <= 0 || (accomodation->house).house > MAX_HOUSE_NUM)
        {
            printf("Введён невалидный номер дома\n");
            return INVALID_HOUSE;
        }
        getchar();

        printf("Введите номер квартиры: ");
        if (scanf("%d", &((accomodation->house).flat)) != 1 || (accomodation->house).flat <= 0 || (accomodation->house).flat > MAX_FLAT_NUM)
        {
            printf("Введён невалидный номер квартиры\n");
            return INVALID_FLAT;
        }
        getchar();
    }
    else if (type == DORMITORY)
    {
        printf("Введите номер общежития: ");
        if (scanf("%d", &((accomodation->dormitory).dormitory)) != 1 || (accomodation->dormitory).dormitory <= 0 || (accomodation->dormitory).dormitory > MAX_DORMITORY_NUM)
        {
            printf("Введён невалидный номер общежития\n");
            return INVALID_DORMITORY;
        }
        getchar();

        printf("Введите номер комнаты: ");
        if (scanf("%d", &((accomodation->dormitory).room)) != 1 || (accomodation->dormitory).room <= 0 || (accomodation->dormitory).room > MAX_ROOM_NUM)
        {
            printf("Введён невалидный номер комнаты\n");
            return INVALID_ROOM;
        }
        getchar();
    }
    else if (type == RENTED)
    {
        printf("Введите улицу: ");
        if (fscan_string(stdin, (accomodation->rented).street, MAX_STREET_LEN) != 0)
        {
            printf("Введена невалидная улица\n");
            return INVALID_STREET;
        }

        printf("Введите номер дома: ");
        if (scanf("%d", &((accomodation->rented).house)) != 1 || (accomodation->rented).house <= 0 || (accomodation->rented).house > MAX_HOUSE_NUM)
        {
            printf("Введён невалидный номер дома\n");
            return INVALID_HOUSE;
        }
        getchar();

        printf("Введите номер квартиры: ");
        if (scanf("%d", &((accomodation->rented).flat)) != 1 || (accomodation->rented).flat <= 0 || (accomodation->rented).flat > MAX_FLAT_NUM)
        {
            printf("Введён невалидный номер квартиры\n");
            return INVALID_FLAT;
        }
        getchar();

        printf("Введите цену аренды: ");
        if (scanf("%lf", &((accomodation->rented).price)) != 1 || (accomodation->rented).price <= 0)
        {
            printf("Введёна невалидная цена аренды\n");
            return INVALID_PRICE;
        }
        getchar();
    }

    return EXIT_SUCCESS;
}

int scan_student(student_t *student)
{
    printf("Введите фамилию: ");
    if (fscan_string(stdin, student->surname, MAX_SURNAME_LEN) != 0)
    {
        printf("Введена невалидная фамилия\n");
        return INVALID_SURNAME;
    }

    printf("Введите имя: ");
    if (fscan_string(stdin, student->name, MAX_NAME_LEN) != 0)
    {
        printf("Введено невалидное имя\n");
        return INVALID_NAME;
    }

    printf("Введите группу: ");
    if (fscan_string(stdin, student->group, MAX_GROUP_LEN) != 0)
    {
        printf("Введена невалидная группа\n");
        return INVALID_GROUP;
    }

    printf("Введите пол в формате M\\F: ");
    char sex;
    if (scanf("%c", &sex) != 1 || (sex != 'M' && sex != 'F'))
    {
        printf("Введён невалидный пол\n");
        return INVALID_SEX;
    }
    if (sex == 'M')
        student->sex = MALE; 
    else if (sex == 'F')
        student->sex = FEMALE;

    printf("Введите возраст: ");
    if (scanf("%d", &student->age) != 1 || student->age < MIN_AGE || student->age > MAX_AGE)
    {
        printf("Введён невалидный возраст\n");
        return INVALID_AGE;
    }
    getchar();

    printf("Введите среднюю оценку: ");
    if (scanf("%lf", &student->avg_mark) != 1 || student->avg_mark < MIN_MARK || student->avg_mark > MAX_MARK)
    {
        printf("Введёна невалидная оценка\n");
        return INVALID_MARK;
    }
    getchar();

    printf("Введите год поступления: ");
    if (scanf("%d", &student->admission_year) != 1 || student->admission_year < MIN_ADMISSION_YEAR || student->admission_year > MAX_ADMISSION_YEAR)
    {
        printf("Введён невалидный год поступления\n");
        return INVALID_ADMISSION_YEAR;
    }
    getchar();

    printf("Введите тип жилья: 1 - дом, 2 - общежитие, 3 - съёмное жильё: ");
    int type;
    if (scanf("%d", &type) != 1 || type < 1 || type > 3)
    {
        printf("Введён невалидный тип жилья\n");
        return INVALID_ACCOMODATION_TYPE;
    }
    getchar();

    if (type == 1)
        student->type = HOUSE;
    else if (type == 2)
        student->type = DORMITORY;
    else if (type == 3)
        student->type = RENTED;

    printf("Введите характеристики жилья\n");
    if (scan_accomodation(&student->accomodation, student->type) != EXIT_SUCCESS)
        return INVALID_ACCOMODATION;

    return EXIT_SUCCESS;
}

int fscan_accomodation(FILE *file, accomodation_t *accomodation, accomodation_type type)
{
    if (type == HOUSE)
    {
        if (fscan_string(file, (accomodation->house).street, MAX_STREET_LEN) != 0)
            return INVALID_STREET;

        if (fscanf(file, "%d", &((accomodation->house).house)) != 1 || (accomodation->house).house <= 0 || (accomodation->house).house > MAX_HOUSE_NUM)
            return INVALID_HOUSE;
        fseek(file, 1, SEEK_CUR);

        if (fscanf(file, "%d", &((accomodation->house).flat)) != 1 || (accomodation->house).flat <= 0 || (accomodation->house).flat > MAX_FLAT_NUM)
            return INVALID_FLAT;
        fseek(file, 1, SEEK_CUR);
    }
    else if (type == DORMITORY)
    {
        if (fscanf(file, "%d", &((accomodation->dormitory).dormitory)) != 1 || (accomodation->dormitory).dormitory <= 0 || (accomodation->dormitory).dormitory > MAX_DORMITORY_NUM)
            return INVALID_DORMITORY;
        fseek(file, 1, SEEK_CUR);

        if (fscanf(file, "%d", &((accomodation->dormitory).room)) != 1 || (accomodation->dormitory).room <= 0 || (accomodation->dormitory).room > MAX_ROOM_NUM)
            return INVALID_ROOM;
       fseek(file, 1, SEEK_CUR);
    }
    else if (type == RENTED)
    {
        if (fscan_string(file, (accomodation->rented).street, MAX_STREET_LEN) != 0)
            return INVALID_STREET;

        if (fscanf(file, "%d", &((accomodation->rented).house)) != 1 || (accomodation->rented).house <= 0 || (accomodation->rented).house > MAX_HOUSE_NUM)
            return INVALID_HOUSE;
        fseek(file, 1, SEEK_CUR);

        if (fscanf(file, "%d", &((accomodation->rented).flat)) != 1 || (accomodation->rented).flat <= 0 || (accomodation->rented).flat > MAX_FLAT_NUM)
            return INVALID_FLAT;
        fseek(file, 1, SEEK_CUR);

        if (fscanf(file, "%lf", &((accomodation->rented).price)) != 1 || (accomodation->rented).price <= 0)
            return INVALID_PRICE;
        fseek(file, 1, SEEK_CUR);
    }

    return EXIT_SUCCESS;
}

int fscan_student(FILE *file, student_t *student)
{	
    if (fscan_string(file, student->surname, MAX_SURNAME_LEN) != 0)
        return INVALID_SURNAME;

    if (fscan_string(file, student->name, MAX_NAME_LEN) != 0)
        return INVALID_NAME;

    if (fscan_string(file, student->group, MAX_GROUP_LEN) != 0)
        return INVALID_GROUP;
	
    char sex;
    if (fscanf(file, "%c", &sex) != 1 || (sex != 'M' && sex != 'F'))
        return INVALID_SEX;

    if (sex == 'M')
        student->sex = MALE; 
    else if (sex == 'F')
        student->sex = FEMALE;

    if (fscanf(file, "%d", &student->age) != 1 || student->age < MIN_AGE || student->age > MAX_AGE)
        return INVALID_AGE;
    fseek(file, 1, SEEK_CUR);

    if (fscanf(file, "%lf", &student->avg_mark) != 1 || student->avg_mark < MIN_MARK || student->avg_mark > MAX_MARK)
        return INVALID_MARK;
    fseek(file, 1, SEEK_CUR);

    if (fscanf(file, "%d", &student->admission_year) != 1 || student->admission_year < MIN_ADMISSION_YEAR || student->admission_year > MAX_ADMISSION_YEAR)
        return INVALID_ADMISSION_YEAR;
    fseek(file, 1, SEEK_CUR);
	
    int type;
    if (fscanf(file ,"%d", &type) != 1 || type < 1 || type > 3)
        return INVALID_ACCOMODATION_TYPE;
    fseek(file, 1, SEEK_CUR);

    if (type == 1)
        student->type = HOUSE;
    else if (type == 2)
        student->type = DORMITORY;
    else if (type == 3)
        student->type = RENTED;

    if (fscan_accomodation(file, &student->accomodation, student->type) != EXIT_SUCCESS)
        return INVALID_ACCOMODATION;

    return EXIT_SUCCESS;
}

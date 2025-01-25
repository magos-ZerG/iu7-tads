#include "students.h"

static int admission_year_comparator(const void *stud1, const void *stud2)
{
	const student_t *student1 = (const student_t *)stud1;
    const student_t *student2 = (const student_t *)stud2;
    return (student1->admission_year > student2->admission_year) - (student1->admission_year < student2->admission_year);
}

static int key_comparator(const void *key_arr1, const void *key_arr2)
{
    int *key1 = (int *)key_arr1 + 1;
    int *key2 = (int *)key_arr2 + 1;
    return (*key1 > *key2) - (*key1 < *key2);
}

static void swap(void *obj1, void *obj2, size_t size)
{
    if (obj1 == obj2)
        return;

    char tmp;
    char *curr_obj1 = (char *)obj1;
    char *curr_obj2 = (char *)obj2;
    while (curr_obj1 < (char *)obj1 + size)
    {
        tmp = *curr_obj1;
        *curr_obj1 = *curr_obj2;
        *curr_obj2 = tmp;
        curr_obj1++;
        curr_obj2++;
    }
}

static void bubble_sort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
    char *obj;
    char *obj_next;
    int swap_flag = 0;

    for (int i = 0; i < (int)nmemb - 1 && !swap_flag; i++)
    {
        swap_flag = 1;
        for (int j = 0; j < (int)nmemb - i - 1; j++)
        {
            obj = (char *)base + j * size;
            obj_next = obj + size;
            if (compar(obj, obj_next) > 0)
            {
                swap(obj, obj_next, size);
                swap_flag = 0;
            }
        }
    }
}

int load_data(student_t students[MAX_STUDENTS_NUM], size_t *students_num)
{
    char filename[MAX_FILENAME_LEN];
    printf("Введите название файла: ");
    if (fscan_string(stdin, filename, MAX_FILENAME_LEN) != 0)
    {
        printf("Введенo невалидное название файла\n");
        return INVALID_FILENAME;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Файл недоступен или закрыт для чтения\n");
        return INVALID_FILE;
    }

    student_t tmp_stud;
    size_t student_count = 0;

    int rc = fscan_student(file, &tmp_stud);
    while (feof(file) == 0 && rc == 0)
    {
        student_count++;
        rc = fscan_student(file, &tmp_stud);
    }
    
    if (rc != 0 && feof(file) == 0)
    {
        printf("Файл содержит невалидные данные\n");
        return INVALID_DATA;
    }

    if (student_count == 0)
    {
        printf("Файл пуст\n");
        return EMPTY_FILE;
    }

    if (student_count > MAX_STUDENTS_NUM)
    {
        printf("В файле превышено максимальное число студентов\n");
        return EXCEEDED_MAX_STUDENTS_NUM;
    }

    fseek(file, 0, SEEK_SET);

    if (*students_num > 0)
    {
        printf("Текущая таблица уже содержит данные.\n");
        printf("Перезаписать [Y/N]:");

        char option;
        int rc = (scanf("%c", &option) != 1 || (option != 'Y' && option != 'N'));
        while (getchar() != '\n');        

        while (rc == 1)
        {
            printf("Перезаписать [Y/N]:");
            rc = (scanf("%c", &option) != 1 || (option != 'Y' && option != 'N'));
            while (getchar() != '\n');        
        }

        if (option == 'N')
        {
            printf("Загрузка данных отменена.\n");
            return EXIT_SUCCESS;
        }
    }

    memset(students, 0, MAX_STUDENTS_NUM * sizeof(student_t));

    for (size_t i = 0; i < student_count; i++)
    {
        fscan_student(file, &tmp_stud);
        students[i] = tmp_stud;
    }

    *students_num = student_count;

    fseek(file, 0, SEEK_SET);

    fclose(file);

    printf("Данные успешно считаны\n");

    return EXIT_SUCCESS;
}

int print_students_table(student_t students[MAX_STUDENTS_NUM], size_t *students_num)
{
    char *separator = "+-----------------+-----------------+------------+-----+---------+------------+-----------------+-----------+---------------------------------------------------------------+";
    printf("%s\n", separator);
    printf("|     Фамилия     |       Имя       |   Группа   | Пол | Возраст | Ср. оценка | Год поступления | Тип жилья |                      Информация о жилье                       |\n");
    printf("%s\n", separator);
    for (size_t i = 0; i < *students_num; i++)
    {
        printf("| %-15s | %-15s | %-10s |", students[i].surname, students[i].name, students[i].group);

        if (students[i].sex == MALE)
            printf("  M  |");
        else if (students[i].sex == FEMALE)
            printf("  F  |");   

        printf(" %-7d | %-10.6lf | %-15d |", students[i].age, students[i].avg_mark, students[i].admission_year);

        if (students[i].type == HOUSE)
            printf("    Дом    | ул. %-20s, д. %-4d, кв. %-4d                   |", students[i].accomodation.house.street, students[i].accomodation.house.house, students[i].accomodation.house.flat);
        else if (students[i].type == DORMITORY)
            printf(" Общежитие | общежитие %-4d, комната %-4d                                  |", students[i].accomodation.dormitory.dormitory, students[i].accomodation.dormitory.room);
        else if (students[i].type == RENTED)
            printf("  Аренда   | ул. %-20s, д. %-4d, кв. %-4d, цена: %-10.2lf |", students[i].accomodation.rented.street, students[i].accomodation.rented.house, students[i].accomodation.rented.flat, students[i].accomodation.rented.price);

        printf("\n");

        printf("%s\n", separator);
    }

    return EXIT_SUCCESS;
}

static void fill_key_table(int key[MAX_STUDENTS_NUM][2], student_t students[MAX_STUDENTS_NUM], size_t students_num)
{
    for (size_t i = 0; i < students_num; i++)
    {
        key[i][0] = i + 1;
        key[i][1] = students[i].admission_year;
    } 
}

static void print_key_table(int key[MAX_STUDENTS_NUM][2], size_t students_num)
{
    char *separator = "+--------+-----------------+";
    printf("%s\n", separator);
    printf("| Индекс | Год поступления |\n");
    printf("%s\n", separator);
    for (int i = 0; i < students_num; i++) 
    {
        printf("|   %-3d  |   %-11d   |\n", key[i][0], key[i][1]);
        printf("%s\n", separator);
    }
}

int print_sorted_key_table(student_t students[MAX_STUDENTS_NUM], size_t *students_num)
{
    int key_table[MAX_STUDENTS_NUM][2];

    fill_key_table(key_table, students, *students_num);

    qsort(key_table, *students_num, sizeof(key_table[0]), key_comparator);

    print_key_table(key_table, *students_num);

    return EXIT_SUCCESS;
}

int add_student(student_t students[MAX_STUDENTS_NUM], size_t *students_num)
{
    if (*students_num >= MAX_STUDENTS_NUM)
    {
        printf("В таблице максимально возможное число студентов\n");
        return STUDENTS_OVERFLOW;
    }

    student_t student;    
    int rc = scan_student(&student);
    if (rc != EXIT_SUCCESS)
        return INVALID_STUDENT;

    students[(*students_num)++] = student;
    printf("Студент успешно добавлен\n");

    return EXIT_SUCCESS;
} 

int del_students_by_surname(student_t students[MAX_STUDENTS_NUM], size_t *students_num)
{
    char surname_pref[MAX_SURNAME_LEN];
    printf("Введите префикс фамилии: ");
    if (fscan_string(stdin, surname_pref, MAX_SURNAME_LEN) != 0)
    {
        printf("Введен невалидный префикс\n");
        return INVALID_STREET;
    }

    size_t shift = 0;

    for (size_t i = 0; i < *students_num; i++)
    {
        if (strncmp(students[i].surname, surname_pref, strlen(surname_pref)) == 0)
            shift++;
        else
            students[i - shift] = students[i];
    }

    *students_num -= shift;

    printf("Удалено %zu студентов\n", shift);

    return EXIT_SUCCESS;
}  

int bubble_sort_students(student_t students[MAX_STUDENTS_NUM], size_t *students_num)
{
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    bubble_sort(students, *students_num, sizeof(student_t), admission_year_comparator);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    long long time_delta_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    
    printf("Студенты успешно отсортированы\n");
    size_t memory = *students_num * sizeof(student_t);
    printf("Время сортировки: %lld нс, затраченная память: %zu байт\n", time_delta_ns, memory);

    return EXIT_SUCCESS;
}

int qsort_students(student_t students[MAX_STUDENTS_NUM], size_t *students_num)
{
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    qsort(students, *students_num, sizeof(student_t), admission_year_comparator);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    long long time_delta_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    
    printf("Студенты успешно отсортированы\n");
    size_t memory = *students_num * sizeof(student_t);
    printf("Время сортировки: %lld нс, затраченная память: %zu байт\n", time_delta_ns, memory);

    return EXIT_SUCCESS;
}

static void print_students_table_by_key(int key[MAX_STUDENTS_NUM][2], student_t students[MAX_STUDENTS_NUM], size_t students_num)
{
    char *separator = "+-----------------+-----------------+------------+-----+---------+------------+-----------------+-----------+---------------------------------------------------------------+--------+-----------------+";
    printf("%s\n", separator);
    printf("|     Фамилия     |       Имя       |   Группа   | Пол | Возраст | Ср. оценка | Год поступления | Тип жилья |                      Информация о жилье                       | Индекс | Год поступления |\n");
    printf("%s\n", separator);
    for (size_t i = 0; i < students_num; i++)
    {
        size_t ind = key[i][0] - 1;
        printf("| %-15s | %-15s | %-10s |", students[ind].surname, students[ind].name, students[ind].group);

        if (students[ind].sex == MALE)
            printf("  M  |");
        else if (students[ind].sex == FEMALE)
            printf("  F  |");   

        printf(" %-7d | %-10.6lf | %-15d |", students[ind].age, students[ind].avg_mark, students[ind].admission_year);

        if (students[ind].type == HOUSE)
            printf("    Дом    | ул. %-20s, д. %-4d, кв. %-4d                   |", students[ind].accomodation.house.street, students[ind].accomodation.house.house, students[ind].accomodation.house.flat);
        else if (students[ind].type == DORMITORY)
            printf(" Общежитие | общежитие %-4d, комната %-4d                                  |", students[ind].accomodation.dormitory.dormitory, students[ind].accomodation.dormitory.room);
        else if (students[ind].type == RENTED)
            printf("  Аренда   | ул. %-20s, д. %-4d, кв. %-4d, цена: %-10.2lf |", students[ind].accomodation.rented.street, students[ind].accomodation.rented.house, students[ind].accomodation.rented.flat, students[ind].accomodation.rented.price);

        printf("   %-3d  |   %-11d   |", key[i][0], key[i][1]);
        printf("\n");

        printf("%s\n", separator);
    }
}

int bubble_sort_students_by_key(student_t students[MAX_STUDENTS_NUM], size_t *students_num)
{
    int key_table[MAX_STUDENTS_NUM][2];
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    fill_key_table(key_table, students, *students_num);

    bubble_sort(key_table, *students_num, sizeof(key_table[0]), key_comparator);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    print_students_table_by_key(key_table, students, *students_num);

    long long time_delta_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    size_t memory = *students_num * (sizeof(student_t) + 2 * sizeof(int));
    printf("Время сортировки: %lld нс, затраченная память: %zu байт\n", time_delta_ns, memory);

    return EXIT_SUCCESS;
}

int qsort_students_by_key(student_t students[MAX_STUDENTS_NUM], size_t *students_num)
{
    int key_table[MAX_STUDENTS_NUM][2];
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    fill_key_table(key_table, students, *students_num);

    qsort(key_table, *students_num, sizeof(key_table[0]), key_comparator);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    
    print_students_table_by_key(key_table, students, *students_num);

    long long time_delta_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    size_t memory = *students_num * (sizeof(student_t) + 2 * sizeof(int));
    printf("Время сортировки: %lld нс, затраченная память: %zu байт\n", time_delta_ns, memory);

    return EXIT_SUCCESS;
}

//Выводит студентов указанного года поступления, живущих в съёмном жилье, дешевле указанного
int my_filter(student_t students[MAX_STUDENTS_NUM], size_t *students_num)
{
    int admission_year;
    printf("Введите год поступления: ");
    if (scanf("%d", &admission_year) != 1 || admission_year < MIN_ADMISSION_YEAR || admission_year > MAX_ADMISSION_YEAR)
    {
        printf("Введён невалидный год поступления\n");
        return INVALID_YEAR;
    }
    getchar();

    double rent;
    printf("Введите стоимость: ");
    if (scanf("%lf", &rent) != 1)
    {
        printf("Введена невалидная стоимость\n");
        return INVALID_RENT;
    }
    getchar();

    int ind = 0;
    printf("Студенты %d года поступления, живущие в съёмном жилье, дешевле %lf:\n", admission_year, rent);
    while (ind < *students_num)
    {
        if (students[ind].admission_year == admission_year && students[ind].type == RENTED && students[ind].accomodation.rented.price - rent < -EPS)
            printf("%s %s %s\n", students[ind].surname, students[ind].name, students[ind].group);
        ind++;
    }

    if (ind == 0)
        printf("Не найдены\n");

    return EXIT_SUCCESS;
}

static void init_data(student_t data_arr[MAX_ARR_LEN], size_t elem_num)
{
    for (size_t i = 0; i < elem_num; i++)
        data_arr[i].admission_year = MIN_ADMISSION_YEAR + rand() % (MAX_ADMISSION_YEAR - MIN_ADMISSION_YEAR);
}

int time_measurement(void)
{
    size_t elem_num[15] = {5, 10, 50, 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
    
    struct timespec start, end;
    int64_t time_delta_ns = 0; 
    long long time_data[TESTS_NUM][4];
    srand(time(NULL));  

    student_t data_arr[MAX_ARR_LEN];
    memset(data_arr, 0, MAX_ARR_LEN * sizeof(student_t));
    int key_table[MAX_ARR_LEN][2];

    for (size_t i = 0; i < TESTS_NUM; i++)
    {
        for (size_t j = 0; j < REPEATS_NUM; j++)
        {
            init_data(data_arr, elem_num[i]);
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            bubble_sort(data_arr, elem_num[i], sizeof(student_t), admission_year_comparator);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            time_delta_ns += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
        }

        time_data[i][0] = time_delta_ns / REPEATS_NUM;
        time_delta_ns = 0;

        for (size_t j = 0; j < REPEATS_NUM; j++)
        {
            init_data(data_arr, elem_num[i]);
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            qsort(data_arr, elem_num[i], sizeof(student_t), admission_year_comparator);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            time_delta_ns += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
        }

        time_data[i][1] = time_delta_ns / REPEATS_NUM;
        time_delta_ns = 0;

        for (size_t j = 0; j < REPEATS_NUM; j++)
        {
            init_data(data_arr, elem_num[i]);
            fill_key_table(key_table, data_arr, elem_num[i]);
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            bubble_sort(key_table, elem_num[i], sizeof(key_table[0]), key_comparator);       
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            time_delta_ns += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
        }

        time_data[i][2] = time_delta_ns / REPEATS_NUM;
        time_delta_ns = 0;

        for (size_t j = 0; j < REPEATS_NUM; j++)
        {
            init_data(data_arr, elem_num[i]);
            fill_key_table(key_table, data_arr, elem_num[i]);
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            qsort(key_table, elem_num[i], sizeof(key_table[0]), key_comparator);        
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            time_delta_ns += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
        }

        time_data[i][3] = time_delta_ns / REPEATS_NUM;
        time_delta_ns = 0;
    }

    char *separator = "+-----------+--------------+--------------+--------------+--------------+---------+-----------------+-------------+";
    printf("%s\n", separator);
    printf("| Кол-во    | Медленная    | Быстрая      | Медленная    | Быстрая      | Размер  | Размер          | Затраченная |\n");
    printf("| элементов | сортировка,  | сортировка,  | сортировка   | сортировка   | данных, | таблицы ключей, | память,     |\n");
    printf("|           | нс.          | нс.          | ключей, нс.  | ключей, нс.  | байт    | байт            | байт        |\n");
    printf("%s\n", separator);

    size_t data_size, key_size, memory;
    
    for (size_t i = 0; i < TESTS_NUM; i++)
    {
        data_size = elem_num[i] * sizeof(student_t);
        key_size = elem_num[i] * 2 * sizeof(int);
        memory = data_size + key_size;
        printf("|   %-5zu   | %-12lld | %-12lld | %-12lld | %-12lld | %-7zu | %-15zu | %-11zu |\n", elem_num[i], time_data[i][0], time_data[i][1], time_data[i][2], time_data[i][3], data_size, key_size, memory);
        printf("%s\n", separator);
    }

    separator = "+-----------+--------------+--------------+";
    printf("%s\n", separator);
    printf("| Кол-во    | Медленная    | Быстрая      |\n");
    printf("| элементов | сортировка,  | сортировка,  |\n");
    printf("|           | %%            | %%            |\n");
    printf("%s\n", separator);

    double slow_sort_eff, quik_sort_eff; 
    for (size_t i = 0; i < TESTS_NUM; i++)
    {
        slow_sort_eff = (double)(time_data[i][0] - time_data[i][2]) / time_data[i][2] * 100.0;
        quik_sort_eff = (double)(time_data[i][1] - time_data[i][3]) / time_data[i][3] * 100.0;
        printf("|   %-5zu   | %-12.3lf | %-12.3lf |\n", elem_num[i], slow_sort_eff, quik_sort_eff);
        printf("%s\n", separator);
    }

    return EXIT_SUCCESS;
}

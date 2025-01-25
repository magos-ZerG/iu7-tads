#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static void print_menu(void);

int main(void)
{
    printf("Программа для нахождения самого длинного простого пути в графе\n");
    printf("\n");

    int rc;
    int option;
    int is_running = 1;

    char *filename = NULL;
    size_t filename_len = 0;

    FILE *src_file = NULL;
    FILE *dst_file = NULL;

    graph_t graph = {0, NULL};

    path_t max_path = {0, NULL};

    while (is_running)
    {
        print_menu();
        printf("Выберите опцию: ");
        rc = (scanf("%d", &option) == 1 && getchar() == '\n');

        if (rc == 1)
        {
            switch (option)
            {
            case 1:
                printf("Введите название файла: ");
                if (getline(&filename, &filename_len, stdin) == -1 || strlen(filename) <= 1)
                    printf("Невалидное имя файла.\n");
                filename[strlen(filename) - 1] = '\0';

                src_file = fopen(filename, "r");
                if (src_file == NULL)
                    printf("Невозможно открыть файл на чтение\n");
                else
                {
                    destroy_graph(&graph);
                    rc = import_graph(src_file, &graph);
                    if (rc != EXIT_SUCCESS)
                        printf("Файл содержит невалидные данные\n");
                    else
                        printf("Граф успешно загружен\n");
                    fclose(src_file);
                }
                break;
            case 2:
                if (graph.vertex_num == 0)
                    printf("Граф не загружен\n");
                else
                {
                    free(max_path.path);
                    max_path.path_len = 0;
                    max_path.path = calloc(graph.vertex_num * graph.vertex_num, sizeof(int));

                    find_max_simple_path(&graph, &max_path);

                    printf("Самый длинный путь: ");
                    print_path(&max_path);
                }
                break;
            case 3:
                if (graph.vertex_num == 0)
                    printf("Граф не загружен\n");
                else
                {
                    printf("Введите название файла: ");
                    if (getline(&filename, &filename_len, stdin) == -1 || strlen(filename) <= 1)
                        printf("Невалидное имя файла.\n");
                    filename[strlen(filename) - 1] = '\0';

                    dst_file = fopen(filename, "w+");
                    if (dst_file == NULL)
                        printf("Невозможно открыть файл на запись\n");
                    else
                    {
                        export_graph_to_dot(dst_file, &graph);
                        fclose(dst_file);
                    }
                }
                break;
            case 4:
                is_running = 0;
                break;
            default:
                printf("Введена невалидная опция\n");
                while (getchar() != '\n'); 
                break;
            }
        }
        else
        {
            printf("Введена невалидная опция\n");
            while (getchar() != '\n'); 
        }
        printf("\n");
    }

    destroy_graph(&graph);
    free(max_path.path);
    free(filename);

    return EXIT_SUCCESS;
}

static void print_menu(void)
{
    printf("Меню:\n");
    printf("1 - Загрузить граф из файла\n");
    printf("2 - Найти самый длинный простой путь в графе\n");
    printf("3 - Вывести граф в файл\n");
    printf("4 - Выход\n");
    printf("\n");
}

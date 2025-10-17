/*
 * Программа: База данных студентов
 * Описание: Консольное приложение для управления базой данных студентов
 * с возможностью добавления, просмотра и сохранения данных в файл
 * 
 */

#include <stdio.h>
#include <stdlib.h>

// Структура для хранения информации о студенте
typedef struct {
    int id;          // Уникальный идентификатор
    char name[50];   // Имя студента
    int age;         // Возраст
    float grade;     // Оценка
} Student;



// Прототипы функций
void show_menu();                                           // Отображение главного меню
int add_student(Student**, int*);                           // Добавление нового студента
int show_students(Student*, int*);                          // Вывод списка всех студентов
int save_to_file(Student*, int*, char*);                    // Сохранение данных в файл
int load_from_file(Student**, int*, char*);                 // Загрузка данных из файла

/*
 * Главная функция программы
 */
int main() {
    // Имя файла для хранения данных
    char* filename = "Students.dat";

    // Динамический массив студентов и счётчик
    Student* students = NULL;
    int count = 0;

    // Автоматическая загрузка данных при старте программы
    load_from_file(&students, &count, filename);

    // Главный цикл программы
    int choice = -1;
    while(choice != 0){
        show_menu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                add_student(&students, &count);
                break;
            case 2:
                show_students(students, &count);
                break;
            case 3:
                save_to_file(students, &count, filename);
                break;
            case 0:
                printf("\n═══════════════════════════════\n");
                printf("  Хорошая работа! До свидания!\n");
                printf("═══════════════════════════════\n");
                free(students);  // Освобождение памяти перед выходом
                break;
            default:
                printf("\n✖ Неизвестная команда! Попробуйте снова.\n");
                break;
        }
    }
    
    
    
    return 0;
}

/*
 * Отображение главного меню программы
 */
void show_menu() {
    printf("\n╔════════════════════════════════╗\n");
    printf("║    БАЗА ДАННЫХ СТУДЕНТОВ      ║\n");
    printf("╠════════════════════════════════╣\n");
    printf("║ 1. Добавить студента           ║\n");
    printf("║ 2. Показать всех студентов     ║\n");
    printf("║ 3. Сохранить в файл            ║\n");
    printf("║ 0. Выход                       ║\n");
    printf("╚════════════════════════════════╝\n");
    printf("Ваш выбор: ");
}

/*
 * Добавление нового студента в базу данных
 * Параметры:
 *   students - указатель на указатель массива студентов (для изменения массива)
 *   count    - указатель на счётчик студентов
 * Возвращает: 0 при успехе, 1 при ошибке
 */
int add_student(Student** students, int* count){
    // Увеличиваем размер массива на 1 элемент
    Student* temp = realloc(*students, (*count + 1) * sizeof(Student));
    if(!temp) {
        printf("\n✖ Ошибка: не удалось выделить память!\n");
        return 1;
    }
    *students = temp;  // Обновляем указатель на новый массив

    printf("\n┌─ Добавление студента ─────────┐\n");
    printf("│ Введите ID: ");
    scanf("%d", &(*students)[*count].id);
    printf("│ Введите имя: ");
    scanf("%s", (*students)[*count].name);
    printf("│ Введите возраст: ");
    scanf("%d", &(*students)[*count].age);
    printf("│ Введите оценку: ");
    scanf("%f", &(*students)[*count].grade);
    printf("└───────────────────────────────┘\n");

    printf("✔ Студент успешно добавлен!\n");
    (*count)++;  // Увеличиваем счётчик студентов

}

/*
 * Вывод списка всех студентов в виде таблицы
 * Параметры:
 *   students - массив студентов
 *   count    - указатель на количество студентов
 * Возвращает: 0 при успехе, 1 если база пуста
 */
int show_students(Student* students, int *count) {
    if(*count == 0){
        printf("\n⚠ База данных пуста! Добавьте студентов.\n");
        return 1;
    }

    printf("\n╔══════╦══════════════════╦═════════╦═════════╗\n");
    printf("║  ID  ║       Имя        ║ Возраст ║ Оценка  ║\n");
    printf("╠══════╬══════════════════╬═════════╬═════════╣\n");

    for(int i = 0; i < *count; i++) {
        printf("║ %-4d ║ %-16s ║  %-6d ║  %-6.2f ║\n",
               students[i].id,
               students[i].name,
               students[i].age,
               students[i].grade);
    }

    printf("╚══════╩══════════════════╩═════════╩═════════╝\n");
    printf("Всего студентов: %d\n", *count);
    return 0;
}

/*
 * Сохранение всех студентов в бинарный файл
 * Параметры:
 *   students - массив студентов
 *   count    - указатель на количество студентов
 *   filename - имя файла для сохранения
 * Возвращает: 0 при успехе, 1 при ошибке
 */
int save_to_file(Student* students, int* count, char* filename) {
    FILE* fp_write = fopen(filename, "wb");  // Открываем файл для бинарной записи
    if(!fp_write) {
        printf("\n✖ Ошибка: не удалось открыть файл для записи!\n");
        return 1;
    }

    // Записываем весь массив студентов одним вызовом
    if(fwrite(students, sizeof(Student), *count, fp_write) != *count) {
        printf("\n✖ Ошибка: не удалось записать данные в файл!\n");
        fclose(fp_write);
        return 1;
    }

    fclose(fp_write);
    printf("\n✔ Успешно сохранено студентов: %d\n", *count);
    printf("  Файл: %s\n", filename);
    return 0;

}

/*
 * Загрузка студентов из бинарного файла
 * Параметры:
 *   students - указатель на указатель массива студентов (для изменения массива)
 *   count    - указатель на счётчик студентов
 *   filename - имя файла для загрузки
 * Возвращает: 0 при успехе, 1 при ошибке
 */
int load_from_file(Student** students, int* count, char* filename) {

    FILE* fp_read = fopen(filename, "rb");  // Открываем файл для бинарного чтения
    if(!fp_read) {
        printf("\n⚠ Файл не найден. Начинаем с пустой базы данных.\n");
        return 0;
    }

    // Определяем размер файла для вычисления количества студентов
    fseek(fp_read, 0, SEEK_END);        // Переходим в конец файла
    size_t file_size = ftell(fp_read);   // Получаем размер в байтах
    rewind(fp_read);                     // Возвращаемся в начало
    *count = file_size / sizeof(Student); // Вычисляем количество записей

    // Выделяем память для загрузки данных
    Student* temp = realloc(*students, sizeof(Student) * *count);
    if(!temp) {
        printf("\n✖ Ошибка: не удалось выделить память для загрузки данных!\n");
        fclose(fp_read);
        return 1;
    }

    *students = temp;  // Обновляем указатель на новый массив

    // Читаем все данные из файла одним вызовом
    if(fread(*students, sizeof(Student), *count, fp_read) != *count) {
        printf("\n✖ Ошибка: не удалось прочитать данные из файла!\n");
        fclose(fp_read);
        return 1;
    }

    fclose(fp_read);
    printf("\n✔ Успешно загружено студентов: %d\n", *count);
    printf("  Файл: %s\n", filename);

    return 0;
}
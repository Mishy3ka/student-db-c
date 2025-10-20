#include "student.h"
#include "func.h"
#include <stdlib.h>
#include <stdio.h>
/*
 * Отображение главного меню программы
 */
void show_menu() {
    printf("\n╔════════════════════════════════╗\n");
    printf("║    БАЗА ДАННЫХ СТУДЕНТОВ       ║\n");
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

    // Автоматическая генерация уникального ID
    int new_id = 1;  // По умолчанию ID = 1
    if(*count > 0) {
        // Находим максимальный ID среди существующих студентов
        int max_id = (*students)[0].id;
        for(int i = 1; i < *count; i++) {
            if((*students)[i].id > max_id) {
                max_id = (*students)[i].id;
            }
        }
        new_id = max_id + 1;  // Новый ID = максимальный + 1
    }

    printf("\n┌─ Добавление студента ─────────┐\n");
    printf("│ Присвоен ID: %d\n", new_id);
    (*students)[*count].id = new_id;  // Присваиваем автоматический ID
    printf("│ Введите имя: ");
    scanf("%s", (*students)[*count].name);
    printf("│ Введите возраст: ");
    scanf("%d", &(*students)[*count].age);
    printf("│ Введите оценку: ");
    scanf("%f", &(*students)[*count].grade);
    printf("└───────────────────────────────┘\n");

    printf("✔ Студент успешно добавлен!\n");
    (*count)++;  // Увеличиваем счётчик студентов

    return 0;
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

    printf("\n════════════════════════════════════════════════════════\n");
    printf("  ID      Имя                 Возраст      Оценка\n");
    printf("════════════════════════════════════════════════════════\n");

    for(int i = 0; i < *count; i++) {
        printf(" %4d    %-20s   %5d      %7.2f\n",
               students[i].id,
               students[i].name,
               students[i].age,
               students[i].grade);
    }

    printf("════════════════════════════════════════════════════════\n");
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
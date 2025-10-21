/*
 * Программа: База данных студентов
 * Описание: Консольное приложение для управления базой данных студентов
 * с возможностью добавления, просмотра и сохранения данных в файл
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "student.h"
#include "func.h"



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
                del_student(&students, &count);
                break;
            case 4:
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


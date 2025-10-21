#ifndef FUNC_H
#define FUNC_H
#include "student.h"
// Прототипы функций
void show_menu();                                           // Отображение главного меню
int add_student(Student**, int*);                           // Добавление нового студента
int show_students(Student*, int*);                          // Вывод списка всех студентов
int save_to_file(Student*, int*, char*);                    // Сохранение данных в файл
int load_from_file(Student**, int*, char*);                 // Загрузка данных из файла
int del_student(Student**, int*);                           // Удаление студента

#endif
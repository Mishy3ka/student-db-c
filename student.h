#ifndef STUDENT_H
#define STUDENT_H


// Структура для хранения информации о студенте
typedef struct {
    int id;          // Уникальный идентификатор
    char name[50];   // Имя студента
    int age;         // Возраст
    float grade;     // Оценка
} Student;

#endif
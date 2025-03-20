/*
Опросчик модулей ввода-вывода.
Содержит в себе основную логику и связь компонентов системы
*/

#ifndef POOLER_H
#define POOLER_H

#include <stdint.h>
#include <stdbool.h>
#include "module.h"

// Состояние модуля (Enum)
typedef enum
{
    POOLER_STATE_NONE,      // Не инициализирован
    POOLER_STATE_INIT,      // Инициализация
    POOLER_STATE_STANDBY,   // Ожидание
    POOLER_STATE_READING,   // Чтение
    POOLER_STATE_WRITING,   // Запись
    POOLER_STATE_ERROR      // Ошибка
} POOLER_STATE_ENUM;

/// @brief Проинициализировать опросчик
/// @param modules Модули ввода-вывода
/// @param modulesCount Количество модулей
/// @return Результат (0 - успех, отрицательное число - ошибка)
int Pooler_Init(Module *modules, int modulesCount);

/// @brief Получить состояние опросчика
/// @return Состояние опросчика
POOLER_STATE_ENUM Pooler_GetState();

/// @brief Прочитать модуль
/// @param module Модуль
/// @return Результат: true - успешно; false - ошибка
bool Pooler_ReadModule(Module *module);

/// @brief Записать модуль
/// @param module Модуль
/// @return Результат: true - успешно; false - ошибка
bool Pooler_WriteModule(Module *module);

/// @brief Обновить данные модулей. Опросить модули (Чтение + запись)
/// @return Результат: true - успешно; false - ошибка
bool Pooler_UpdateModules();

#endif
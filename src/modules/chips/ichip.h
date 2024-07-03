/*
Интерфейс чипа.
Создает высокий уровень адстракции для работы с чипом.
Оспользуется для дальнейшего создания чипа с определенным поведением
на основе структуры Chip.
*/

#ifndef ICHIP_H
#define ICHIP_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"

// MOD: Наименование чипа (Enum)
typedef enum
{
    CHIP_NAME_NONE,       // Нет
    CHIP_NAME_PCA8574,    // PCF8574 (DIO)
    CHIP_NAME_PCA8575,    // PCF8575 (DIO)
    CHIP_NAME_ADS1115,    // ADS1115 (AI)
    CHIP_NAME_PCA9685,    // PCA9685 (AO)
    CHIP_NAME_XXX
} CHIP_NAME_ENUM;



/// @brief Данные вывода чипа
typedef struct
{
    bool updated;               // Обновлено (флаг состояния "прочитаны/записаны" значения)
    int number;                 // Номер вывода чипа
    int value;                  // Значение (< 0 - значение не получено)
} Chip_PinData;

/// @brief Чип
typedef struct
{
    int address;                                                // Адрес
    int pinsCount;                                              // Количество выводов
    CHIP_NAME_ENUM name;                                        // Наименование
    I2C_Connection *connection;                                 // Подключение по I2C
    Chip_PinData *pinsData;                                     // Выводы
    bool (*ReadPins)(I2C_Connection *connection, int address, Chip_PinData *pinsData, int pinsCount);    // Функция считывания выводов. return: true - успех, false - ошибка.
    bool (*WritePins)(I2C_Connection *connection, int address, Chip_PinData *pinsData, int pinsCount);   // Функция записи выводов. return: true - успех, false - ошибка
} Chip;

#endif
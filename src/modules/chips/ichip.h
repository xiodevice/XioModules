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

/// @brief Чип
typedef struct
{
    int address;                    // Адрес
    CHIP_NAME_ENUM name;            // Наименование
    I2C_Connection *connection;     // Подключение по I2C    
    int (*ReadPin)(int);            // Функция считывания. Params: number - номер вывода, return: считанное значение (в случае ошибки значение < 0)
    int (*WritePin)(int, int);      // Функция записи. Params: number - номер вывода, value - значение. return: 0 - успех, -1 - ошибка
} Chip;

#endif
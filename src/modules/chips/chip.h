/*
Интерфейс чипа.
Создает высокий уровень адстракции для работы с чипом
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


// Чип
typedef struct
{
    int address;                    // Адрес
    CHIP_NAME_ENUM name;            // Наименование
    I2C_Connection *connection;     // Подключение по I2C    
    int (*ReadPin)(int);            // Функция считывания. Params: number - номер вывода, return: считанное значение (в случае ошибки значение < 0)
    int (*WritePin)(int, int);      // Функция записи. Params: number - номер вывода, value - значение. return: 0 - успех, -1 - ошибка
} Chip;


/// @brief Создать экземпляр чипа
/// @param chipName Наименование чипа
/// @param connection Подключение
/// @param address Адрес
/// @return Экземпляр чипа. NULL - ошибка создания
Chip* Chip_Create(CHIP_NAME_ENUM chipName, I2C_Connection* connection, int address);

/// @brief Удалить экземпляр чипа
/// @param chip Чип
/// @return Результат: true - успешно, false - ошибка
bool Chip_Destroy(Chip* chip);

/// @brief Проверить подключение чипа
/// @param chip Чип
/// @return Результат: true - успешно, false - ошибка
bool Chip_CheckConnection(Chip* chip);

/// @brief Прочитать вывод чипа
/// @param chip Чип
/// @param pinNumber Номер вывода
/// @return Значение вывода. -1 - ошибка
int Chip_ReadPin(Chip* chip, int pinNumber);

/// @brief Записать вывод чипа
/// @param chip Чип
/// @param pinNumber Номер вывода
/// @param value Значение вывода
/// @return Результат: true - успешно, false - ошибка
bool Chip_WritePin(Chip* chip, int pinNumber, int value);

#endif
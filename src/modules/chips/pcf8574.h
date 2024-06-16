/*
Модуль для работы с микросхемой PCF8574
*/

#ifndef PCF8574CHIP_H
#define PCF8574CHIP_H

#include "chip.h"
#include "i2c.h"


/// @brief Создать экземпляр микросхемы PCF8574. После использования освободить память free()
/// @param chipName Наименование микросхемы
/// @param address Адрес микросхемы
/// @return Экземпляр микросхемы. NULL - ошибка
Chip* Pcf8574Chip_Create(I2C_Connection *connection, int address);

#endif
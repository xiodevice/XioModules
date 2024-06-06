/*
Модуль для работы с микросхемой PCF8574
*/

#ifndef PCF8574CHIP_H
#define PCF8574CHIP_H

#include "ichip.h"

/// @brief Инициализировать микросхему PCF8574
/// @param chip Чип
/// @return Результат операции (Код ошибки)
int Pcf8574Chip_Init(iChip* chip);

#endif
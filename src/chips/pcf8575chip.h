/*
Модуль для работы с микросхемой PCF8575
*/

#ifndef PCF8575MODULE_H
#define PCF8575MODULE_H

#include "ichip.h"

/// @brief Инициализировать микросхему PCF8575
/// @param chip Чип
/// @return Результат операции (Код ошибки)
int Pcf8575Chip_Init(iChip* chip);

#endif
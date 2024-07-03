/*
MDO8R1. Модуль дискретного ввода. 
Обертка над микросхемами PCF8574 и др.
*/

#ifndef MDO8R1_MODULE_H
#define MDO8R1_MODULE_H

#include <stdbool.h>
#include "module.h"

typedef enum
{
    MDO8R1_MODULE_CHIP_NONE,
    MDO8R1_MODULE_CHIP_PCF8574
} MDO8R1_MODULE_CHIP_ENUM;

/// @brief Создать модуль MDO8R1
/// @param module Конфигурация модуля
/// @param connection Подключение
/// @param chipName Наименование чипа
/// @return Экземпляр модуля
Module* MDO8R1Module_Create(Module_Config *config, I2C_Connection *connection, MDO8R1_MODULE_CHIP_ENUM chipName);

#endif
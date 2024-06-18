/*
MDI8. Модуль дискретного ввода. 
Обертка над микросхемами PCF8574 и др.
*/

#ifndef MDI8_MODULE_H
#define MDI8_MODULE_H

#include <stdbool.h>
#include "module.h"

typedef enum
{
    MDI8_MODULE_CHIP_NONE,
    MDI8_MODULE_CHIP_PCF8574
} MDI8_MODULE_CHIP_ENUM;

/// @brief Создать модуль MDI8
/// @param module Конфигурация модуля
/// @param connection Подключение
/// @param chipName Наименование чипа
/// @return Экземпляр модуля
Module* MDI8Module_Create(Module_Config *config, I2C_Connection *connection, MDI8_MODULE_CHIP_ENUM chipName);

#endif
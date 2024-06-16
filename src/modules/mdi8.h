/*
MDI8. Модуль дискретного ввода
*/

#ifndef MDI8_MODULE_H
#define MDI8_MODULE_H

#include <stdbool.h>
#include "module.h"
#include "chips/chip.h"

typedef enum
{
    MDI8_MODULE_CHIP_NONE,
    MDI8_MODULE_CHIP_PCF8574
} MDI8_MODULE_CHIP_ENUM;


/// @brief Инициализировать модуль
/// @param module Конфигурация модуля
/// @return Экземпляр модуля
Module* MDI8Module_Create(Module_Config *config, I2C_Connection *connection, MDI8_MODULE_CHIP_ENUM chipName);

#endif
#include <stdlib.h>
#include "../log.h"
#include "mdi8.h"
#include "chips/pcf8574.h"

/// @brief Количество выводов модуля
static int const MDI8_PIN_COUNT = 8;
/// @brief Соответствие выводов модуля выводам микросхемы. MDI8 (PCF8574)
/// @brief Индексы - выводы модуля;
/// @brief Значения - выводы микросхемы.
static int const MDI8_PCF8574_PIN_CORRESPONDENCE[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

// ***** Предоставляемые интерфейсы module_io.h (реализация)*****

Module* MDI8Module_Create(Module_Config *config, I2C_Connection *connection, MDI8_MODULE_CHIP_ENUM chipName)
{
    // TODO: Ready to test
    Module* module = (Module*)calloc(1, sizeof(Module));
    if (module == NULL)
    {
        Log_Write("Module: ERROR. Failed to allocate memory for module MDI8!");
        return module;
    }

    // Инициализация данных модуля
    module->inited = false;
    module->name = config->name;
    module->uniqueName = config->uniqueName;
    module->description = config->description;
    module->i2cAddress = config->address;

    // Чип
    switch (chipName)
    {
        case MDI8_MODULE_CHIP_NONE:
        {
            module->chip = NULL;
            break;
        }
        case MDI8_MODULE_CHIP_PCF8574:
        {
            module->chip = Pcf8574Chip_Create(connection, config->address);
            break;
        }
        default:
        {
            Log_Write("Module: ERROR. Unknown chip (%d) for module (%s)!", chipName, config->name);
            break;
        }
    }

    if (module->chip == NULL)
    {
        free(module);
        module = NULL;
        return module;
    }

    // Входы
    module->inputPinsCount = MDI8_PIN_COUNT;
    module->inputPins = (Module_Pin*)calloc(module->inputPinsCount, sizeof(Module_Pin));
    if (module->inputPins == NULL)
    {
        Log_Write("Module: ERROR. Failed to allocate memory for input pins!");
        free(module->chip);
        module->chip = NULL;
        free(module);
        module = NULL;
        return module;
    }

    // Соответствие выводов модуля выводам микросхемы
    for (int i = 0; i < module->inputPinsCount; i++)
    {
        module->inputPins[i].number = MDI8_PCF8574_PIN_CORRESPONDENCE[i];
        module->inputPins[i].value = 0;
        module->inputPins[i].updated = false;
        module->inputPins[i].read = false;
    }

    // Выходы
    module->outputPinsCount = 0;
    module->outputPins = NULL;

    module->inited = true;

    return module;
}
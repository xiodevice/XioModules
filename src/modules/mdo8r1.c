#include <stdlib.h>
#include "../log.h"
#include "mdo8r1.h"
#include "chips/pcf8574.h"

/// @brief Количество выводов модуля MDO8R1
static int const MDO8R1_PIN_COUNT = 8;
/// @brief Соответствие выводов модуля выводам микросхемы. MDO8R1 (PCF8574)
/// @brief Индексы - выводы модуля;
/// @brief Значения - выводы микросхемы.
static int const MDO8R1_PCF8574_PIN_CORRESPONDENCE[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
/// @brief Соответствие выводов модуля инверсивности сигнала. MDO8R1 (PCF8574)
static bool const MDO8R1_PCF8574_PINS_INVERT[] = { false, false, false, false, false, false, false, false };

// ***** Предоставляемые интерфейсы module_io.h (реализация)*****

Module* MDO8R1Module_Create(Module_Config *config, I2C_Connection *connection, MDO8R1_MODULE_CHIP_ENUM chipName)
{
    // TODO: Ready to test
    Module* module = (Module*)calloc(1, sizeof(Module));
    if (module == NULL)
    {
        LOG(LL_ERROR, ("MDO8R1Module: ERROR. Failed to allocate memory for module MDO8R1!"));
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
        case MDO8R1_MODULE_CHIP_NONE:
        {
            module->chip = NULL;
            break;
        }
        case MDO8R1_MODULE_CHIP_PCF8574:
        {
            module->chip = Pcf8574Chip_Create(connection, config->address);
            break;
        }
        default:
        {
            LOG(LL_ERROR, ("MDO8R1Module: ERROR. Unknown chip (%d) for module %s (%s)!", chipName, config->uniqueName, config->name));
            break;
        }
    }

    if (module->chip == NULL)
    {
        free(module);
        module = NULL;
        return module;
    }

    // Выходы
    module->outputsCount = MDO8R1_PIN_COUNT;
    module->outputs = (Module_Pin*)calloc(module->outputsCount, sizeof(Module_Pin));
    if (module->inputs == NULL)
    {
        LOG(LL_ERROR, ("MDO8R1Module: ERROR. Failed to allocate memory for outputs!"));
        free(module->chip);
        module->chip = NULL;
        free(module);
        module = NULL;
        return module;
    }

    // Соответствие выводов модуля выводам микросхемы
    for (int i = 0; i < module->outputsCount; i++)
    {
        module->outputs[i].number = MDO8R1_PCF8574_PIN_CORRESPONDENCE[i];
        module->outputs[i].inverted = MDO8R1_PCF8574_PINS_INVERT[i];        
        module->outputs[i].data.updated = false;
        module->outputs[i].data.value = 0;
    }
    
    // Входы
    module->inputsCount = 0;
    module->inputs = NULL;
    
    module->inited = true;

    return module;
}
#include <stdlib.h>
#include <stddef.h>
#include "../log.h"
#include "module.h"
#include "mdi8.h"
#include "mdo8r1.h"



// ***** Предоставляемые интерфейсы module_io.h (реализация)*****

Module* Module_Create(Module_Config *config, I2C_Connection *connection)
{    
    // TODO: Ready to test
    if (config == NULL)
        return NULL;

    Module *module = NULL;
    
    if (!config->inited)
        return module;
    
    // MOD: Модули ввода-вывода. Создание модулей
    switch (config->code)
    {
        // Модули дискретного ввода
        case MODULE_NAME_MDI8_PCF8574:
        {
            module = MDI8Module_Create(config, connection, MDI8_MODULE_CHIP_PCF8574);
            if (module == NULL)
            {
                Log_Write("Module: ERROR. Failed to create module MDI8 (%s)!", config->name);
            }

            break;
        }
        case MODULE_NAME_MDI16_PCF8575:
        {
            // TODO: Создание экземпляра модуля MDI16_PCF8575
            break;
        }
        // Модули дискретного вывода
        case MODULE_NAME_MDO8R1_PCF8574:
        {
            // XXX: Остановился 29.06.2024 в 19.00
            module = MDO8R1Module_Create(config, connection, MODULE_NAME_MDO8R1_PCF8574);
            if (module == NULL)
            {
                Log_Write("Module: ERROR. Failed to create module MDO8R1 (%s)!", config->name);
            }

            break;
        }
        case MODULE_NAME_MDO6R2_PCF8574:
        {
            // TODO: Создание экземпляра модуля MDO6R2_PCF8574
            break;
        }
        case MODULE_NAME_MDO6R3_PCF8574:
        {
            // TODO: Создание экземпляра модуля MDO6R3_PCF8574
            break;
        }
        case MODULE_NAME_MDO8T_PCF8574:
        {
            // TODO: Создание экземпляра модуля MDO8T_PCF8574
            break;
        }
        // Модули аналогового ввода
        case MODULE_NAME_MAI4U_ADS1115:
        {
            // TODO: Создание экземпляра модуля MAI4U_ADS1115
            break;
        }
        // Модули аналогового вывода
        case MODULE_NAME_MAO8_PCA9685:
        {
            // TODO: Создание экземпляра модуля MAO8_PCA9685
            break;
        }
        default:
        {
            Log_Write("Module: ERROR. Unknown code (%d) for module (%s)!", config->code, config->name);
            break;
        }
    }

    return module;
}

bool Module_Destroy(Module *module)
{
    bool result = false;

    // TODO: Ready to test
    if (module != NULL)
    {
        if (module->chip != NULL)
        {
            free(module->chip);
            module->chip = NULL;
        }

        if (module->inputs != NULL)
        {
            free(module->inputs);
            module->inputs = NULL;
        }

        if (module->outputs != NULL)
        {
            free(module->outputs);
            module->outputs = NULL;
        }
    }

    result = true;
    return result;
}


bool Module_ReadPins(Module *module)
{
    bool result = false;
    
    // TODO: Ready to test
    if (module == NULL || module->inputs == NULL || 
        module->chip == NULL || module->chip->ReadPins == NULL)
        return result;

    // Чтение всех выводов модуля
    Chip_PinData pinsData[module->inputsCount];
    for (int i = 0; i < module->inputsCount; i++)
    {
        pinsData[i].number = module->inputs[i].number;
        module->inputs[i].data.updated = false;
    }
    
    result = module->chip->ReadPins(module->chip->connection, module->chip->address, pinsData, module->inputsCount);

    if (!result)
        return result;
    
    // Применение данных к выводам экземпляра модуля
    for (int i = 0; i < module->inputsCount; i++)
    {
        if (module->inputs[i].inverted)
            module->inputs[i].data.value = !pinsData[i].value;
        else
            module->inputs[i].data.value = pinsData[i].value;

        module->inputs[i].data.updated = true;
    }

    result = true;
    return result;
}


bool Module_WritePins(Module *module)
{
    bool result = false;

    if (module == NULL || module->outputs == NULL ||
        module->chip == NULL || module->chip->WritePins == NULL)
        return result;

    // Запись всех выводов модуля
    Chip_PinData pinsData[module->outputsCount];
    for (int i = 0; i < module->outputsCount; i++)
    {
        pinsData[i].number = module->outputs[i].number;
        pinsData[i].updated = false;

        if (module->outputs[i].inverted)
            pinsData[i].value = !module->outputs[i].data.value;
        else
            pinsData[i].value = module->outputs[i].data.value;

    }

    // Чтение всех выводов модуля
    result = module->chip->WritePins(module->chip->connection, module->chip->address, pinsData, module->outputsCount);

    if (!result)
        return result;
    
    // Применение данных к выводам экземпляра модуля
    for (int i = 0; i < module->outputsCount; i++)
    {
        if (module->outputs[i].inverted)
            module->outputs[i].data.value = !pinsData[i].value;
        else
            module->outputs[i].data.value = pinsData[i].value;

        module->outputs[i].data.updated = true;
    }

    result = true;
    return result;
}


bool Module_CheckConnection(Module *module)
{
    bool result = false;

    // TODO: Проверка связи с модулем

    //result = true;
    return result;
}


void Module_ShowData(Module *module)
{
    // TODO: Отобразить данные модуля
    if (module == NULL)
        return;

    Log_Write("Module: Module <%s> information: name = %s, description = %s, inited = %d, i2cAddress = 0x%02X (%d)", 
        module->uniqueName, module->name == NULL ? "" : module->name, module->description == NULL ? "" : module->description, 
        module->inited, module->i2cAddress, module->i2cAddress);

    Log_Write("Module: <%s> inputs count = %d:", module->uniqueName, module->inputsCount);
    if (module->inputs != NULL)
    {
        for (int i = 0; i < module->inputsCount; i++)
        {
            Log_Write("Module: <%s> input %d: number = %d, inverted = %d, value = %d", module->uniqueName, i, module->inputs[i].number, module->inputs[i].inverted, module->inputs[i].data.value);
        }
    }
    else
    {
        Log_Write("Module: WARNING. <%s> inputs = NULL!", module->uniqueName);
    }

    Log_Write("Module: <%s> outputs count = %d:", module->uniqueName, module->outputsCount);
    if (module->outputs != NULL)
    {
        for (int i = 0; i < module->outputsCount; i++)
        {
            Log_Write("Module: <%s> output %d: number = %d, inverted = %d, value = %d", module->uniqueName, i, module->outputs[i].number, module->outputs[i].inverted, module->outputs[i].data.value);
        }
    }
    else
    {
        Log_Write("Module: WARNING. <%s> outputs = NULL!", module->uniqueName);
    }

    if (module->chip != NULL)
    {
        Log_Write("Module: <%s> chip info: address = 0x%02X (%d), name = %d", 
        module->uniqueName, module->chip->address, module->chip->address, module->chip->name);
        Log_Write("Module: <%s> chip pins count = %d:", module->uniqueName, module->chip->pinsCount);
        if (module->chip->pinsData != NULL)
        {
            for (int i = 0; i < module->chip->pinsCount; i++)
            {
                Log_Write("Module: <%s> chip pin %d: number = %d, value = %d", module->uniqueName, i, module->chip->pinsData[i].number, module->chip->pinsData[i].value);
            }
        }
        else
        {
            Log_Write("Module: WARNING. <%s> chip pinsData = NULL!", module->uniqueName);
        }

        if (module->chip->connection != NULL)
        {
            if (module->chip->connection->config != NULL)
            {
                Log_Write("Module: <%s> chip I2C connection info: busNumber = %d, frequency = %d Hz, readTimeout = %d Ms",
                    module->uniqueName, module->chip->connection->config->busNumber, 
                    module->chip->connection->config->frequencyHz, module->chip->connection->config->readTimeoutMs);
            }                
            else
            {
                Log_Write("Module: WARNING. <%s> chip I2C connection config = NULL!", module->uniqueName);
            }                
        }
        else
        {
            Log_Write("Module: WARNING. <%s> chip I2C connection = NULL!", module->uniqueName);
        }

        if (module->chip->ReadPins != NULL)
        {
            Log_Write("Module: <%s> chip ReadPins() function inited:", module->uniqueName);
        }
        else
        {
            Log_Write("Module: WARNING. <%s> chip ReadPins() = NULL!", module->uniqueName);
        }

        if (module->chip->WritePins != NULL)
        {
            Log_Write("Module: <%s> chip WritePins() function inited:", module->uniqueName);
        }
        else
        {
            Log_Write("Module: WARNING. <%s> chip WritePins() = NULL!", module->uniqueName);
        }
    }
    else
    {
        Log_Write("Module: WARNING. <%s> chip = NULL", module->uniqueName);
    }    
}
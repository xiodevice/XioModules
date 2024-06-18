#include <stdlib.h>
#include <stddef.h>
#include "../log.h"
#include "module.h"
#include "mdi8.h"



// ***** Предоставляемые интерфейсы module_io.h (реализация)*****

Module* Module_Create(Module_Config* config, I2C_Connection *connection)
{
    if (config == NULL)
        return NULL;

    Module *module = NULL;    
    // TODO: Ready to test
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

            break;
        }
        // Модули дискретного вывода
        case MODULE_NAME_MDO8R1_PCF8574:
        {

            break;
        }
        case MODULE_NAME_MDO6R2_PCF8574:
        {

            break;
        }
        case MODULE_NAME_MDO6R3_PCF8574:
        {

            break;
        }
        case MODULE_NAME_MDO8T_PCF8574:
        {

            break;
        }
        // Модули аналогового ввода
        case MODULE_NAME_MAI4U_ADS1115:
        {

            break;
        }
        // Модули аналогового вывода
        case MODULE_NAME_MAO8_PCA9685:
        {

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


bool Module_Destroy(Module* module)
{
    int result = -1;

    // TODO: Удаление экземпляра модуля

    //result = 0;
    return result;
}


bool Module_ReadPin(Module* module, Module_Pin pin)
{
    bool result = false;

    // TODO: Чтение вывода модуля

    //result = true;
    return result;
}

bool Module_ReadAllPins(Module* module)
{
    bool result = false;
    
    if (module == NULL)
        return result;

    // Чтение всех выводов модуля
    for (int i = 0; i < module->inputPinsCount; i++)
    {
        module->inputPins[i].value = module->chip->ReadPin(module->inputPins[i].number);
    }

    result = true;
    return result;
}


bool Module_WritePin(Module* module, Module_Pin pin)
{
    bool result = false;

    // TODO: Запись вывода модуля

    //result = true;
    return result;
}

bool Module_WriteAllPins(Module* module)
{
    bool result = false;

    // TODO: Запись выводов модуля

    //result = true;
    return result;
}


bool Module_CheckConnection(Module* module)
{
    bool result = false;

    // TODO: Проверка связи с модулем

    //result = true;
    return result;
}


void Module_ShowData(Module* module)
{
    // TODO: Отобразить данные модуля
}
#include <stdlib.h>
#include <stddef.h>
#include "../log.h"
#include "module.h"
#include "mdi8.h"



// ***** Предоставляемые интерфейсы module_io.h (реализация)*****

int Module_InitBus(I2C_Connection *connection)
{
    int result = -1;

    // TODO: Инициализация шины подключения модулей

    //result = 0;
    return result;
}


int Module_CheckBus(I2C_Connection connection)
{
    int result = -1;

    // TODO: Проверка шины подключения модулей

    //result = 0;
    return result;
}


Module* Module_Create(Module_Config* config, I2C_Connection *connection)
{
    if (config == NULL)
        return NULL;

    Module *module = NULL;
    switch (config->code)
    {
        case MODULE_NAME_MDI8_PCF8574:
        {
            module = MDI8Module_Create(config, connection, MDI8_MODULE_CHIP_PCF8574);
            // XXX: Ост 11.06.2024 16:00
            break;
        }
        case MODULE_NAME_MDI16_PCF8575:
        {            
            break;
        }
        default:
        {
            break;
        }
    }

    module->inited = 1;
    return module;
}


int Module_Destroy(Module* module)
{
    int result = -1;

    // TODO: Удаление экземпляра модуля

    //result = 0;
    return result;
}


int Module_ReadPins(Module* module)
{
    int result = -1;
    
    if (module == NULL)
        return result;

    // Чтение всех выводов модуля
    for (int i = 0; i < module->inputPinsCount; i++)
    {
        module->inputPins[i].value = module->chip->ReadPin(module->inputPins[i].number);
    }

    result = 0;
    return result;
}


int Module_WritePins(Module* module)
{
    int result = -1;

    // TODO: Запись выводов модуля

    //result = 0;
    return result;
}


int Module_CheckConnection(Module* module)
{
    int result = -1;

    // TODO: Проверка связи с модулем

    //result = 0;
    return result;
}


void Module_ShowData(Module* module)
{
    // TODO: Отобразить данные модуля
}
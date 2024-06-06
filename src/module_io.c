#include <stdlib.h>
#include <stddef.h>
#include "log.h"
#include "module_io.h"
#include "module_data.h"



// ***** Предоставляемые интерфейсы module_io.h (реализация)*****

Module* Module_Create(Module_Config* config)
{
    // TODO: Создание и инициализация экземпляра модуля

    Module* module = (Module*)malloc(sizeof(Module));
    if (module == NULL)
    {
        Log_Write("ERROR: Failed to create module!");
        return module;
    }

    return module;
}


int Module_Destroy(Module* module)
{
    int result = -1;

    // TODO: Удаление экземпляра модуля

    //result = 0;
    return result;
}


int Module_ReadPins(Module_Pin* readPins, int pinsCount)
{
    int result = -1;

    // TODO: Чтение выводов модуля

    //result = 0;
    return result;
}


int Module_WritePins(Module_Pin* writePins, int pinsCount)
{
    int result = -1;

    // TODO: Запись выводов модуля

    //result = 0;
    return result;
}
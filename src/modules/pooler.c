#include "pooler.h"
#include "../log.h"







// ***** Предоставляемые интерфейсы module.h (реализация)*****

int Pooler_Init(Module *modules, int modulesCount)
{
    bool result = false;

    LOG(LL_INFO, ("Pooler: Initializing..."));
    // TODO: Инициализация опросчика
    
    return result;
}

POOLER_STATE_ENUM Pooler_GetState()
{
    POOLER_STATE_ENUM state = POOLER_STATE_NONE;

    // TODO: Получение состояния опросчика

    return state;
}

bool Pooler_ReadModule(Module *module)
{
    bool result = false;

    // TODO: Чтение данных модуля

    return result;
}

bool Pooler_WriteModule(Module *module)
{
    bool result = false;

    // TODO: Запись данных модуля

    return result;
}

bool Pooler_UpdateModules()
{
    bool result = false;

    // TODO: Опросить модули

    return result;
}
#include "chip.h"



// ***** Реализуемые интерфейсы методов chip.h - Chip (реализация)*****

Chip* Chip_Create(CHIP_NAME_ENUM chipName, I2C_Connection* connection, int address)
{
    Chip* chip = NULL;

    // TODO: Создание экземпляра чипа

    return chip;
}


bool Chip_CheckConnection(Chip* chip)
{
    bool result = false;

    // TODO: Проверка подключения чипа

    //result = true;
    return result;
}


int Chip_ReadPin(Chip* chip, int pinNumber)
{
    int result = -1;

    // TODO: Чтение вывода чипа

    //result = 0;
    return result;
}


bool Chip_WritePin(Chip* chip, int pinNumber, int value)
{
    bool result = false;

    // TODO: Запись вывода чипа

    //result = true;
    return result;
}
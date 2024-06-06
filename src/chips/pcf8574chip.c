#include "pcf8574chip.h"
#include "ichip.h"

// ***** Реализуемые интерфейсы методов ichip.h - Chip (реализация)*****

/// @brief Чтение вывода
/// @param pinNumber Номер вывода
/// @return Значение вывода
static int ReadPin(int pinNumber)
{
    int result = -1;

    // TODO: Чтение вывода

    //result = 0;
    return result;
}

/// @brief Запись вывода
/// @param pinNumber Номер вывода
/// @param value Значение вывода для записи
static void WritePin(int pinNumber, int value)
{
    // TODO: Запись вывода
}


// ***** Предоставляемые интерфейсы pcf8574chip.h (реализация)*****

int Pcf8574Module_Init(iChip* chip)
{
    int result = -1;

    // TODO: Инициализация

    //result = 0;
    return result;
}
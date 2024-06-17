#include <stdlib.h>
#include "pcf8574.h"
#include "../../log.h"

// ***** Реализуемые интерфейсы методов ichip.h - Chip (реализация)*****

/// @brief Чтение вывода
/// @param pinNumber Номер вывода
/// @return Значение вывода. -1 - ошибка чтения
static int ReadPin(int pinNumber)
{
    int result = -1;

    // TODO: Чтение вывода

    //result = 0;
    return result;
}

/// @brief Запись вывода
/// @param pinNumber Номер вывода
/// @param value Значение вывода для записи.
/// @return Результат (0 - успех, -1 - ошибка)
static int WritePin(int pinNumber, int value)
{
    int result = -1;

    // TODO: Запись вывода

    //result = 0;
    return result;
}

/// @brief Проверка соединения
/// @return Результат (0 - успех, -1 - ошибка)
/*static int CheckConnection(I2C_Connection connection)
{
    int result = -1;

    // TODO: Проверка соединения

    //result = 0;
    return result;
}*/


// ***** Предоставляемые интерфейсы pcf8574chip.h (реализация)*****

Chip* Pcf8574Chip_Create(I2C_Connection *connection, int address)
{
    if (connection == NULL)
        return NULL;
    // TODO: Ready to test
    Chip* chip = calloc(1, sizeof(Chip));
    if (chip == NULL)
    {
        Log_Write("PCF8574: ERROR. Failed to allocate memory for chip!");
        return chip;
    }

    chip->name = CHIP_NAME_PCA8574;
    chip->address = address;
    chip->connection = connection;
    chip->ReadPin = ReadPin;
    chip->WritePin = WritePin;

    return chip;
}
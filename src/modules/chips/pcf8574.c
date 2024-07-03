#include <stdlib.h>
#include "pcf8574.h"
#include "../../log.h"


static const int PINS_COUNT = 8;    // Количество выводов

// ***** Реализуемые интерфейсы методов ichip.h - Chip (реализация)*****

/// @brief Чтение выводов
/// @param connection Соединение
/// @param address Адрес микросхемы
/// @param pinsData Массив значений выводов
/// @param pinsCount Количество значений в массиве
/// @return Результат: true - успех, false - ошибка
static bool ReadPins(I2C_Connection *connection, int address, Chip_PinData *pinsData, int pinsCount)
{
    bool result = false;
    // TODO: Ready to test
    if (connection == NULL || connection->opened == false || pinsData == NULL)
    {
        return result;
    }
    
    if (pinsCount < 0 || pinsCount > PINS_COUNT)
    {
        return result;
    }

    for (int i = 0; i < pinsCount; i++)
    {
        pinsData[i].updated = false;
    }

    // Чтение всех восьми выводов (1 байт данных)
    int byteInt = I2C_ReadRegByte(connection, address, 0x00);
    
    if (byteInt < 0)
        return result;

    // Получение значения для каждого вывода
    uint8_t byte = (uint8_t)byteInt;
    for (int i = 0; i < pinsCount; i++)
    {
        pinsData[i].value = (byte >> pinsData[i].number) & 0x01;    // Получение состояния вывода (по порядковому номеру бита)
        pinsData[i].updated = true;
    }

    result = true;
    return result;
}

/// @brief Запись выводов
/// @param connection Соединение
/// @param address Адрес микросхемы
/// @param pins Массив значений выводов
/// @param pinsCount Количество значений в массиве
/// @return Результат: true - успех, false - ошибка
static bool WritePins(I2C_Connection *connection, int address, Chip_PinData *pinsData, int pinsCount)
{
    bool result = false;
    // TODO: Ready to test
    if (connection == NULL || connection->opened == false || pinsData == NULL)
    {
        return result;
    }
    
    if (pinsCount < 0 || pinsCount > PINS_COUNT)
    {
        return result;
    }

    // Сброс флага "обновлено" для всех выводов
    for (int i = 0; i < pinsCount; i++)
    {
        pinsData[i].updated = false;
    }

    // Чтение всех восьми выводов (1 байт данных)
    int byteInt = I2C_ReadRegByte(connection, address, 0x00);
    
    if (byteInt < 0)
        return result;

    uint8_t byte = (uint8_t)byteInt;

    // Изменение данных для каждого вывода
    for (int i = 0; i < pinsCount; i++)
    {
        if (pinsData[i].value == 0)
        {
            byte &= ~(1 << pinsData[i].number);
        }
        else if (pinsData[i].value == 1)
        {
            byte |= (1 << pinsData[i].number);
        }
    }

    // TODO: Запись всех выводов
    bool res = I2C_WriteRegByte(connection, address, 0x00, byte);

    if (res == false)
        return result;

    // Установка флага обновлено для всех выводов
    for (int i = 0; i < pinsCount; i++)
    {
        pinsData[i].updated = true;
    }
    
    result = true;
    return result;
}

// ***** Предоставляемые интерфейсы pcf8574chip.h (реализация)*****

Chip* Pcf8574Chip_Create(I2C_Connection *connection, int address)
{
    if (connection == NULL)
        return NULL;
    
    Chip* chip = calloc(1, sizeof(Chip));
    if (chip == NULL)
    {
        Log_Write("PCF8574: ERROR. Failed to allocate memory for chip!");
        return chip;
    }

    chip->name = CHIP_NAME_PCA8574;
    chip->address = address;
    chip->connection = connection;
    
    // Инициализация выводов
    chip->pinsCount = PINS_COUNT;
    chip->pinsData = (Chip_PinData*)calloc(chip->pinsCount, sizeof(Chip_PinData));
    if (chip->pinsData == NULL)
    {
        Log_Write("PCF8574: ERROR. Failed to allocate memory for pins!");
        free(chip);
        chip = NULL;
        return chip;
    }

    for (int i = 0; i < chip->pinsCount; i++)
    {
        chip->pinsData[i].updated = false;
        chip->pinsData[i].number = i;
        chip->pinsData[i].value = 0;
    }

    // Инициализация функций
    chip->ReadPins = ReadPins;
    chip->WritePins = WritePins;

    return chip;
}
/*
Библиотека I2C.
*/

#ifndef IC2_H
#define IC2_H

#include <stdbool.h>
#include <stdint.h>

// Конфигурация I2C
typedef struct
{
    uint8_t busNumber;          // Номер шины в системе
    int frequencyHz;            // Частота, Гц
    int readTimeoutMs;          // Таймаут при чтении, мс
} I2C_Config;

// Подключение I2C
typedef struct
{
    bool opened;            // Подключение открыто
    int fd;                 // Файловый дескриптор I2C
    I2C_Config *config;     // Конфигурация
} I2C_Connection;

/// @brief Создать экземпляр I2C подключения. Подключение остается открытым
/// @param config Конфигурация
/// @return Экземпляр подключения
I2C_Connection* I2C_CreateConnection(I2C_Config *config);

/// @brief Удалить экземпляр I2C подключения. Подключение закрывается
/// @param connection Подключение
/// @return Результат: true - успешно, false - ошибка
bool I2C_DestroyConnection(I2C_Connection *connection);

/// @brief Проверить связь с устройством на шине I2C по адресу
/// @param connection Подключение
/// @param address Адрес устройства
/// @return Результат: true - успешно, false - ошибка
bool I2C_CheckConnectionAddress(I2C_Connection *connection, uint16_t address);

/// @brief Установить частоту I2C
/// @param connection Подключение
/// @param frequency Частота
/// @return Результат: true - успешно, false - ошибка
bool I2C_SetFrequency(I2C_Connection *connection, int frequency);

/// @brief Установить таймаут при чтении
/// @param connection Подключение
/// @param readTimeoutMs Таймаут, мс
/// @return Результат: true - успешно, false - ошибка
bool I2C_SetReadTimeout(I2C_Connection *connection, int readTimeoutMs);

/// @brief Прочитать байт по I2C
/// @param connection Подключение
/// @param address Адрес устройства на шине
/// @param regAddress Адрес регистра
/// @param regValue Значение регистра
/// @return Числовое значение в 1 байт (от 0x00 до 0xff). Отрицательное значение - ошибка
int I2C_ReadRegByte(I2C_Connection *connection, uint16_t address, uint16_t regAddress);

/// @brief Прочитать машинное слово по I2C
/// @param connection Подключение
/// @param address Адрес устройства на шине
/// @param regAddress Адрес регистра
/// @param regsValue Значение регистров
/// @return Числовое значение в 2 байта (от 0x0000 до 0xffff). Отрицательное значение - ошибка
int I2C_ReadRegWord(I2C_Connection *connection, uint16_t address, uint16_t regAddress);

/// @brief Чтение данных из I2C
/// @param connection Подключение
/// @param address Адрес устройства на шине
/// @param buffer Буфер данных (отпавляемые)
/// @param len Длина буфера данных
/// @return Результат операции (Код ошибки)
//int I2C_Read(I2C_Connection *connection, uint16_t address, uint8_t* buffer, int len);

/// @brief Запись одного байта в I2C
/// @param connection Подключение
/// @param address Адрес устройства на шине
/// @param regAddress Адрес регистра
/// @param regValue Значение регистра
/// @return Результат: true - успешно, false - ошибка
bool I2C_WriteRegByte(I2C_Connection *connection, uint16_t address, uint16_t regAddress, uint8_t regValue);

/// @brief Запись машинного слова в I2C
/// @param connection Подключение
/// @param address Адрес устройства на шине
/// @param regAddress Адрес регистра
/// @param regValue Значение регистров
/// @return Результат: true - успешно, false - ошибка
bool I2C_WriteRegWord(I2C_Connection *connection, uint16_t address, uint16_t regAddress, uint16_t regValue);

/// @brief Запись данных в I2C
/// @param connection Подключение
/// @param address Адрес слейва на шине
/// @param buffer Буфер данных для записи
/// @param len Длина буфера данных 
/// @return Результат: true - успешно, false - ошибка
//bool I2C_Write(I2C_Connection *connection, uint16_t address, uint8_t* buffer, int len);


#endif
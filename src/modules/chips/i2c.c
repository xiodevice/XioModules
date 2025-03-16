#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "../../log.h"
#include "i2c.h"

// TEMP: Закомментированы библиотеки i2c для Linux

#ifdef __linux__

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

/// @brief Системные подключения I2C
static const char* I2C_LINUX_DEVICES[4] = {
    "/dev/i2c-0",
    "/dev/i2c-1",
    "/dev/i2c-2",
    "/dev/i2c-3"
};

#endif

/// @brief Частота I2C по умолчанию
static const int I2C_DEFAULT_FREQUENCY = 100000;

/// @brief Время ожидания при чтении, в миллисекундах
static const int I2C_DEFAULT_READ_TIMEOUT_MS = 100;

/// @brief Прочитать из слейва I2C данные (max 2 bytes)
/// @param connection Подключение
/// @param address Адрес устройства на шине
/// @param regAddress Стартовый регистр
/// @param regsCount Количество регистров (если более 2, то считаем, что это машинное слово)
/// @return Числовое значение в байтах (от 0x0000 до 0xffff). Отрицательное значение - ошибка
static int I2C_Read(I2C_Connection *connection, uint16_t address, uint16_t regAddress, uint8_t regsCount)
{
    int result = -1;
    // TODO: Ready to test
    if (connection == NULL)
        return result;

    if (regsCount > 2)
        regsCount = 2;

#ifdef __linux__
    // Выбор устройства на шине
    int res = ioctl(connection->fd, I2C_SLAVE, address);
    if (res < 0) 
    {
        LOG(LL_ERROR, ("I2C: ERROR. Failed to acquire bus access and/or find the slave %d (0x%02x)!", address, address));
        return result;
    }

    // Выбор стартового регистора для чтения
    res = write(connection->fd, &regAddress, 1);
    if (res != regsCount)
    {
        LOG(LL_ERROR, ("I2C: ERROR. Faild to use start register 0x%02x (count = %d) for slave %d (0x%02x)!", 
            regAddress, regsCount, address, address));
        return result;
    }

    // Чтение данных из регистра
    res = read(connection->fd, &result, regsCount);
    if (res != regsCount)
    {
        LOG(LL_ERROR, ("I2C: ERROR. The slave %d (0x%02x) is not responding!", address, address));
        result = -1;
        return result;
    }
#else
    LOG(LL_ERROR, ("I2C: ERROR. Reading data from I2C is not supported on this platform!"));
    return result;
#endif

    return result;
}


/// @brief Записать в слейв I2C данные
/// @param connection Подключение
/// @param address Адрес устройства на шине
/// @param regAddress Адрес стартового регистра для записи
/// @param data Данные для записи
/// @param len Длина данных
/// @return Результат: true - успех, false - ошибка
static bool I2C_Write(I2C_Connection *connection, uint16_t address, uint16_t regAddress, uint8_t* data, int len)
{
    bool result = false;
    if (connection == NULL)
        return result;
    // TODO: Ready to test
#ifdef __linux__
    // Создание буфера для записи
    uint8_t buffer[len + 1]; // Первый байт - адрес регистра
    for (size_t i = 0; i < len; i++) {
        buffer[i + 1] = data[i];
    }

    // Выбор устройства на шине
    int res = ioctl(connection->fd, I2C_SLAVE, address);
    if (res < 0) 
    {
        LOG(LL_ERROR, ("I2C: ERROR. Failed to acquire bus access and/or find the slave %d (0x%02x)!", address, address));
        return result;
    }

    // Запись данных в регистр
    res = write(connection->fd, buffer, len + 1);
    if (res != len)
    {
        LOG(LL_ERROR, ("I2C: ERROR. Faild to use start register 0x%02x (count = %d) for slave %d (0x%02x)!", 
            regAddress, len, address, address));
        return result;
    }
#else
    LOG(LL_ERROR, ("I2C: ERROR. Writing data to I2C is not supported on this platform!"));
    return result;
#endif

    result = true;
    return result;
}


// ***** Предоставляемые интерфейсы i2c.h (реализация)*****

I2C_Connection* I2C_CreateConnection(I2C_Config *config)
{
    I2C_Connection* connection = NULL;
    // TODO: Ready to test
    if (config == NULL)
        return connection;

#ifdef __linux__
    connection = calloc(1, sizeof(I2C_Connection));

    if (connection == NULL)
    {
        LOG(LL_ERROR, ("I2C: ERROR. Failed to allocate memory for I2C connection instance!"));
        return connection;
    }

    connection->opened = false;

    // Открытие устройства на шине
    connection->fd = open(I2C_LINUX_DEVICES[config->busNumber], O_RDWR);
    if (connection->fd < 0)
    {
        LOG(LL_ERROR, ("I2C: ERROR. Failed to open I2C bus! It may be missing, unavailable or busy!"));
        I2C_DestroyConnection(connection);
        return connection;
    }   

    // Установка частоты
    bool res = I2C_SetFrequency(connection, config->frequencyHz);
    if (res == false)
    {
        LOG(LL_WARN, ("I2C: WARNING. Failed to set new frequency (%d Hz) of I2C!", config->frequencyHz));
        //I2C_DestroyConnection(connection);
        //return connection;
    }

    // Установка таймаута чтения
    res = I2C_SetReadTimeout(connection, config->readTimeoutMs);
    if (res == false)
    {
        LOG(LL_WARN, ("I2C: WARNING. Failed to set new read timeout (%d ms) of I2C!", config->readTimeoutMs));
        //I2C_DestroyConnection(connection);
        //return connection;
    }

    connection->opened = true;    
#else
    LOG(LL_WARN, ("I2C: WARNING. Creating an I2C connection is not supported on this platform!"));
    return connection;
#endif

    connection->opened = true;

    return connection;
}


bool I2C_DestroyConnection(I2C_Connection *connection)
{
    int result = false;
    // TODO: Ready to test
    if (connection != NULL)
    {
        if (connection->opened)
        {
            close(connection->fd);
            free(connection);
            connection = NULL;
        }
    }

    result = true;
    return result;
}


bool I2C_CheckConnectionAddress(I2C_Connection *connection, uint16_t address)
{
    bool result = false;
    // TODO: Ready to test
    if (connection == NULL)
        return result;    

#ifdef __linux__
    // Проверка наличия устройства на шине (через установку адреса устройства)
    int res = ioctl(connection->fd, I2C_SLAVE, address);
    if (res < 0) 
    {
        LOG(LL_ERROR, ("I2C: ERROR. Failed to acquire bus access and/or find the slave %d (0x%02x)!", address, address));
        return result;
    }

    // Попробуем выполнить чтение одного байта для проверки устройства
    char buffer;
    res = read(connection->fd, &buffer, 1);
    if (res != 1)
    {
        LOG(LL_ERROR, ("I2C: ERROR. The slave %d (0x%02x) is not responding!", address, address));
        return result;
    }

#else
    LOG(LL_ERROR, ("I2C: ERROR. Checking an I2C connection is not supported on this platform!"));
    return result;
#endif

    result = true;
    return result;
}


bool I2C_SetFrequency(I2C_Connection *connection, int frequencyHz)
{
    bool result = false;
    // TODO: Ready to test
    if (connection == NULL)
        return result;

    if (frequencyHz <= 0)
    {
        frequencyHz = I2C_DEFAULT_FREQUENCY;
    }

#ifdef __linux__
    // !!! Не уверен в правильности и последовательности действий !!!
    // Установка частоты
    /*int i2cFrequencyCode = i2c_frequency_to_code(frequencyHz);  // Функция для преобразования частоты в код частоты
    int res = ioctl(connection->fd, I2C_FREQ, i2cFrequencyCode);
    if (res < 0)
    {
        LOG(LL_ERROR, ("I2C: ERROR. Failed to set I2C frequency to %d Hz!", frequencyHz));
    }*/
    LOG(LL_ERROR, ("I2C: ERROR. Setting I2C bus frequency is not supported from code!"));
#else
    LOG(LL_ERROR, ("I2C: ERROR. Setting I2C bus frequency is not supported on this platform!"));
    return result;
#endif

    result = true;
    return result;
}


bool I2C_SetReadTimeout(I2C_Connection *connection, int readTimeoutMs)
{
    bool result = false;
    // TODO: Ready to test
    if (connection == NULL)
        return result;

    if (readTimeoutMs <= 0)
        readTimeoutMs = I2C_DEFAULT_READ_TIMEOUT_MS;

#ifdef __linux__
    // !!! Не уверен в правильности и последовательности действий !!!
    // Установка таймаута чтения
    int res = ioctl(connection->fd, I2C_TIMEOUT, readTimeoutMs);
    if (res < 0)
    {
        LOG(LL_ERROR, ("I2C: ERROR. Failed to set I2C read timeout to %d ms!", readTimeoutMs));
    }
#else
    LOG(LL_ERROR, ("I2C: ERROR. Setting I2C bus read timeout is not supported on this platform!"));
    return result;
#endif

    result = true;
    return result;
}


int I2C_ReadRegByte(I2C_Connection *connection, uint16_t address, uint16_t regAddress)
{
    int result = -1;
    // TODO: Ready to test
    if (connection == NULL)
        return result;
    
    result = I2C_Read(connection, address, regAddress, 1);

    return result;
}


int I2C_ReadRegWord(I2C_Connection *connection, uint16_t address, uint16_t regAddress)
{
    int result = -1;
    // TODO: Ready to test
    if (connection == NULL)
        return result;
    
    result = I2C_Read(connection, address, regAddress, 2);

    return result;
}


bool I2C_WriteRegByte(I2C_Connection *connection, uint16_t address, uint16_t regAddress, uint8_t regValue)
{
    bool result = false;
    // TODO: Ready to test
    if (connection == NULL)
        return result;

    result = I2C_Write(connection, address, regAddress, &regValue, 1);

    return result;
}


bool I2C_WriteRegWord(I2C_Connection *connection, uint16_t address, uint16_t regAddress, uint16_t regValue)
{
    bool result = false;
    uint8_t data[2];
    // TODO: Ready to test
    if (connection == NULL)
        return result;

    data[0] = (regValue >> 8) & 0xff;
    data[1] = regValue & 0xff;

    result = I2C_Write(connection, address, regAddress, data, 2);

    return result;
}
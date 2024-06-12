#include "i2c.h"



// ***** Предоставляемые интерфейсы i2c.h (реализация)*****

I2C_Connection* I2C_CreateConnection(I2C_Config config)
{
    I2C_Connection* connection = NULL;

    // TODO: Создание экземпляра подключения I2C

    return connection;
}


bool I2C_SetFrequency(I2C_Connection connection, int frequency)
{
    bool result = false;

    // TODO: Установка частоты I2C подключения

    //result = true;
    return result;
}


int I2C_ReadRegByte(I2C_Connection connection, uint16_t address, uint16_t regAddress)
{
    int result = -1;

    // TODO: Чтение одного байта по I2C

    //result = 0;
    return result;
}


int I2C_ReadReqWord(I2C_Connection connection, uint16_t address, uint16_t regAddress)
{
    int result = -1;

    // TODO: Чтение машинного слова по I2C

    //result = 0;
    return result;
}


int I2C_Read(I2C_Connection connection, uint16_t address, uint8_t* buffer, int len)
{
    int result = -1;

    // TODO: Чтение данных по I2C

    //result = 0;
    return result;
}


bool I2C_WriteRegByte(I2C_Connection connection, uint16_t address, uint16_t regAddress, uint8_t regValue)
{
    int result = -1;

    // TODO: Запись одного байта в I2C

    //result = 0;
    return result;
}


bool I2C_WriteReqWord(I2C_Connection connection, uint16_t address, uint16_t regAddress, uint16_t regValue)
{
    int result = -1;

    // TODO: Запись машинного слова в I2C

    //result = 0;
    return result;
}


bool I2C_Write(I2C_Connection connection, uint16_t address, uint8_t* buffer, int len)
{
    int result = -1;

    // TODO: Запись данных в I2C

    //result = 0;
    return result;
}


bool I2C_DestroyConnection(I2C_Connection connection)
{
    int result = -1;

    // TODO: Удалить экземпляр I2C подключения. Подключение закрывается

    //result = 0;
    return result;
}


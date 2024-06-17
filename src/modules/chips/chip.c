#include "chip.h"
#include "../../log.h"
#include "pcf8574.h"


// ***** Реализуемые интерфейсы методов chip.h - Chip (реализация)*****

// MOD: Создание экземпляров чипов
Chip* Chip_Create(CHIP_NAME_ENUM chipName, I2C_Connection* connection, int address)
{
    Chip* chip = NULL;
    if (connection == NULL)
        return chip;
    // TODO: Ready to test
    // Создание экземпляра чипа в зависимости от его наименования
    switch (chipName)
    {
        case CHIP_NAME_PCA8574:
        {
            chip = Pcf8574Chip_Create(connection, address);
            if (chip == NULL)
            {
                Log_Write("Chip: ERROR. Failed to create chip PCF8574!");
                return chip;
            }
            break;
        }        
        case CHIP_NAME_PCA8575:
        {
            // TEMP: Раскомментировать для создания чипа PCF8575
            //chip = Pcf8575Chip_Create(connection, address);
            break;
        }
        case CHIP_NAME_ADS1115:
        {
            // TEMP: Раскомментировать для создания чипа ADS1115
            //chip = Ads1115Chip_Create(connection, address);
            break;
        }
        case CHIP_NAME_PCA9685:
        {
            // TEMP: Раскомментировать для создания чипа PCA9685
            //chip = Pca9685Chip_Create(connection, address);
            break;
        }
        default:
        {
            Log_Write("Chip: ERROR. Unknown chip name!");
            break;
        }
    }

    if (chip == NULL)
        return chip;

    // Проверить подключение чипа
    bool result = I2C_CheckConnectionAddress(chip->connection, chip->address);
    if (!result)
    {
        Chip_Destroy(chip);
        return chip;
    }

    return chip;
}


bool Chip_Destroy(Chip* chip)
{
    bool result = false;

    // TODO: Удаление экземпляра чипа

    //result = true;
    return result;
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
/*
Модуль ввода-вывода на шине I2C.
Собирательный/связующий модуль, для удобства работы с микросхемами ввода/вывода.
Обертка над микросхемами.
Содержит в себе основные функции для работы с модулями ввода-вывода.
*/

#ifndef MODULE_IO_H
#define MODULE_IO_H

#include <stdint.h>
#include <stdbool.h>
#include "chips/ichip.h"
#include "chips/i2c.h"

// Состояние модуля (Enum)
typedef enum
{
    MODULE_STATE_NONE,      // Не инициализирован
    MODULE_STATE_INIT,      // Инициализация
    MODULE_STATE_STANDBY,   // Ожидание
    MODULE_STATE_READING,   // Чтение
    MODULE_STATE_WRITING,   // Запись
    MODULE_STATE_ERROR      // Ошибка
} MODULE_STATE_ENUM;

/*
MOD: Модули ввода-вывода (Enum)
КОД: ABCDE, где A - тип модуля, B - вариант чипа, CD - количество входов-выходов, E - вариант/версия модуля по особенностям
(Описание составляющих кода в документации к модулям ввода-вывода)
*/
typedef enum
{
    MODULE_NAME_NONE,

    // Модули с дискретными входами (стартовый код 10000)
    MODULE_NAME_MDI = 10000,            // Модуль MDI (Без привязки к чипу)
    MODULE_NAME_MDI8_PCF8574 = 11081,   // Модуль MDI-8 (чип PCF8574), универсальные дискретные входы
    MODULE_NAME_MDI16_PCF8575 = 12161,  // Модуль MDI-16 (чип PCF8575)

    // Модули с дискретными выходами (стартовый код 20000)
    MODULE_NAME_MDO = 20000,            // Модуль MDO (Без привязки к чипу)
    MODULE_NAME_MDO8R1_PCF8574 = 21081, // Модуль MDO-8R1 (чип PCF8574), однопоз. реле
    MODULE_NAME_MDO6R2_PCF8574 = 21062, // Модуль MDO-6R2 (чип PCF8574), двупоз. реле
    MODULE_NAME_MDO6R3_PCF8574 = 21063, // Модуль MDO-6R3 (чип PCF8574), разнотипные реле
    MODULE_NAME_MDO8T_PCF8574 = 21085,  // Модуль MDO-8T (чип PCF8574), транзистор
    //MODULE_NAME_MDO10R3_PCF8575 = 22103,// Модуль MDO-10R3 (чип PCF8575), разнотипные реле
    
    // Модули с аналоговыми входами (стартовый код 30000)
    MODULE_NAME_MAI = 30000,            // Модуль MAI (Без привязки к чипу)
    MODULE_NAME_MAI4U_ADS1115 = 31040,  // Модуль MAI-4U (чип ADS1115), универсальный аналоговые входы
    
    // Модули с аналоговыми выходами (стартовый код 40000)
    MODULE_NAME_MAO = 40000,            // Модуль MAO (Без привязки к чипу)
    MODULE_NAME_MAO8_PCA9685 = 41080    // Модуль MAO-8 (чип PCA9685)
} MODULE_NAME_ENUM;

// Тип вывода/ножки (struct)
/*typedef enum
{
    MODULE_PIN_TYPE_NONE,
    MODULE_PIN_TYPE_DI,
    MODULE_PIN_TYPE_DO,
    MODULE_PIN_TYPE_AI,
    MODULE_PIN_TYPE_AO
} MODULE_PIN_TYPE;*/

// Режим работы вывода/ножки (Enum)
/*typedef enum
{
    MODULE_PIN_MODE_NONE,
    MODULE_PIN_MODE_INPUT,
    MODULE_PIN_MODE_OUTPUT
} MODULE_PIN_MODE_ENUM;*/

// Данные вывода модуля для чтения/записи (struct)
typedef struct
{
    bool updated;               // Обновлено (флаг состояния "получено" для значения)
    int value;                  // Значение ( <0 - значение не получено )
} Module_PinData;

// Вывод/ножка модуля (struct)
typedef struct
{
    bool inverted;              // Инверсия вывода
    int number;                 // Номер вывода (модуля)
    Module_PinData data;        // Данные для чтения/записи
} Module_Pin;

// Модуль ввода/вывода (struct)
typedef struct
{
    bool inited;                            // Инициализация пройдена
    int i2cAddress;                         // Адрес I2C
    int inputsCount;                        // Количество входов
    int outputsCount;                       // Количество выходов
    MODULE_STATE_ENUM state;                // Состояние
    Module_Pin *inputs;                     // Выводы входов
    Module_Pin *outputs;                    // Выводы выходов
    Chip *chip;                             // Микросхема
    char *uniqueName;                       // Уникальное имя модуля (будет использоваться при формировании MQTT топика)
    char *name;                             // Пользовательское имя модуля
    char *description;                      // Описание модуля
} Module;

// Конфигурация модуля (struct)
typedef struct
{
    bool inited;                // Инициализация пройдена
    int code;                   // Код модуля
    int address;                // Адрес модуля
    char *uniqueName;           // Уникальное имя модуля (будет использоваться при формировании MQTT топика)
    char *name;                 // Пользовательское имя модуля
    char *description;          // Описание модуля
} Module_Config;


/// @brief Создать экземпляр модуля
/// @param config Конфигурация модуля
/// @param connection Подключение
/// @return Экземпляр модуля. NULL - ошибка создания
Module* Module_Create(Module_Config *config, I2C_Connection *connection);

/// @brief Удалить экземпляр модуля
/// @param module Модуль
/// @return Результат: true - успешно; false - ошибка
bool Module_Destroy(Module *module);


/// @brief Прочитать все выводы модуля
/// @param readPins Модуль
/// @return Результат: true - успешно; false - ошибка
bool Module_ReadPins(Module *module);

/// @brief Запиcать все выводы модуля
/// @param writePins Модуль
/// @return Результат: true - успешно; false - ошибка
bool Module_WritePins(Module *module);


/// @brief Проверить связь с модулем
/// @param module Модуль
/// @return Результат: true - успешно; false - ошибка
bool Module_CheckConnection(Module *module);

/// @brief Отобразить данные модуля
/// @param module Модуль
void Module_ShowData(Module *module);

#endif
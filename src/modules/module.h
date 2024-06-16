/*
Модуль ввода-вывода на шине I2C.
Собирательный/связующий модуль, высший уровень абстракции, для удобства
работы с микросхемами ввода/вывода.
*/

#ifndef MODULE_IO_H
#define MODULE_IO_H

#include <stdint.h>
#include <stdbool.h>
#include "chips/chip.h"
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

// MOD: Модули ввода/вывода (Enum)
// КОД: XYZ, где X - тип модуля, Y - тип чипа, Z - модель модуля
typedef enum
{
    MODULE_NAME_NONE,
    // Модули с дискретными входами (стартовый код 100)
    MODULE_NAME_MDI = 100,              // Модуль MDI (Без привязки к чипу)
    MODULE_NAME_MDI8_PCF8574 = 101,     // Модуль MDI-8 (чип PCF8574)
    MODULE_NAME_MDI16_PCF8575 = 111,    // Модуль MDI-16 (чип PCF8575)
    // Модули с дискретными выходами (стартовый код 200)
    MODULE_NAME_MDO = 200,      // Модуль MDO (Без привязки к чипу)
    MODULE_NAME_MDO8R_PCF8574 = 201,    // Модуль MDO-8R (чип PCF8574)
    MODULE_NAME_MDO6R_PCF8575 = 202,    // Модуль MDO-6R (чип PCF8574)
    MODULE_NAME_MDO10R_PCF8575 = 211,   // Модуль MDO-10R (чип PCF8575)
    // Модули с аналоговыми входами (стартовый код 300)
    MODULE_NAME_MAI = 300,              // Модуль MAI (Без привязки к чипу)
    MODULE_NAME_MAI4U_ADS1115 = 301,    // Модуль MAI-4U (чип ADS1115)
    // Модули с аналоговыми выходами (стартовый код 400)
    MODULE_NAME_MAO = 400,              // Модуль MAO (Без привязки к чипу)
    MODULE_NAME_MAO8_PCA9685 = 401      // Модуль MAO-8 (чип PCA9685)
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
typedef enum
{
    MODULE_PIN_MODE_NONE,
    MODULE_PIN_MODE_INPUT,
    MODULE_PIN_MODE_OUTPUT
} MODULE_PIN_MODE_ENUM;

// Вывод/ножка модуля (struct)
typedef struct
{
    bool updated;               // Обновлено
    int number;                 // Номер вывода (на клеммнике)
    int value;                  // Значение ( <0 - значение не получено )
    //MODULE_PIN_TYPE type;       // Тип вывода
    //MODULE_PIN_MODE_ENUM mode;  // Режим работы
} Module_Pin;

// Модуль ввода/вывода (struct)
typedef struct
{
    bool inited;                            // Инициализация пройдена
    int i2cAddress;                         // Адрес I2C
    int inputPinsCount;                     // Количество входов
    int outputPinsCount;                    // Количество выходов
    MODULE_STATE_ENUM state;                // Состояние
    I2C_Connection *connection;             // I2C подключение
    Module_Pin* inputPins;                  // Входы
    Module_Pin* outputPins;                 // Выходы
    Chip* chip;                             // Микросхема
    char* uniqueName;                       // Уникальное имя модуля (будет использоваться при формировании MQTT топика)
    char* name;                             // Пользовательское имя модуля
    char* description;                      // Описание модуля
    bool (*ReadPin)(Module_Pin*);  // Прочитать вход модуля. Возвращает: true - прочитано, false - ошибка
    bool (*WritePin)(Module_Pin*); // Записать выход модуля. Возвращает: true - записано, false - ошибка
} Module;

// Конфигурация модуля (struct)
typedef struct
{
    int code;                   // Код модуля
    int address;                // Адрес модуля
    char* uniqueName;           // Уникальное имя модуля (будет использоваться при формировании MQTT топика)
    char* name;                 // Пользовательское имя модуля
    char* description;          // Описание модуля
} Module_Config;


/// @brief Проинициализировать шину подключения модулей
/// @param connection Подключение
/// @return Результат операции (Код ошибки)
int Module_InitBus(I2C_Connection *connection);

/// @brief Проверить шину подключения модулей
/// @param connection Подключение
/// @return Результат
int Module_CheckBus(I2C_Connection connection);


/// @brief Создать экземпляр модуля
/// @param config Конфигурация модуля
/// @param connection Подключение
/// @return Экземпляр модуля. NULL - ошибка создания
Module* Module_Create(Module_Config* config, I2C_Connection *connection);

/// @brief Удалить экземпляр модуля
/// @param module Модуль
/// @return Результат операции (Код ошибки)
int Module_Destroy(Module* module);

/// @brief Прочитать выводы модуля
/// @param readPins Модуль
/// @return Результат операции (Код ошибки)
int Module_ReadPins(Module* module);

/// @brief Запиcать выводы модуля
/// @param writePins Модуль
/// @return Результат операции (Код ошибки)
int Module_WritePins(Module* module);

/// @brief Проверить связь с модулем
/// @param module Модуль
/// @return Результат операции (Код ошибки), 0 - есть; -1 - нет
int Module_CheckConnection(Module* module);


/// @brief Отобразить данные модуля
/// @param module Модуль
void Module_ShowData(Module* module);

#endif
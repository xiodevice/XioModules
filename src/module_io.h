/*
Модуль ввода-вывода на шине I2C.
Собирательный/связующий модуль, высший уровень абстракции, для удобства
работы с микросхемами ввода/вывода.
*/

#ifndef MODULE_IO_H
#define MODULE_IO_H

//#include <stdint.h>
#include "chips/ichip.h"

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
    MODULE_NAME_MDI = 100,      // Модуль MDI (Без привязки к чипу)
    MODULE_NAME_MDI8 = 101,     // Модуль MDI-8 (чип PCF8574)
    MODULE_NAME_MDI16 = 111,    // Модуль MDI-16 (чип PCF8575)
    // Модули с дискретными выходами (стартовый код 200)
    MODULE_NAME_MDO = 200,      // Модуль MDO (Без привязки к чипу)
    MODULE_NAME_MDO8R = 201,    // Модуль MDO-8R (чип PCF8574)
    MODULE_NAME_MDO6R = 202,    // Модуль MDO-6R (чип PCF8574)
    MODULE_NAME_MDO10R = 211,   // Модуль MDO-10R (чип PCF8575)
    // Модули с аналоговыми входами (стартовый код 300)
    MODULE_NAME_MAI = 300,      // Модуль MAI (Без привязки к чипу)
    MODULE_NAME_MAI4U = 301,    // Модуль MAI-4U (чип ADS1115)
    // Модули с аналоговыми выходами (стартовый код 400)
    MODULE_NAME_MAO = 400,      // Модуль MAO (Без привязки к чипу)
    MODULE_NAME_MAO8 = 401      // Модуль MAO-8 (чип PCA9685)
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

// I2C Подключение (struct)
typedef struct
{
    int i2cNumber;              // Номер группы выводов I2C микропроцессора
    int sdaPin;                 // Номер пина SDA
    int sclPin;                 // Номер пина SCL
    char* i2cAddress;           // Адрес I2C слейва
    int connectionFailed;       // Связь отсутствует
} Module_Connection;

// Вывод/ножка модуля (struct)
typedef struct
{
    int number;                 // Номер вывода (на клеммнике)
    int value;                  // Значение
    //MODULE_PIN_TYPE type;       // Тип вывода
    MODULE_PIN_MODE_ENUM mode;  // Режим работы
} Module_Pin;

// Модуль ввода/вывода (struct)
typedef struct
{
    int inited;                         // Инициализация пройдена
    int inputPinsCount;                 // Количество входов
    int outputPinsCount;                // Количество выходов
    MODULE_STATE_ENUM state;            // Состояние
    Module_Connection *connection;  // I2C подключение
    Module_Pin* inputPins;               // Входы
    Module_Pin* outputPins;              // Выходы
    iChip* chip;                         // Микросхема
} Module;

// Конфигурация модуля (struct)
typedef struct
{
    int code;                   // Код модуля
    int address;                // Адрес модуля
    char* unique_name;          // Уникальное имя модуля (будет использоваться при формировании MQTT топика)
    char* name;                 // Пользовательское имя модуля
    char* description;          // Описание модуля
} Module_Config;


/// @brief Создать экземпляр модуля
/// @param config Конфигурация
/// @return Экземпляр модуля
Module* Module_Create(Module_Config* config);

/// @brief Удалить экземпляр модуля
/// @param module Модуль
/// @return Результат операции (Код ошибки)
int Module_Destroy(Module* module);

/// @brief Прочитать выводы модуля
/// @param readPins Выводы для чтения
/// @param pinsCount Количество выводов
/// @return Результат операции (Код ошибки)
int Module_ReadPins(Module_Pin* readPins, int pinsCount);

/// @brief Запиcать выводы модуля
/// @param writePins Выводы для записи
/// @param pinsCount Количество выводов
/// @return Результат операции (Код ошибки)
int Module_WritePins(Module_Pin* writePins, int pinsCount);


#endif
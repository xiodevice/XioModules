#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "system.h"
#include "log.h"
#include "configurator.h"
#include "modules/module.h"
#include "modules/chips/i2c.h"
#include "mqtt.h"

#define TEST_MODE

/// @brief Подключение модулей
static I2C_Config connectionConfig = 
{
    .unitNumber = 0,
    .frequencyHz = 100000,
    .readTimeoutMs = 100
};

// MOD: Системные названия типов плат (изменять количество элементов вместе с SYSTEM_BOARD_ENUM)
static char* SYSTEM_BOARDS_NAMES[] = 
{
    "Unknown",
    "NanoPi-NEO-Core",                  // NanoPi NEO Core (2)
    "Raspberry Pi",                     // Raspberry Pi 2 - 5 (все версии)
    "Raspberry Pi Compute Module 4",    // Название не проверено
    "Orange Pi",                        // Название не проверено
    "BIGTREETECH CB1"                   // Название не проверено
    /*Количество названий должно совпадать с 
    количеством перечисления SYSTEM_BOARD_ENUM*/
};

static SYSTEM_BOARD_ENUM board = SYSTEM_BOARD_UNKNOWN;  // Тип платы

static I2C_Connection *connection;      // Подключение I2C

static Config config;
static Module **modules;                // Модули ввода-вывода (указатель на массив указателей)
static int modulesCount;                // Количество модулей (в работе)

static pthread_t threadIndication;      // Поток индикации
static pthread_t threadModules;         // Поток для опроса модулей
static pthread_t threadMqtt;            // Поток для обмена данными с MQTT

static int indicationThreadToRun = 1;   // Флаг для завершения потока индикации
static int modulesThreadToRun = 1;      // Флаг для завершения потока опроса модулей
static int mqttThreadToRun = 1;         // Флаг для завершения потока обмена данными с MQTT

static int delayIndicationThreadUsec = 1000;    // Задержка в потоке индикации (в микросекундах)
static int delayModulesThreadUsec = 1000;       // Задержка в потоке опроса модулей (в микросекундах)
static int delayMqttThreadUsec = 1000;          // Задержка в потоке обмена данными с MQTT (в микросекундах)



/// @brief MOD: Получить наименование платы
/// @param buffer Буффер для результата
/// @return 
static SYSTEM_BOARD_ENUM System_GetBoardLinux()
{
    SYSTEM_BOARD_ENUM result = SYSTEM_BOARD_UNKNOWN;
    Log_Write("System: Getting board name...");

    FILE *fp;
    char buf[100];
    fp = fopen("/sys/firmware/devicetree/base/model", "r");
    if (fp == NULL) 
    {
        Log_Write("System: WARNING.Failed to open /sys/firmware/devicetree/base/model");

        fp = fopen("/proc/device-tree/model", "r");
        if (fp == NULL) 
        {
            Log_Write("System: WARNING. Failed to open /proc/device-tree/model");
            Log_Write("System: Board name - Unknown");
            return result;
        }
    }

    if (fgets(buf, sizeof(buf), fp) != NULL)
    {
        // Удаление символа новой строки, если он присутствует
        int len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }
    } 
    else 
    {
        return result;
    }

    for (int i = 0; i < SYSTEM_BOARD_COUNT; i++)
    {
        if (strstr(buf, SYSTEM_BOARDS_NAMES[i]) != NULL)
        {
            board = (SYSTEM_BOARD_ENUM)i;
            break;
        }
    }

    Log_Write("System: Board name - %s (boardcode = %d)", buf, board);
    fclose(fp);

    return result;
}


// ***** Инициализация *****

/// @brief Инициализировать данные системы
/// @return Результат (0 - успех, отрицательное число - ошибка)
static int System_InitSystemData()
{
    int result = -1;
    Log_Write("System: Initializing system data...");
    // Логи
    Log_EnableTerminalOutput(config.log_use_terminal_output);
    Log_UseRowsCleaning(config.log_rows_min_count, config.log_rows_max_count);

    // TODO: Инициализация данных системы и применение конфигурации к системе
    
    
    // Получить название платы
#ifdef __linux__
    board = System_GetBoardLinux();
#else
    board = SYSTEM_BOARD_UNKNOWN;
#endif


    Log_Write("System: System data initialized.");
    result = 0;
    return result;
}


/// @brief Инициализировать индикацию
/// @return Результат (0 - успех, отрицательное число - ошибка)
static int System_InitIndication()
{
    int result = -1;
    Log_Write("System: Initializing indication...");

#ifdef TEST_MODE
    Log_Write("System: WARNING. TEST MODE. DO NOT USE IN PRODUCTION.");
    result = 0;
#endif

    // TODO: Инициализация данных индикации

    Log_Write("System: Indication initialized.");
    //result = 0;
    return result;
}

/// @brief Инициализировать модули
/// @return Результат (0 - успех, отрицательное число - ошибка)
static int System_InitModules()
{
    int result = -1;
    Log_Write("System: Initializing modules...");

    modulesCount = 0;
    // TODO: Ready to test
    if (config.modules == NULL || config.modules_count <= 0)
    {
        return result;
    }

    // Проверить количество проинициализированных модулей
    for (int i = 0; i < config.modules_count; i++)
    {
        if (config.modules[i].inited)
        {
            modulesCount++;
        }
    }

    if (modulesCount <= 0)
    {
        return result;
    }

    // Создать I2C подключение    
    Log_Write("System: Creating I2C connection...");
    connection = I2C_CreateConnection(&connectionConfig);
#ifndef __linux__ 
    // TEMP: Создание эмуляции подключения для пропуска ошибки на Windows
    connection = calloc(1, sizeof(I2C_Connection));
    Log_Write("System: WARNING. TEST MODE. DO NOT USE IN PRODUCTION. I2C connection created.");
#endif
    if (connection == NULL)
    {
        Log_Write("System: ERROR. Failed to create I2C connection!");
        return result;
    }
    Log_Write("System: I2C connection created!");

    // Выделить память под указатели на модули
    modules = (Module**)calloc(modulesCount, sizeof(Module*));
    if (modules == NULL)
    {
        Log_Write("System: ERROR. Failed to allocate memory for modules list !");
        free(connection);
        connection = NULL;
        return result;
    }
    
    for (int i = 0, j = 0; i < config.modules_count; i++)
    {
        if (config.modules[i].inited)
        {
            Module_Config mConfig =
            {
                .inited = config.modules[i].inited,
                .code = config.modules[i].code,
                .address = config.modules[i].address,
                .uniqueName = config.modules[i].uniqueName,
                .name = config.modules[i].name,
                .description = config.modules[i].description
            };

            Module *module = Module_Create(&mConfig, connection);
            // XXX: Ост 10.06.2024 16:30
            if (module != NULL)
            {
                // Добавить модуль в систему
                modules[j] = module;
                j++;
            }
        }
    }

    Log_Write("System: Modules initialized.");
    result = 0;
    return result;
}

/// @brief Инициализировать Mqtt
/// @return Результат (0 - успех, отрицательное число - ошибка)
static int System_InitMqtt()
{
    int result = -1;
    Log_Write("System: Initializing MQTT...");

    // TODO: Инициализация данных Mqtt

#ifdef TEST_MODE
    Log_Write("System: WARNING. TEST MODE. DO NOT USE IN PRODUCTION.");
    result = 0;
#endif

    Log_Write("System: MQTT initialized.");
    //result = 0;
    return result;
}


/// @brief Обработчик потока индикации
/// @param args Аргументы
/// @return Результат
static void* System_Indication_ThreadHandler(void *args)
{
    while (indicationThreadToRun)
    {
        // TODO: Обработчик потока индикации

        // Засыпаем
        usleep(delayIndicationThreadUsec);
    }

    return EXIT_SUCCESS;
}


/// @brief Обработчик потока опроса модулей
/// @param args Аргументы
/// @return Результат
static void* System_ModulesPolling_ThreadHandler(void *args)
{
    // TODO: Обработчик потока опроса модулей
    while (modulesThreadToRun)
    {
        for (int i = 0; i < modulesCount; i++)
        {
            // Чтение всех выводов модуля
            Module_ReadAllPins(modules[i]);

            // Запись всех выводов модуля
            Module_WriteAllPins(modules[i]);

            // Засыпаем
            usleep(delayModulesThreadUsec);
        }
        
    }

    return EXIT_SUCCESS;
}

/// @brief Обработчик потока обмена данными с Mqtt
/// @param args Аргументы
/// @return Результат
static void* System_MqttDataExchange_ThreadHandler(void *args)
{
    while (mqttThreadToRun)
    {
        // TODO: Обработчик потока обмена данными с Mqtt

        // Засыпаем
        usleep(delayMqttThreadUsec);
    }    
    
    return EXIT_SUCCESS;
}


// ***** Предоставляемые интерфейсы system.h (реализация)*****

int System_Init()
{
    int result = -1;
    
    // TODO: Инициализация индикации
    // Indication_Init();

    Log_Write("System: Initializing...");
    
    // Инициализация конфигурации
    int configInitResult = Config_Init(NULL);

    if (configInitResult != 0)
    {
        Log_Write("System: ERROR. Failed to init configuration!");
        return result;
    }

    // Чтение конфигурации
    int configReadResult = Config_Read(&config);
    if (configReadResult != 0)
    {
        Log_Write("System: ERROR. Failed to read configuration!");
        return result;
    }
    else
    {
        Log_Write("System: Configuration successfully read");

        if (config.modules_count > 0)
        {
            Log_Write("System: There are %d modules in configuration:", config.modules_count);
            for (int i = 0; i < config.modules_count; i++)
            {
                if (config.modules[i].inited)
                {
                    Log_Write("System: Module %s inited", config.modules[i].name);
                }
                else
                {
                    if (config.modules[i].name != NULL)
                    {
                        Log_Write("System: WARNING. Module %s (index %d) is NOT inited! It will not be used", config.modules[i].name, i);
                    }
                    else
                    {
                        Log_Write("System: WARNING. Module (index %d) is NOT inited! It will not be used", i);
                    }
                }
            }            
        }
        else
        {
            Log_Write("System: ERROR. There are no modules in configuration! Service will be stoped");
            return result;
        }
        
    }

    // Отображение данных конфигурации
    Config_ShowData(&config);

    // Инициализация данных системы
    int res = System_InitSystemData();
    if (res < 0)
    {
        Log_Write("System: ERROR. Failed to init system data!");
        return result;
    }

    // Инициализация данных индикации
    res = System_InitIndication();
    if (res < 0)
    {
        Log_Write("System: ERROR. Failed to init indication!");
        return result;
    }

    // Инициализация данных модулей
    res = System_InitModules();
    if (res < 0)
    {
        Log_Write("System: ERROR. Failed to init modules!");
        return result;
    }

    // Инициализация данных Mqtt
    res = System_InitMqtt();
    if (res < 0)
    {
        Log_Write("System: ERROR. Failed to init MQTT!");
        return result;
    }
    
    Log_Write("System: Initialized");
    result = 0;
    return result;
}

int System_Start()
{
    int result = -1;

    // TODO: Запуск системы
    Log_Write("System: Starting...");

    Log_Write("System: Starting indication thread");
    // Создание потока индикации
    int res1 = pthread_create(&threadIndication, NULL, System_Indication_ThreadHandler, NULL);
    if (res1 != 0)
    {
        Log_Write("System: ERROR. Failed to start indication (Thread_Indication). Error code: %d", res1);
        return result;
    }
    Log_Write("System: Indication thread started");

    Log_Write("System: Starting modules polling thread");
    // Создание потока опроса модулей
    int res2 = pthread_create(&threadModules, NULL, System_ModulesPolling_ThreadHandler, NULL);
    if (res2 != 0)
    {
        Log_Write("System: ERROR. Failed to start polling thread (Thread_Modules). Error code: %d", res2);
        return result;
    }
    Log_Write("System: Modules polling thread started");

    Log_Write("System: Starting MQTT data exchange thread");
    // Создание потока обмена данными с Mqtt
    int res3 = pthread_create(&threadMqtt, NULL, System_MqttDataExchange_ThreadHandler, NULL);
    if (res3 != 0)
    {
        Log_Write("System: ERROR. Failed to start MQTT data exchange thread (Thread_MQTT). Error code: %d", res3);
        return result;
    }
    Log_Write("System: MQTT data exchange thread started");

    Log_Write("System: Started");

    // Ожидание завершения потока индикации
    res1 = pthread_join(threadIndication, NULL);
    if (res1 != 0)
    {
        Log_Write("System: ERROR. Failed to join Thread_Indication with result %d", res1);
        return result;
    }

    // Ожидание завершения потока опроса модулей
    res2 = pthread_join(threadModules, NULL);
    if (res2 != 0)
    {
        Log_Write("System: ERROR. Failed to join Thread_Modules with result %d", res2);
        return result;
    }

    // Ожидание завершения потока обмена данными с Mqtt
    res3 = pthread_join(threadMqtt, NULL);
    if (res3 != 0)
    {
        Log_Write("System: ERROR. Failed to join Thread_MQTT with result %d", res3);
        return result;
    }
    
    result = 0;
    return result;
}

int System_Stop()
{
    int result = -1;

    // TODO: Остановка системы
    Log_Write("System: Stopping...");

    // Завершение потоков
    indicationThreadToRun = 0;
    modulesThreadToRun = 0;
    mqttThreadToRun = 0;

    // Отсоединение от шины I2C
    I2C_DestroyConnection(connection);

    Log_Write("System: Stopped");

    result = 0;
    return result;
}
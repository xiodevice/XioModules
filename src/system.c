#include <pthread.h>
#include <stdlib.h>
#include "system.h"
#include "log.h"
#include "configurator.h"
#include "module_io.h"
#include "mqtt.h"

static Config config;
static pthread_t threadIndication;      // Поток индикации
static pthread_t threadModules;         // Поток для опроса модулей
static pthread_t threadMqtt;            // Поток для обмена данными с MQTT

static int indicationThreadToRun = 1;   // Флаг для завершения потока индикации
static int modulesThreadToRun = 1;      // Флаг для завершения потока опроса модулей
static int mqttThreadToRun = 1;         // Флаг для завершения потока обмена данными с MQTT


// ***** Инициализация *****

/// @brief Инициализировать данные системы
/// @return Результат (0 - успех, отрицательное число - ошибка)
static int System_InitSystemData()
{
    int result = -1;

    // Логи
    Log_EnableTerminalOutput(config.log_use_terminal_output);
    Log_UseRowsCleaning(config.log_rows_min_count, config.log_rows_max_count);

    // TODO: Инициализация данных системы и применение конфигурации к системе
    

    result = 0;
    return result;
}


/// @brief Инициализировать данные индикации
/// @return Результат (0 - успех, отрицательное число - ошибка)
static int System_InitIndicationData()
{
    int result = -1;

    // TODO: Инициализация данных индикации


    //result = 0;
    return result;
}

/// @brief Инициализировать данные модулей
/// @return Результат (0 - успех, отрицательное число - ошибка)
static int System_InitModulesData()
{
    int result = -1;

    // TODO: Инициализация данных модулей
    for (int i = 0; i < config.modules_count; i++)
    {
        
    }

    //result = 0;
    return result;
}

/// @brief Инициализировать данные Mqtt
/// @return Результат (0 - успех, отрицательное число - ошибка)
static int System_InitMqttData()
{
    int result = -1;

    // TODO: Инициализация данных Mqtt


    //result = 0;
    return result;
}


/// @brief Обработчик потока индикации
/// @param args Аргументы
/// @return Результат
static void* System_Indication_ThreadHandler(void* args)
{
    while (indicationThreadToRun)
    {
        // TODO: Обработчик потока индикации
    }

    return EXIT_SUCCESS;
}


/// @brief Обработчик потока опроса модулей
/// @param args Аргументы
/// @return Результат
static void* System_ModulesPolling_ThreadHandler(void* args)
{    
    while (modulesThreadToRun)
    {
        // TODO: Обработчик потока опроса модулей
    }

    return EXIT_SUCCESS;
}

/// @brief Обработчик потока обмена данными с Mqtt
/// @param args Аргументы
/// @return Результат
static void* System_MqttDataExchange_ThreadHandler(void* args)
{
    while (mqttThreadToRun)
    {
        // TODO: Обработчик потока обмена данными с Mqtt
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
    res = System_InitIndicationData();
    if (res < 0)
    {
        Log_Write("System: ERROR. Failed to init indication data!");
        return result;
    }

    // Инициализация данных модулей
    res = System_InitModulesData();
    if (res < 0)
    {
        Log_Write("System: ERROR. Failed to init modules data!");
        return result;
    }

    // Инициализация данных Mqtt
    res = System_InitMqttData();
    if (res < 0)
    {
        Log_Write("System: ERROR. Failed to init MQTT data!");
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

    // Создание потока индикации
    int res1 = pthread_create(&threadIndication, NULL, System_Indication_ThreadHandler, NULL);
    if (res1 != 0)
    {
        Log_Write("System: ERROR. Failed to start Thread_Indication");
        return result;
    }

    // Создание потока опроса модулей
    int res2 = pthread_create(&threadModules, NULL, System_ModulesPolling_ThreadHandler, NULL);
    if (res2 != 0)
    {
        Log_Write("System: ERROR. Failed to start Thread_Modules");
        return result;
    }

    // Создание потока обмена данными с Mqtt
    int res3 = pthread_create(&threadMqtt, NULL, System_MqttDataExchange_ThreadHandler, NULL);
    if (res3 != 0)
    {
        Log_Write("System: ERROR. Failed to start Thread_MQTT");
        return result;
    }

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
    
    //EINVAL = 22
    //ESRCH = 3
    //EDEADLK = 36
    
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

    Log_Write("System: Stopped");

    result = 0;
    return result;
}
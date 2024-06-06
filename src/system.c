#include <pthread.h>
#include <stdlib.h>
#include "system.h"
#include "log.h"
#include "configurator.h"
#include "module_io.h"
#include "mqtt.h"

static Config config;
static pthread_t threadIndication;
static pthread_t threadModules;
static pthread_t threadMqtt;

static int indicationThreadToRun = 1;
static int modulesThreadToRun = 1;
static int mqttThreadToRun = 1;

/// @brief Поток индикации
/// @param args Аргументы
/// @return Результат
void* ThreadIndication(void* args)
{
    for (int i = 0; i < 10000 && indicationThreadToRun; i++)
    {
        //printf("Thread 111\n");
        Log_Write("Thread 111");
    } 

    return EXIT_SUCCESS;
}


/// @brief Поток для опроса модулей
/// @param args Аргументы
/// @return Результат
void* ThreadModulesPolling(void* args)
{
    for (int i = 0; i < 10000 && modulesThreadToRun; i++)
    {
        //printf("Thread 222\n");
        Log_Write("Thread 222");
    }

    return EXIT_SUCCESS;
}

/// @brief Поток для обмена данными с MQTT
/// @param args Аргументы
/// @return Результат
void* ThreadMqttDataExchange(void* args)
{
    for (int i = 0; i < 10000 && mqttThreadToRun; i++)
    {
        //printf("Thread 333\n");
        Log_Write("Thread 333");
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

    // Отобразить данные конфигурации
    Config_ShowData(&config);

    // XXX: Остановился 02.06.2024 2:50
    // TODO: Инициализация системы

    // Логи
    Log_EnableTerminalOutput(config.log_use_terminal_output);
    Log_UseRowsCleaning(config.log_rows_min_count, config.log_rows_max_count);


    // TODO: Инициализация модулей ввода/вывода
    for (int i = 0; i < config.modules_count; i++)
    {

    }


    // TODO: Инициализация MQTT

    
    Log_Write("System: Initialized");
    result = 0;
    return result;
}

int System_Start()
{
    int result = -1;

    // TODO: Запуск системы
    Log_Write("System: Starting...");

    int res1 = pthread_create(&threadIndication, NULL, ThreadIndication, NULL);
    if (res1 != 0)
    {
        Log_Write("System: ERROR. Failed to start Thread_Indication");
        return result;
    }

    int res2 = pthread_create(&threadModules, NULL, ThreadModulesPolling, NULL);
    if (res2 != 0)
    {
        Log_Write("System: ERROR. Failed to start Thread_Modules");
        return result;
    }

    int res3 = pthread_create(&threadMqtt, NULL, ThreadMqttDataExchange, NULL);
    if (res3 != 0)
    {
        Log_Write("System: ERROR. Failed to start Thread_MQTT");
        return result;
    }

    Log_Write("System: Started");

    // Завершение потоков
    int res11 = pthread_join(threadIndication, NULL);
    if (res11 != 0)
    {
        Log_Write("System: ERROR. Failed to join Thread_Indication with result %d", res11);
        return result;
    }

    int res22 = pthread_join(threadModules, NULL);
    if (res22 != 0)
    {
        Log_Write("System: ERROR. Failed to join Thread_Modules with result %d", res22);
        return result;
    }

    int res33 = pthread_join(threadMqtt, NULL);
    if (res33 != 0)
    {
        Log_Write("System: ERROR. Failed to join Thread_MQTT with result %d", res33);
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
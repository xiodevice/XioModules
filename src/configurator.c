#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "log.h"
#include "configurator.h"
#include "libs/frozen.h"

// Использовать тестовую конфигурацию
#define USE_TEST_CONFIG 0

static char* CONFIG_FILE_NAME_DEFAULT = "config.json";  // Имя файла конфигурации по умолчанию
static char* fileName;  // Имя файла конфигурации

/// @brief Конфигурация модул 1 по умолчанию
static Config_Module configModule1Default =
{
    .code = 0,
    .address = 0,
    .uniqueName = "uid_module1",
    .name = "name_module1",
    .description = "description_module1"
};

/// @brief Конфигурация модул 2 по умолчанию
static Config_Module configModule2Default =
{
    .code = 0,
    .address = 0,
    .uniqueName = "uid_module2",
    .name = "name_module2",
    .description = "description_module2"
};

/// @brief Конфигурация модул 3 по умолчанию
static Config_Module configModule3Default =
{
    .code = 0,
    .address = 0,
    .uniqueName = "uid_module3",
    .name = "name_module3",
    .description = "description_module3"
};

// MOD: Конфигурация по умолчанию
static Config configDefault = 
{
    .log_use_terminal_output = 1,

    .log_rows_min_count = 9000,
    .log_rows_max_count = 10000,
    .modules_count = 3,

    .modules = NULL
};


/// @brief Распарсить JSON массив модулей в конфигурацию
/// @param str Строка в формате JSON
/// @param len Длина строки
/// @param user_data Данные пользователя. Конфигурация (Config*). При наличии данных - изменится параметр количества модулей
static void ParseJsonModulesArrayToConfig(const char *str, int len, void *user_data) 
{
    struct json_token t;

    if (user_data == NULL)
        return;
    
    Config* cfg = (Config*)user_data;

    //printf("Parsing array: %.*s\n", len, str);
    cfg->modules_count = 0;
    
    // Определяем количество элементов в массиве
    for (int i = 0; json_scanf_array_elem(str, len, "", i, &t) > 0; i++) 
    {
        cfg->modules_count++;
        //printf("Index %d, token %.*s\n", i, t.len, t.ptr);
    }

    if (cfg->modules_count == 0)
    {
        return;
    }

    cfg->modules = (Config_Module*)malloc(sizeof(Config_Module) * cfg->modules_count);

    if (cfg->modules == NULL)
    {
        Log_Write("Configurator: ERROR. Failed to allocate memory for modules!");
        return;
    }

    // MOD: Чтение данных модулей в структуру Config
    // Заполним массив модулей значениями
    for (int i = 0; i < cfg->modules_count; i++)
    {
        cfg->modules[i].code = 0;
        cfg->modules[i].address = 0;
        cfg->modules[i].uniqueName = NULL;
        cfg->modules[i].name = NULL;
        cfg->modules[i].description = NULL;
        cfg->modules[i].inited = 0;
    }

    // Парсим элементы массива и вносим их в конфигурацию
    for (int i = 0; json_scanf_array_elem(str, len, "", i, &t) > 0; i++)
    {
        int inited = 1;
        cfg->modules[i].inited = 0;
        
        if (json_scanf(t.ptr, t.len, "{code: %d}", &cfg->modules[i].code) <= 0)
        {
            inited = 0;
            Log_Write("Configurator: ERROR. There is no or invalid parameter <code> for (index %d) module!", i);
        }

        if (json_scanf(t.ptr, t.len, "{address: %d}", &cfg->modules[i].address) <= 0)
        {
            inited = 0;
            Log_Write("Configurator: ERROR. There is no or invalid parameter <address> for (index %d) module!", i);
        }
        
        if (json_scanf(t.ptr, t.len, "{uniqueName: %Q}", &cfg->modules[i].uniqueName) <= 0)
        {
            inited = 0;
            Log_Write("Configurator: ERROR. There is no or invalid parameter <uniqueName> for (index %d) module!", i);
        }
        
        if (json_scanf(t.ptr, t.len, "{name: %Q}", &cfg->modules[i].name) <= 0)
        {
            inited = 0;
            Log_Write("Configurator: ERROR. There is no or invalid parameter <name> for (index %d) module!", i);
        }
        
        if (json_scanf(t.ptr, t.len, "{description: %Q}", &cfg->modules[i].description) <= 0)
        {
            inited = 0;
            Log_Write("Configurator: WARNING. There is no or invalid parameter <description> for (index %d) module!", i);
        }
        
        if (!inited)
        {
            continue;
        }

        cfg->modules[i].inited = 1;
    }
  }

/// @brief Прочитать конфигурацию из файла
/// @param configOut Выходная конфигурация
/// @return Результат операции (Код ошибки)
static int ReadConfigFromFile(Config* configOut)
{
    int result = -1;

    if (configOut == NULL)
        return result;

    *configOut = configDefault;

    char *content = json_fread(fileName);

    if (content == NULL)
    {
        Log_Write("Configurator: ERROR. Failed to read config from file!");
        return result;
    }

    // MOD: Чтение конфигурации из файла в структуру Config
    json_scanf(content, strlen(content), "{log_use_terminal_output: %B}", &configOut->log_use_terminal_output);
    
    json_scanf(content, strlen(content), "{log_rows_min_count: %d}", &configOut->log_rows_min_count);
    json_scanf(content, strlen(content), "{log_rows_max_count: %d}", &configOut->log_rows_max_count);
    //json_scanf(content, strlen(content), "{p1: %d}", &configOut->p1);    
    configOut->modules_count = 0;

    json_scanf(content, strlen(content), "{modules: %M;", ParseJsonModulesArrayToConfig, configOut);
    
    free(content);
    content = NULL;

    if (configOut->modules_count <= 0)
    {
        Log_Write("Configurator: ERROR. There are no modules data in config file or it contains errors!");
    }

    result = 0;
    return result;
}


/// @brief Проверка файла конфигурации
/// @return Результат операции (Код ошибки). -1 - Ошибка в конфиге. 0 - Отсутствуют данные в конфиге. 1 - Конфиг валиден
static int CheckConfigFile()
{
    int result = -1;

    char *content = json_fread(fileName);
    if (content == NULL)
    {
        Log_Write("Configurator: ERROR. Config file %s is empty!", fileName);
        result = 0;
    }
    else
    {
        int res = json_prettify_file(fileName);
        if (res <= 0)
        {
            Log_Write("Configurator: ERROR. Config file %s contains errors!", fileName);
            result = -1;
        }
        else
        {
            result = 1;
        }
    }

    free(content);
    content = NULL;

    return result;
}


/// @brief Инициализировать конфигурацию по умолчанию для модулей 
/// @return Результат операции (Код ошибки)
static int InitDefaultConfigModulesData(Config* configDefault)
{
    int result = -1;
    if (configDefault == NULL)
        return result;

    configDefault->modules = malloc(sizeof(Config_Module) * configDefault->modules_count);
    if (configDefault->modules == NULL)
    {
        Log_Write("Configurator: ERROR. Failed to allocate modules array");
        return result;
    }

    configDefault->modules[0] = configModule1Default;
    configDefault->modules[1] = configModule2Default;
    configDefault->modules[2] = configModule3Default;

    result = 0;
    return result;
}

/// @brief Заполнить массив модулей
/// @param out 
/// @param ap 
/// @return Результат операции (Код ошибки)
static int PrintConfigModulesArray(struct json_out *out, va_list *ap)
{
    int result = -1;

    Config* config = va_arg(*ap, Config*);

    if (config == NULL)
        return result;

    // MOD: Заполнение массива модулей
    char* jsonModulesParams = 
        "{"
            "code: %d,"
            "address: %d,"
            "uniqueName: %Q,"
            "name: %Q,"
            "description: %Q"
        "}";
    
    json_printf(out, "[");
    for (int i = 0; i < config->modules_count; i++)
    {
        int fillResult = json_printf(out, jsonModulesParams, 
        config->modules[i].code,
        config->modules[i].address,
        config->modules[i].uniqueName,
        config->modules[i].name,
        config->modules[i].description);

        if (fillResult < 0)
        {
            Log_Write("Configurator: ERROR. Failed to print modules array to config file!");
            return result;
        }
        json_printf(out, ",");
    }
    json_printf(out, "]");   
    
    result = 0;
    return result;
}

/// @brief Записать конфигурацию в файл
/// @param config Конфигурация
/// @return Результат операции (Код ошибки)
static int WriteConfigFile(Config *config)
{
    int result = -1;

    // MOD: Запись файла конфигурации
    char* jsonParams = 
        "{"
            "log_use_terminal_output: %B,"

            "log_rows_min_count: %d,"
            "log_rows_max_count: %d,"
            "modules_count: %d,"

            "modules: %M"
        "}";
    
    int fillResult = json_fprintf(fileName, jsonParams,
        config->log_use_terminal_output,

        config->log_rows_min_count,
        config->log_rows_max_count,
        config->modules_count,

        PrintConfigModulesArray, config);
    
    if (fillResult < 0)
    {
        Log_Write("Configurator: ERROR. Failed to write config file!");
        return result;
    }

    json_prettify_file(fileName);

    result = 0;
    return result;
}

/// @brief Создать новый файл конфигурации
/// @param config Конфигурация
/// @return Код ошибки (если ошибки нет - 0)
static int CreateNewConfigFile()
{
    int result = -1;

    int createResult = json_fprintf(fileName, "");
    if (createResult < 0)
    {
        Log_Write("Configurator: ERROR. Failed to creat and open config file!");
        return 1;
    } 

    result = 0;
    return result;
}


// ***** Предоставляемые интрефейсы configurator.h (реализация) *****

int Config_Init(char* configFileName)
{
    int result = -1;
    Log_Write("Configurator: Config initialization...");

    if (configFileName == NULL)
    {
        fileName = CONFIG_FILE_NAME_DEFAULT;
    }
    else
    {
        fileName = configFileName;
    }

    // Проверка наличия файла конфигурации
    int fileExist = access(fileName, F_OK);
    if (fileExist == -1)
    {
        Log_Write("Configurator: ERROR. Config file %s doesn't exist!", fileName);        
        Log_Write("Configurator: Trying to create config file %s...", fileName);

        // Создание нового файла конфигурации со значениями по умолчанию
        int createResult = CreateNewConfigFile();
        if (createResult < 0)
        {
            return result;
        }

        Log_Write("Configurator: Config file created");

        InitDefaultConfigModulesData(&configDefault);

        // Заполнение нового файла конфигурации значениями по умолчанию
        Log_Write("Configurator: Default config writing...");

        int fillResult = WriteConfigFile(&configDefault);
        if (fillResult < 0)
        {
            return result;
        }
        
        Log_Write("Configurator: Default config is written");
    }
    else
    {
        Log_Write("Configurator: Checking config file %s...", fileName);
        int checkResult = CheckConfigFile();
        if (checkResult == 0)
        {
            // Записать конфигурацию по умолчанию
            InitDefaultConfigModulesData(&configDefault);

            // Заполнение файла конфигурации значениями по умолчанию
            Log_Write("Configurator: Default config writing...");

            int fillResult = WriteConfigFile(&configDefault);
            if (fillResult < 0)
            {
                return result;
            }
            
            Log_Write("Configurator: Default config is written");
        }
        else if (checkResult == -1)
        {
            return result;
        }
    }

    Log_Write("Configurator: Config initialized");

    result = 0;
    return result;
}


int Config_Read(Config *config)
{
    int result = -1;

    // Проверка наличия файла конфигурации
    int fileExist = access(fileName, F_OK);
    if (fileExist == -1)
    {
        Log_Write("Configurator: WARNING. Config file doesn't exist! Will be used default config");
        *config = configDefault;
    }

    Log_Write("Configurator: Config reading...");
    
    // Заполнение параметрами из файла конфигурации
    int readResult = ReadConfigFromFile(config);
    if (readResult < 0)
    {
        return result;
    }

    Log_Write("Configurator: Config is read");

    result = 0;
    return result;
}


int Config_Write(Config *config)
{
    int result = -1;

    if (config == NULL)
        return result;

    // Проверка наличия файла конфигурации
    int fileExist = access(fileName, F_OK);
    if (fileExist == -1)
    {
        Log_Write("Configurator: ERROR. Config file doesn't exist!");        
        Log_Write("Configurator: Trying to create config file %s...", fileName);

        // Создание нового файла конфигурации со значениями по умолчанию
        int createResult = CreateNewConfigFile();
        if (createResult < 0)
        {
            return result;
        }
    }

    Log_Write("Configurator: Config writing...");

    // Заполнение файла конфигурации
    int fillResult = WriteConfigFile(config);
    if (fillResult < 0)
    {
        return result;
    }

    Log_Write("Configurator: Config is written");
    
    result = 0;
    return result;
}


void Config_ShowData(Config *config)
{
    //MOD: Показать данные конфигурации
    if (config == NULL)
        return;
    
    Log_Write("Configurator: Config data:");
    Log_Write("Configurator: log_use_terminal_output = %d", config->log_use_terminal_output);
    Log_Write("Configurator: log_rows_min_count = %d", config->log_rows_min_count);
    Log_Write("Configurator: log_rows_max_count = %d", config->log_rows_max_count);
    Log_Write("Configurator: modules_count = %d", config->modules_count);

    for (int i = 0; i < config->modules_count; i++)
    {
        Log_Write("Configurator: Module %d:", i+1);
        Log_Write("Configurator: inited = %d", config->modules[i].inited);
        Log_Write("Configurator: code = %d", config->modules[i].code);
        Log_Write("Configurator: address = %d", config->modules[i].address);

        if (config->modules[i].uniqueName != NULL)
            Log_Write("Configurator: uniqueName = %s", config->modules[i].uniqueName);
        else
            Log_Write("Configurator: uniqueName = NULL");

        if (config->modules[i].name != NULL)
            Log_Write("Configurator: name = %s", config->modules[i].name);
        else
            Log_Write("Configurator: name = NULL");

        if (config->modules[i].description != NULL)
            Log_Write("Configurator: description = %s", config->modules[i].description);
        else
            Log_Write("Configurator: description = NULL");
    }
}
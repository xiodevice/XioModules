/*
Модуль конфигурации.
Чтение и запись конфигурации из файлов json
*/

#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

typedef struct
{
    char inited;                // Инициализирован
    int code;                   // Код модуля
    int address;                // Адрес модуля
    char* uniqueName;           // Уникальное имя модуля (будет использоваться при формировании MQTT топика)
    char* name;                 // Пользовательское имя модуля
    char* description;          // Описание модуля
} Config_Module;


// MOD: Данные конфигурации (Struct)
typedef struct
{
    char log_use_terminal_output;   // Использовать вывод данных лога в терминал
    
    int log_rows_min_count;         // Минимальное количество строк после чистки
    int log_rows_max_count;         // Максимальное количество строк до чистки        
    int modules_count;              // Количество модулей ввода-вывода
    
    Config_Module* modules;         // Модули ввода-вывода
} Config;


/// @brief Инициализировать конфигурацию
/// @param fileName Имя файла конфигурации. Если NULL - используется путь по умолчанию
/// @return Результат операции (Код ошибки)
int Config_Init(char* fileName);

/// @brief Прочитать конфигурацию
/// @param config Конфигурация. Должна быть не NULL
/// @return Код ошибки (если ошибки нет - 0)
int Config_Read(Config *config);

/// @brief Записать конфигурацию
/// @param config Конфигурация
/// @return Код ошибки (если ошибки нет - 0). 
/// @return В случае отсутствия файла конфигурации, создается новый
int Config_Write(Config *config);

/// @brief Отобразить данные конфигурации
/// @param config Конфигурация
void Config_ShowData(Config *config);
#endif
#ifndef LOG_H
#define LOG_H

//#define LOG_MULTITHREAD_MODE

/// @brief Инициализировать Log
/// @return Результат операции (Код ошибки)
int Log_Init();

/// @brief Установить имя Log файла
/// @param logFileName Имя Log файла
void Log_SetNewLogFileName(char* logFileName);

/// @brief Использовать чистку строк Log файла (в случае превышения размера)
/// @param minCount Минимальное число строк после чистки (если < 0 - использовать текущее значение)
/// @param maxCount Максимальное число строк до чистки (если == 0 - не использовать чистку, если < 0 - использовать текущее значение)
void Log_UseRowsCleaning(int minCount, int maxCount);

/// @brief Включить/выключить вывод в терминал
/// @param enable Включить (1) или выключить (0)
void Log_EnableTerminalOutput(char enable);

/// @brief Добавить сообщение в Log
/// @param message Сообщение
/// @param ... Параметры форматирования
void Log_Write(const char* message, ...);

#endif
#ifndef LOG_H
#define LOG_H

//#define LOG_MULTITHREAD_MODE


/// @brief Уровень логирования 
typedef enum
{
    LL_NONE = -1,           // Уровень логирования не задан
    LL_ERROR = 0,           // Уровень логирования + Ошибки
    LL_WARN = 1,            // Уровень логирования + Предупреждения
    LL_INFO = 2,            // Уровень логирования + Информация
    LL_DEBUG = 3,           // Уровень логирования + Отладочные сообщения
    LL_VERBOSE_DEBUG = 4,   // Уровень логирования + Отладочные сообщения + Дополнительная информация (не используется)
} LOG_LEVEL;

/// @brief Инициализировать Log
/// @return Результат операции (Код ошибки)
int Log_Init();

/// @brief Установить имя Log файла
/// @param logFileName Имя Log файла
void Log_SetNewLogFileName(char* logFileName);

/// @brief Установить уровень логирования (по умолчанию используется LL_INFO)
/// @param level Уровень логирования
void Log_SetLogLevel(LOG_LEVEL level);

/// @brief Проверить разрешение уровня логирования
/// @param level Уровень логирования
/// @return 0 - логирование запрещено, 1 - разрешено
int Log_CheckLogLevel(LOG_LEVEL level);

/// @brief Использовать чистку строк Log файла (в случае превышения размера)
/// @param minCount Минимальное число строк после чистки (если < 0 - использовать текущее значение)
/// @param maxCount Максимальное число строк до чистки (если == 0 - не использовать чистку, если < 0 - использовать текущее значение)
void Log_UseRowsCleaning(int minCount, int maxCount);

/// @brief Включить/выключить вывод в терминал
/// @param enable Включить (1) или выключить (0)
void Log_EnableTerminalOutput(char enable);

/// @brief Добавить сообщение в Log
/// @param level Уровень логирования
/// @param message Сообщение
/// @param ... Параметры форматирования
void Log_Write(LOG_LEVEL level, const char* message, ...);

/// @brief Записать сообщение в Log. Не рекомендуется использовать напрямую! Вместо него использовать макрос LOG(level, (message, formats...))
/// @param message Сообщение
/// @param ... Параметры форматирования
void Log_WriteMessage_DontUseDirectly(const char* message, ...);

/// @brief Макрос логирования. Использовать приоритетно, если код будет переноситься в Mongoose OS!
/// Код с сообщениями логирования в Mongoose OS в данном случае будет работать без внесения изменений.
/// @param l Уровень логирования
/// @param x Кортеж форматированного сообщения. Используются круглые скобки для передачи текущего параметра (сообщение, форматирование, ...)
/// Пример: LOG(LL_INFO, ("My info message: %d", 123));
#define LOG(l, x)  do { if (Log_CheckLogLevel(l))  { Log_WriteMessage_DontUseDirectly x; }} while (0)

#endif
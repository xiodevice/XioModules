#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "log.h"


#ifdef LOG_MULTITHREAD_MODE

#include <pthread.h>
static pthread_mutex_t logWriteMutex;

#endif

static char useRowsCleaning = 0;        // Использовать максимальное количество строк
static char enableTerminalOutput = 1;   // Вывод в терминал включен (1) или выключен (0)
static char inited = 0;                 // Проинициализирован

static int rowsMaxCount = 10000;        // Максимальное количество строк до чистки
static int rowsMinCount = 9000;         // Минимальное количество строк после чистки

static char* LOG_FILE_NAME_DEFAULT = "app.log"; // Имя файла Log по умолчанию
static char* LOG_TEMP_FILE_NAME = "temp.log";   // Имя файла Log временное
static char* fileName;                          // Имя файла
static int logFileRowsCount = 0;                // Количество строк в лог файле

/// @brief Удалить строки в лог файле с самого начала
/// @param rowsCount Количество первых строк для удаления
/// @return Количество Записанных строк
static int Log_RemoveLogFileRowsFromBeginning(int rowsCount)
{
    int result = 0;
    
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        Log_Write("Log: ERROR. Failed to open log file!");
        return result;
    }

    remove(LOG_TEMP_FILE_NAME);

    FILE* fileTemp = fopen(LOG_TEMP_FILE_NAME, "w");
    if (fileTemp == NULL)
    {
        Log_Write("Log: ERROR. Failed to create or open temp log file!");
        fclose(file);
        return result;
    }

    char ch;
    while (rowsCount > 0 && (ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
            rowsCount--;
    }

    while ((ch = fgetc(file)) != EOF)
    {
        fputc(ch, fileTemp);
        if (ch == '\n')
            result++;
    }

    fclose(file);
    fclose(fileTemp);

    if (remove(fileName) < 0)
    {
        Log_Write("Log: ERROR. Failed to remove old log file!");
    }

    if (rename(LOG_TEMP_FILE_NAME, fileName) < 0)
    {
        Log_Write("Log: ERROR. Failed to rename temp log file!");
    }
    
    return result;
}

/// @brief Получить количество строк в файле
/// @return Количество строк
static int Log_GetFileRowsCount()
{
    int result = 0;
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
        return -1;

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            result++;
        }
    }

    fclose(file);
    return result;
}


// ***** Предоставляемые интерфейсы log.h (реализация) *****

int Log_Init()
{
    int result = -1;
    inited = 0;

#ifdef LOG_MULTITHREAD_MODE
    pthread_mutex_init(&logWriteMutex, NULL);
#endif
    
    fileName = LOG_FILE_NAME_DEFAULT;
    
    FILE* logFile = fopen(fileName, "a");
    if (logFile == NULL)
    {
        printf("Log: Error initializing log file! There is error to open log file. Writing log to file won't to work\n");
        return result;
    }

    fclose(logFile);

    logFileRowsCount = Log_GetFileRowsCount();

    inited = 1;
    result = 0;
    return result;
}

void Log_SetNewFileName(char* logFileName)
{
    fileName = logFileName;
}

void Log_UseRowsCleaning(int minCount, int maxCount)
{
    if (minCount >= 0)
        rowsMinCount = minCount;

    if (maxCount == 0)
    {
        useRowsCleaning = 0;
    }
    else if (maxCount > 0)
    {        
        rowsMaxCount = maxCount;
        useRowsCleaning = 1;
    }
}

void Log_EnableTerminalOutput(char enable)
{
    enableTerminalOutput = enable;
}

void Log_Write(const char* message, ...)
{
    if (!inited)
    {
        printf("Log: Not inited!\n");
        return;
    }        

#ifdef LOG_MULTITHREAD_MODE
    pthread_mutex_lock(&logWriteMutex);
#endif

    if (useRowsCleaning)
    {
        if (logFileRowsCount >= rowsMaxCount)
        {
            int rowsToRemove = logFileRowsCount - rowsMinCount;
            if (rowsToRemove >= 0)
            {
                int removeRes = Log_RemoveLogFileRowsFromBeginning(rowsToRemove);
                if (removeRes > 0)
                    logFileRowsCount = rowsMinCount;
            }
        }
    }

    FILE* logFile = fopen(fileName, "a");
    if (logFile == NULL)
    {
        printf("Log: Error opening log file!\n");
        //return;
    }

    va_list args;
    va_start(args, message);

    time_t timer = time(NULL); // Текущее время в тиках
    const struct tm* t = localtime(&timer); // Текущее время в нормальном формате
    
    if (logFile != NULL)
    {
        fprintf(logFile, "%02d.%02d.%d %02d:%02d:%02d: ", t->tm_mday, t->tm_mon+1, t->tm_year+1900, t->tm_hour, t->tm_min, t->tm_sec);
        vfprintf(logFile, message, args);
        fprintf(logFile, "\n");
        logFileRowsCount++;
    }    

    if (!inited || enableTerminalOutput)
    {
        printf("%02d.%02d.%d %02d:%02d:%02d: ", t->tm_mday, t->tm_mon+1, t->tm_year+1900, t->tm_hour, t->tm_min, t->tm_sec);
        vprintf(message, args);
        printf("\n");
    }
    
    va_end(args);
    
    fclose(logFile);

#ifdef LOG_MULTITHREAD_MODE
    pthread_mutex_unlock(&logWriteMutex);
#endif
}

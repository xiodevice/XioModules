#include <signal.h>
#include <stdlib.h>
#include "src/log.h"
#include "src/system.h"

/// @brief Обработчик внешнего прерывания (Ctrl+C) из ОС
/// @param signal Сигнал прерывания
void SignalInterruptHadler(int signal)
{
    LOG(LL_INFO, ("SERVICE ABORTED!"));
    System_Stop();    
    LOG(LL_INFO, ("SERVICE COMPLITION..."));
    LOG(LL_INFO, ("SERVICE STOPPED"));
    exit(EXIT_SUCCESS);
}


int main(int args, char **argv)
{
    int result = EXIT_FAILURE;
    // Инициализировать Log
    Log_Init();

    LOG(LL_INFO, ("SERVICE STARTING..."));

    // Инициализировать внешнее прерывание (Ctrl+C) из ОС
    signal(SIGINT, SignalInterruptHadler);

    // Инициализировать системное ядро
    int res = System_Init();
    if (res < 0)
    {
        result = EXIT_FAILURE;
    }
    else
    {
        // Запуск системного ядра
        res = System_Start();
        if (result < 0)
        {
            result = EXIT_FAILURE;
        }

        result = EXIT_SUCCESS;
    }    
    
    System_Stop();
    LOG(LL_INFO, ("SERVICE COMPLITION..."));
    LOG(LL_INFO, ("SERVICE STOPPED"));
    return result;
} 

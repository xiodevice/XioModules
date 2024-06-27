/*
Системное ядро приложения.
Содержит в себе основную логику и связь компонентов системы
*/

#ifndef SYSTEM_H
#define SYSTEM_H

// MOD: Типы плат (изменять количество элементов вместе с BOARDS_NAMES[])
typedef enum
{
    SYSTEM_BOARD_UNKNOWN,       // Неизвестно
    SYSTEM_BOARD_NPICORE,       // NanoPi Neo Core (2)
    SYSTEM_BOARD_RPI,           // Raspberry Pi 3 - 5
    SYSTEM_BOARD_RPICP4,        // Raspberry Pi Compute Module 4
    SYSTEM_BOARD_OPI,           // Orange Pi 3
    SYSTEM_BOARD_CB1,           // BIGTREETECH CB1 Compute Module
    SYSTEM_BOARD_COUNT          // Количество типов плат
} SYSTEM_BOARD_ENUM;


/// @brief Инициализировать систему
/// @return Результат (Код ошибки)
int System_Init();

/// @brief Запустить систему
/// @return Результат (Код ошибки)
int System_Start();

/// @brief Остановить систему
/// @return Результат (Код ошибки)
int System_Stop();

#endif
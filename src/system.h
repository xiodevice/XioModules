/*
Системное ядро приложения.
Содержит в себе основную логику и связь компонентов системы
*/

#ifndef SYSTEM_H
#define SYSTEM_H

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
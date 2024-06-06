/*
Системное ядро приложения.
Содержит в себе основную логику и связь компонентов системы
*/

#ifndef SYSTEM_H
#define SYSTEM_H

/// @brief Инициализация системы
/// @return Результат (Код ошибки)
int System_Init();

/// @brief Запуск системы
/// @return Результат (Код ошибки)
int System_Start();

/// @brief Остановка системы
/// @return Результат (Код ошибки)
int System_Stop();

#endif
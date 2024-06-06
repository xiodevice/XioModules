/*
Интерфейс микросхем.
Связующее звено между модулем и микросхемой
*/

#ifndef ICHIP_H
#define ICHIP_H


// MOD: Наименование чипа (Enum)
typedef enum
{
    CHIP_NAME_NONE,       // Нет
    CHIP_NAME_PCA8574,    // PCF8574 (DIO)
    CHIP_NAME_PCA8575,    // PCF8575 (DIO)
    CHIP_NAME_ADS1115,    // ADS1115 (AI)
    CHIP_NAME_PCA9685,    // PCA9685 (AO)
    CHIP_NAME_XXX
} CHIP_NAME_ENUM;


// Интерфейс - Чип (struct)
typedef struct
{
    CHIP_NAME_ENUM name;                // Наименование
    int (*ReadPin)(int);                // Функция считывания. Params: number - номер вывода, return: считанное значение
    void (*WritePin)(int, int);         // Функция записи
} iChip;


#endif
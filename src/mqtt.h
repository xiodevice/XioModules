#include <stdio.h>
void ParseConfigFile1();
struct CSP1_Topics
{
    /* Топики Релейные выходов */
    // Топики установки
    char* rel1_topic;
    char* rel2_topic;
    char* rel3_topic;
    char* rel4_topic;
    char* rel5_topic;
    char* rel6_topic;
    char* rel7_topic;
    char* rel8_topic;
    char* rel9_topic;
    char* rel10_topic;
    // Топики чтения
    char* rel1_topic_cb;
    char* rel2_topic_cb;
    char* rel3_topic_cb;
    char* rel4_topic_cb;
    char* rel5_topic_cb;
    char* rel6_topic_cb;
    char* rel7_topic_cb;
    char* rel8_topic_cb;
    char* rel9_topic_cb;
    char* rel10_topic_cb;

    /* Топики дискретных входов */
    // Топики чтения
    char* di1_topic_cb;
    char* di2_topic_cb;
    char* di3_topic_cb;
    char* di4_topic_cb;
    char* di5_topic_cb;
    char* di6_topic_cb;
    char* di7_topic_cb;
    char* di8_topic_cb;
    char* di9_topic_cb;
    char* di10_topic_cb;

    /* Топики аналоговых выходов */
    // Топики установки
    char* ao1_topic;
    char* ao2_topic;
    char* ao3_topic;
    char* ao4_topic;
    // Топики чтения 
    char* ao1_topic_cb;
    char* ao2_topic_cb;
    char* ao3_topic_cb;
    char* ao4_topic_cb;

    /* Топики аналоговых входов */
    // Топики чтения
    char* ai1_topic_cb;
    char* ai2_topic_cb;
    char* ai3_topic_cb;
    char* ai4_topic_cb;
};


// Инициализация топиков MQTT
void InitMqttTopics();

// Инициализация топиков MQTT
void InitMqttTopics(FILE* file);

// Опубликовать данные MQTT
void PublishMqttData();

// Получение данных MQTT
void ReadMqttData();
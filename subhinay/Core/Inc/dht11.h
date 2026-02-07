#ifndef DHT_H_
#define DHT_H_

#include "stm32f4xx_hal.h"

typedef struct
{
    uint8_t Temperature;
    uint8_t Humidity;
} DHT11_Data;

uint8_t DHT11_Read(DHT11_Data *data);
#endif

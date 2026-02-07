#include "dht11.h"

extern TIM_HandleTypeDef htim2;

#define DHT_PORT GPIOA
#define DHT_PIN  GPIO_PIN_6

static void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    while (__HAL_TIM_GET_COUNTER(&htim2) < us);
}

static void DHT11_SetOutput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT_PORT, &GPIO_InitStruct);
}

static void DHT11_SetInput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT_PORT, &GPIO_InitStruct);
}

uint8_t DHT11_Read(DHT11_Data *data)
{
    uint8_t bits[5] = {0};

    DHT11_SetOutput();
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_RESET);
    HAL_Delay(18);
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_SET);
    delay_us(30);
    DHT11_SetInput();

    delay_us(80);
    if (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) return 1;
    delay_us(80);
    if (!HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN)) return 2;

    for (int i = 0; i < 40; i++)
    {
        while (!HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN));
        delay_us(40);
        if (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN))
            bits[i/8] |= (1 << (7 - (i % 8)));
        while (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN));
    }

    if ((bits[0] + bits[1] + bits[2] + bits[3]) != bits[4])
        return 3;

    data->Humidity = bits[0];
    data->Temperature = bits[2];
    return 0;
}

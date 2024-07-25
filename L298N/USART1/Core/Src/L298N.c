#include "main.h"

void W_IN1(uint8_t value)
{
    HAL_GPIO_WritePin(IN1_GPIO_Port,IN1_Pin,(GPIO_PinState)value);
}

void W_IN2(uint8_t value)
{
    HAL_GPIO_WritePin(IN2_GPIO_Port,IN2_Pin,(GPIO_PinState)value);
}

void W_IN3(uint8_t value)
{
    HAL_GPIO_WritePin(IN3_GPIO_Port,IN3_Pin,(GPIO_PinState)value);
}

void W_IN4(uint8_t value)
{
    HAL_GPIO_WritePin(IN4_GPIO_Port,IN4_Pin,(GPIO_PinState)value);
}

void M1_Forward(void)
{
    W_IN1(1);
    W_IN2(0);
}

void M1_Stop(void)
{
    W_IN1(0);
    W_IN2(0);
}

void M2_Forward(void)
{
    W_IN3(1);
    W_IN4(0);
}

void M2_Stop(void)
{
    W_IN3(0);
    W_IN4(0);
}

#ifndef INC_JDY31_H_
#define INC_JDY31_H_

#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdio.h>

#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256

#define OK_STR "+OK"
/*
  UART settings example
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
*/

enum Responses{
	OK
};

void JDY31_Init(UART_HandleTypeDef* uart);
void JDY31_RegisterCallback(void* func);
uint8_t JDY31_Disconnect();
uint8_t JDY31_Reset();
void JDY31_SendData(uint8_t* data, uint32_t size);
void JDY31_SetName(char* name);
void JDY31_SetPin(char* pin);

#endif /* INC_JDY31_H_ */

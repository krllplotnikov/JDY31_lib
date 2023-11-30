#include "JDY31.h"

UART_HandleTypeDef* m_uart;
uint8_t connected = 0;
void (*callbackFunc)(uint8_t*, uint32_t);
uint8_t rxBuffer[RX_BUFFER_SIZE] = {};
uint8_t txBuffer[TX_BUFFER_SIZE] = {};
uint8_t response = 0;

void JDY31_Init(UART_HandleTypeDef* uart){
	m_uart = uart;
}
void JDY31_RegisterCallback(void* func){
	callbackFunc = func;
	HAL_UARTEx_ReceiveToIdle_IT(m_uart, rxBuffer, RX_BUFFER_SIZE);
}
uint8_t JDY31_Disconnect(){
	HAL_UART_AbortReceive_IT(m_uart);

	sprintf((char*)txBuffer, "AT+DISC\r\n");
	JDY31_SendData(txBuffer, strlen((char*)txBuffer));
	HAL_UART_Receive(m_uart, rxBuffer, strlen(OK_STR), 100);
	memset(txBuffer, 0, RX_BUFFER_SIZE);
	response = 0;

	HAL_UARTEx_ReceiveToIdle_IT(m_uart, rxBuffer, RX_BUFFER_SIZE);
	if(strstr((char*)rxBuffer, OK_STR)){
		return OK;
	}
	else return -1;
}
uint8_t JDY31_Reset(){
	HAL_UART_AbortReceive_IT(m_uart);

	sprintf((char*)txBuffer, "AT+RESET\r\n");
	JDY31_SendData(txBuffer, strlen((char*)txBuffer));
	HAL_UART_Receive(m_uart, rxBuffer, strlen(OK_STR), 100);
	memset(txBuffer, 0, RX_BUFFER_SIZE);
	response = 0;

	HAL_UARTEx_ReceiveToIdle_IT(m_uart, rxBuffer, RX_BUFFER_SIZE);
	if(strstr((char*)rxBuffer, OK_STR)){
		return OK;
	}
	else return -1;
}

void JDY31_SendData(uint8_t* data, uint32_t size){
	HAL_UART_Transmit(m_uart, data, size, 100);
}

void JDY31_SetName(char* name){
	JDY31_Disconnect();

	sprintf((char*)txBuffer, "AT+NAME%s\r\n", name);
	JDY31_SendData(txBuffer, strlen((char*)txBuffer));
	memset(txBuffer, 0, RX_BUFFER_SIZE);

	JDY31_Reset();
}

void JDY31_SetPin(char* pin){
	JDY31_Disconnect();

	sprintf((char*)txBuffer, "AT+PIN%s\r\n", pin);
	JDY31_SendData(txBuffer, strlen((char*)txBuffer));
	memset(txBuffer, 0, RX_BUFFER_SIZE);

	JDY31_Reset();
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart == m_uart)
    {
    	memset(rxBuffer + Size, 0, RX_BUFFER_SIZE - Size);
    	if(rxBuffer[0] != '+'){
        	callbackFunc(rxBuffer, RX_BUFFER_SIZE);
    	}
    	else if(strstr((char*)rxBuffer, OK_STR)){
    		response = OK;
    	}
    	HAL_UARTEx_ReceiveToIdle_IT(m_uart, rxBuffer, RX_BUFFER_SIZE);
    }
}

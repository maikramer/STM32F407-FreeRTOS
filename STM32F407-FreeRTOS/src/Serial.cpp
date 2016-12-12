//=================================================================================//
//	Arquivo : Serial.cpp
//	Projeto : FreeRTOS9CMSIS
//	Autor : Maikeu Locatelli
//	Copyright : Locatelli Engenharia
//
//	Descricão: Objeto para manipulação de Serial no STM32F103
//=================================================================================//
//	This file is part of IntTeste
//	IntTeste is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//==================================================================================//
//	Includes
//==================================================================================//

#include <Serial.h>

//==================================================================================//
//	Váriaveis globais
//==================================================================================//

static UART_HandleTypeDef USART1_Handle;

//==================================================================================//
//	Definição dos Métodos
//==================================================================================//

Serial::Serial(USART_TypeDef * xPort) :
		_port(xPort), _init(pdFALSE) {
	xMutex = xSemaphoreCreateMutex();
	if (xMutex == NULL) {
		errorMessage("Erro ao criar MUTEX");
	}
}

//Destrutor
Serial::~Serial() {
	//Somente um placehold
}

//Inicializa a serial com a velocidade selecionada
//Ex: 9600, 115200 ...
BaseType_t Serial::init(unsigned long ulWantedBaud) {
	BaseType_t xReturn = pdFAIL;
	UART_InitTypeDef UART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	if (_init == pdTRUE) {
		return (pdPASS);
	}

	//Habilita os Clocks
	__HAL_RCC_USART1_CLK_ENABLE()
	;
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;

	/* The common (not port dependent) part of the initialisation. */
	UART_InitStructure.BaudRate = ulWantedBaud;
	UART_InitStructure.WordLength = UART_WORDLENGTH_8B;
	UART_InitStructure.StopBits = UART_STOPBITS_1;
	UART_InitStructure.Parity = UART_PARITY_NONE;
	UART_InitStructure.Mode = UART_MODE_TX_RX;
	UART_InitStructure.HwFlowCtl = UART_HWCONTROL_NONE;
	UART_InitStructure.OverSampling = UART_OVERSAMPLING_16;

	USART1_Handle.Init = UART_InitStructure;

	//Configuração do GPIO dos pinos PA9(TX) e PA10(RX)
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART1;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_9;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Configura Interrupção
	HAL_NVIC_SetPriority(USART1_IRQn,
	configLIBRARY_KERNEL_INTERRUPT_PRIORITY, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	// Inicializa a USART1
	USART1_Handle.Instance = USART1;
	USART1_Handle.gState = HAL_UART_STATE_RESET;
	USART1_Handle.RxState = HAL_UART_STATE_RESET;
	HAL_UART_Init(&USART1_Handle);

	//Inicia Recepção
	HAL_UART_Receive_IT(&USART1_Handle, (uint8_t *) cRxBuffer,
	serRX_BUFFER_SIZE);

	/* Everything is ok. */
	xReturn = pdPASS;

	return xReturn;
}

BaseType_t Serial::print(const char * pcStr) {
	static char cTxBuffer[256];

	//Aguarda até o fim da transmissão anterior
	xSemaphoreTake(xMutex, portMAX_DELAY);

	strcpy(cTxBuffer, pcStr);
	uint16_t uStrLen = strlen(cTxBuffer);
	BaseType_t xReturn;
	HAL_StatusTypeDef xHalStatus;

	xHalStatus = HAL_UART_Transmit_IT(&USART1_Handle, (uint8_t *) cTxBuffer,
			uStrLen);
	while (USART1_Handle.gState == HAL_UART_STATE_BUSY_TX)
		;

	if (xHalStatus != HAL_ERROR) {
		xReturn = pdPASS;
	}

	xSemaphoreGive(xMutex);

	return xReturn;
}

BaseType_t Serial::printf(const char *format, ...) {
	static char buffer[256];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 256, format, args);
	va_end(args);
	return (print(buffer));
}

BaseType_t Serial::gets(char * pcStr, uint16_t maxLen) {
	BaseType_t xReturn = pdFAIL;
	xSemaphoreTake(xMutex, portMAX_DELAY);

	uint16_t msgSize = USART1_Handle.RxXferSize - USART1_Handle.RxXferCount;
	if ((msgSize < (maxLen - 1)) && (msgSize > 0)) {
		strncpy(pcStr, cRxBuffer, msgSize);
		*(pcStr + msgSize + 1) = '\0';
		xReturn = pdPASS;
	}

//Reinicia o ponteiro de leitura
	USART1_Handle.RxXferCount = serRX_BUFFER_SIZE;
	USART1_Handle.pRxBuffPtr = (uint8_t *) cRxBuffer;

	xSemaphoreGive(xMutex);

	return xReturn;

}

void Serial::endRxTransfer(UART_HandleTypeDef *huart) {
	/* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
	CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
	CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

	/* At end of Rx process, restore huart->RxState to Ready */
	huart->RxState = HAL_UART_STATE_READY;
}

BaseType_t Serial::config(uint32_t baud,
		uint32_t flowCtl = UART_HWCONTROL_NONE) {
	BaseType_t xReturn = pdFAIL;

	USART1_Handle.Init.BaudRate = baud;
	USART1_Handle.Init.HwFlowCtl = flowCtl;

	if (HAL_UART_Init(&USART1_Handle) != HAL_ERROR) {
		xReturn = pdPASS;
	}

	return xReturn;
}

extern "C" void USART1_IRQHandler(void) {
	HAL_UART_IRQHandler(&USART1_Handle);
}

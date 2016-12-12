//=================================================================================//
//	Arquivo : Serial.h
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

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

//==================================================================================//
//	Includes STM32
//==================================================================================//

#include <stm32f4xx_hal.h>

//==================================================================================//
//	Includes FreeRTOS
//==================================================================================//

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

//==================================================================================//
//	Includes Locais
//==================================================================================//

#include <debug.h>

#ifndef SERIAL_H_
#define SERIAL_H_

//==================================================================================//
//	Definições de configuração
//==================================================================================//

#define	serRX_BUFFER_SIZE				( 1024 )

//==================================================================================//
//	Definição da Classe
//==================================================================================//

class Serial {
public:
	Serial(USART_TypeDef * xPort);
	virtual ~Serial();
	BaseType_t init(unsigned long ulWantedBaud);
	BaseType_t print(const char * str);
	BaseType_t printf(const char *format, ...);
	BaseType_t gets(char * pcStr, uint16_t maxLen);
	void endRxTransfer(UART_HandleTypeDef *huart);
	BaseType_t config(uint32_t baud, uint32_t flowCtl);
private:
	USART_TypeDef * _port;
	BaseType_t _init;
	SemaphoreHandle_t xMutex;
	char cRxBuffer[serRX_BUFFER_SIZE];
};

#endif /* SERIAL_H_ */

//=================================================================================//
//	Arquivo : Timer.h
//	Projeto : FreeRtos9CMSIS
//	Autor : Maikeu Locatelli
//	Copyright : Locatelli Engenharia
//
//	Descricão:
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
#include <stdlib.h>
#include <stdint.h>

//==================================================================================//
//	Includes STM32
//==================================================================================//

#include <stm32f4xx_hal.h>

//==================================================================================//
//	Includes FreeRTOS
//==================================================================================//

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//==================================================================================//
//	Includes Locais
//==================================================================================//

#include <debug.h>

#ifndef TIMER_H_
#define TIMER_H_

#define MAX_FUNCTIONS 1

typedef struct {
	uint32_t clk;
	GPIO_TypeDef* port;
	uint16_t pin;
}OC_t;

typedef void (*pFunction)();

class Timer {
public:
	Timer(TIM_TypeDef * timer);
	virtual ~Timer();
	void init();
	void setPeriod(uint16_t periodOverflow);
	void configIT(uint16_t periodOverflow, pFunction func);
	void setPWM(uint16_t channel, uint32_t frequency, uint8_t dutyCycle);
private:
	TIM_TypeDef * _timer;
	IRQn_Type _timerIRQn;
	bool _init;
};

#endif /* TIMER_H_ */

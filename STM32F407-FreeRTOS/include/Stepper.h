//=================================================================================//
//	Arquivo : Stepper.h
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

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//==================================================================================//
//	Includes Locais
//==================================================================================//

#include <debug.h>
#include <DigitalOut.h>

#ifndef STEPPER_H_
#define STEPPER_H_

enum class Speed
	: uint8_t {
		SLOWEST = 20,
		SLOW = 10,
		MEDIUM = 5,
		FAST = 2,
		FASTEST = 1
};

class Stepper {
public:
	Stepper(DigitalOut& step, DigitalOut& direction, DigitalOut& enableOut);
	virtual ~Stepper();
	void init();
	void update();
	void step(long ulSteps, Speed speed = Speed::MEDIUM);
private:
	DigitalOut& _stepOut;
	DigitalOut& _directionOut;
	DigitalOut& _enableOut;
	long long _totalSteps;
};

#endif /* STEPPER_H_ */

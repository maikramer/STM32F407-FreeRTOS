//=================================================================================//
//	Arquivo : Led.h
//	Projeto : FreeRtos9CMSIS
//	Autor : Maikeu Locatelli
//	Copyright : Locatelli Engenharia
//
//	Descrição: Classe para manipulação de LEDs no STM32F10x
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

//==================================================================================//
//	Includes STM32
//==================================================================================//

#include <stm32f4xx_hal.h>

//==================================================================================//
//	Includes Locais
//==================================================================================//

#include <DigitalOut.h>

#ifndef LED_H_
#define LED_H_

enum class Status
	: bool {
		OFF = false, ON = true
};

class Led: public DigitalOut {
public:
	Led(GPIO_TypeDef* port, uint16_t pin, State onState);
	Status status();
	//Retorna o status do led
	void set();
	//Acende o Led
	void clear();
	//Apaga o Led
	void toggle();
	//Alterna o estado do led
	virtual ~Led();
private:
	State _activeState;
};

#endif /* LED_H_ */

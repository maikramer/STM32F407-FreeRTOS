//=================================================================================//
//	Arquivo : DigitalOut.h
//	Projeto : IntTeste
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

//==================================================================================//
//	Includes STM32
//==================================================================================//

#include "stm32f4xx_hal.h"

#ifndef DIGITALOUT_H_
#define DIGITALOUT_H_

enum class State
	: bool {
		LOW = false, HIGH = true
};

class DigitalOut {
public:

	DigitalOut(GPIO_TypeDef* port, uint16_t pin);
	//Construtor
	virtual ~DigitalOut();
	//Destrutor
	void init();
	//Inicializa a porta
	void setOutput(State state);
	//Seta a saída
	//Argumentos aceitos State::LOW e State::HIGH
	State getOutput();
	//Busca estado da saída
private:
	GPIO_TypeDef* _port;
	uint16_t _pin;
	uint16_t _init;
};

#endif /* DIGITALOUT_H_ */

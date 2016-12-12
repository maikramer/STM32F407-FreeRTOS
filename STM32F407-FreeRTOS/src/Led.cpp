//=================================================================================//
//	Arquivo : Led.cpp
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

#include <Led.h>

#include "debug.h"

//==================================================================================//
//	Definição dos Métodos
//==================================================================================//

Led::Led(GPIO_TypeDef* port, uint16_t pin, State activeState) :
		DigitalOut(port, pin), _activeState(activeState) {
}

Status Led::status() {
	if (_activeState == State::HIGH) {
		if (getOutput() == State::HIGH) {
			return Status::ON;
		} else {
			return Status::OFF;
		}
	} else {
		if (getOutput() == State::LOW) {
			return Status::ON;
		} else {
			return Status::OFF;
		}
	}
}

void Led::set() {
	setOutput(_activeState);
}

void Led::clear() {
	if (_activeState == State::HIGH) {
		setOutput(State::LOW);
	} else {
		setOutput(State::HIGH);
	}
}

void Led::toggle() {
	if ((bool)status()) {
		clear();
	} else
		set();
}

Led::~Led() {
	// TODO Auto-generated destructor stub
}


//=================================================================================//
//	Arquivo : Button.h
//	Projeto : FreeRtos9CMSIS
//	Autor : Maikeu Locatelli
//	Copyright : Locatelli Engenharia
//
//	Descricão: Objeto para botões
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

#include <debug.h>

#ifndef BUTTON_H_
#define BUTTON_H_

class Button {
public:
	Button(GPIO_TypeDef* port, uint16_t pin);
	virtual ~Button();
	void init();
	bool getState();
private:
	GPIO_TypeDef* _port;
	uint16_t _pin;
	uint16_t _init;
};

#endif /* BUTTON_H_ */

//=================================================================================//
//	Arquivo : Button.cpp
//	Projeto : FreeRtos9CMSIS
//	Autor : Maikeu Locatelli
//	Copyright : Locatelli Engenharia
//
//	Descricão: Objeto para Botões
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

#include <Button.h>

//==================================================================================//
//	Definição dos Métodos
//==================================================================================//

Button::Button(GPIO_TypeDef* port, uint16_t pin) :
		_port(port), _pin(pin), _init(0) {
}

Button::~Button() {
}

void Button::init() {
	if (_init == 0) {
		GPIO_InitTypeDef GPIO_InitStructure;
		// Habilita o Clock
		if (_port == GPIOA)
			__HAL_RCC_GPIOA_CLK_ENABLE()
			;
		else if (_port == GPIOB)
			__HAL_RCC_GPIOB_CLK_ENABLE()
			;
		else if (_port == GPIOC)
			__HAL_RCC_GPIOC_CLK_ENABLE()
			;
		else if (_port == GPIOD)
			__HAL_RCC_GPIOD_CLK_ENABLE()
			;
		else if (_port == GPIOE)
			__HAL_RCC_GPIOE_CLK_ENABLE()
			;
		else if (_port == GPIOF)
			__HAL_RCC_GPIOF_CLK_ENABLE()
			;
		else
			errorMessage("Porta invalida");

		GPIO_InitStructure.Pin = _pin;
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(_port, &GPIO_InitStructure);
		_init = 1;
	}
}

bool Button::getState() {
	return (!((bool) HAL_GPIO_ReadPin(_port, _pin)));
}

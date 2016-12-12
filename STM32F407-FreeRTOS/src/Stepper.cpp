//=================================================================================//
//	Arquivo : Stepper.cpp
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
#include <Stepper.h>

Stepper::Stepper(DigitalOut& step, DigitalOut& direction, DigitalOut& enableOut) :
		_stepOut(step), _directionOut(direction), _enableOut(enableOut), _totalSteps(0) {

}

Stepper::~Stepper() {

}

void Stepper::init() {
	_stepOut.init();
	_directionOut.init();
	_enableOut.init();
}

void Stepper::update() {

}

void Stepper::step(long ulSteps, Speed speed) {
	_enableOut.setOutput(State::LOW);
	while (ulSteps != 0) {
		if (ulSteps > 0) {
			ulSteps--;
			_totalSteps++;
			_directionOut.setOutput(State::HIGH);
		} else {
			ulSteps++;
			_totalSteps--;
			_directionOut.setOutput(State::LOW);
		}
		_stepOut.setOutput(State::HIGH);
		vTaskDelay((uint8_t) speed / portTICK_RATE_MS);
		_stepOut.setOutput(State::LOW);
		vTaskDelay((uint8_t) speed / portTICK_RATE_MS);
	}
	_enableOut.setOutput(State::HIGH);
}

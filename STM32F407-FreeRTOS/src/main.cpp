//=================================================================================//
//	Arquivo : Main.cpp
//	Projeto : FreeRtos9CMSIS
//	Autor : Maikeu Locatelli
//	Copyright : Locatelli Engenharia
//
//	Descricão: Configuração basica do FreeRTOS 9 com CMSIS no STM32F103
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

//==================================================================================//
//	Includes FreeRTOS
//==================================================================================//

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//==================================================================================//
//	Includes Locais
//==================================================================================//

#include <Led.h>
#include <Button.h>
#include <Serial.h>
#include <Timer.h>
#include <Stepper.h>
#include <ESP8266.h>

#include "debug.h"

//==================================================================================//
//	Objetos
//==================================================================================//

Led led(GPIOE, GPIO_PIN_0, State::HIGH);

Led led2(GPIOE, GPIO_PIN_2, State::HIGH);

Button botao(GPIOB, GPIO_PIN_6);

// ESP8266
Serial serial1(USART1);
ESP8266 wifi(serial1);

//Timer timer1(TIM1);

//Motor de passo
DigitalOut stepOut(GPIOB, GPIO_PIN_7);
DigitalOut directionOut(GPIOB, GPIO_PIN_6);
DigitalOut enableOut(GPIOB, GPIO_PIN_8);
Stepper step1(stepOut, directionOut, enableOut);

//==================================================================================//
//	Defines
//==================================================================================//

#define mainVINICIALIZE_PRIO	configMAX_PRIORITIES
#define mainLEDBLINK_PRIO		2
#define mainESCREVESERIAL_PRIO	2
#define mainLESERIAL_PRIO		2
#define mainLEBOTAO_PRIO		2
#define mainENVIASTEPS_PRIO		3

//==================================================================================//
//	Funções
//==================================================================================//

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

static void LEDBlinkTask(void *pvParameters) {
	while (1) {
		// Delay 1 segundo e então alterna estado
		vTaskDelay(1000 / portTICK_RATE_MS);
		led.toggle();
		if (led.status() == Status::ON) {
			wifi.sendTelnet("#led:Ligado");
		} else {
			wifi.sendTelnet("#led:Desligado");
		}
	}

	//Recomendação, se conseguir quebrar o loop, deleta a tarefa
	vTaskDelete(NULL);
}

static void escreveSerialTask(void *pvParameters) {
	while (1) {
		vTaskDelay(2000 / portTICK_RATE_MS);
		wifi.sendTelnet("#msg:Ola de escreveSerial\n");
	}

	//Recomendação, se conseguir quebrar o loop, deleta a tarefa
	vTaskDelete(NULL);
}

static void leSerialTask(void *pvParameters) {
	while (1) {
		vTaskDelay(3000 / portTICK_RATE_MS);
		wifi.sendTelnet("#msg:Ola de leSerial\n");
	}

	//Recomendação, se conseguir quebrar o loop, deleta a tarefa
	vTaskDelete(NULL);
}

static void leBotao(void *pvParameters) {
	while (1) {
		if (botao.getState()) {
			led2.set();
		} else
			led2.clear();
		portYIELD()
		;
	}

	//Recomendação, se conseguir quebrar o loop, deleta a tarefa
	vTaskDelete(NULL);
}

static void enviaStepsTask(void *pvParameters) {
	while (1) {
		step1.step(1000, Speed::FASTEST);
		vTaskDelay(2000 / portTICK_RATE_MS);
		step1.step(-200, Speed::SLOWEST);
		vTaskDelay(2000 / portTICK_RATE_MS);
	}

	//Recomendação, se conseguir quebrar o loop, deleta a tarefa
	vTaskDelete(NULL);
}

static void vInicialize(void *pvParameters) {

	//Inicializa os Leds
	led.init();
	led2.init();
	step1.init();

	//Inicializa serial
	serial1.init(115200);

	//Inicializa o botao
	botao.init();

	//Inicializa o ESP8266
	wifi.begin();
	while (wifi.connect() == pdFAIL) {
		vTaskDelay(2000 / portTICK_RATE_MS);
	}
	while (wifi.startTelnet() == pdFAIL) {
		if (wifi.connStatus() == WifiStatus::DISCONNECTED) {
			wifi.connect();
		}
		vTaskDelay(2000 / portTICK_RATE_MS);
	}
	logMessage("Cria as Tarefas");
	//Cria as Tarefas
	xTaskCreate(LEDBlinkTask, "Blink", 512, NULL, mainLEDBLINK_PRIO, NULL);
	logMessage("Tarefa Blink Adcionada\n");

	xTaskCreate(escreveSerialTask, "EscreveSerial", 512, NULL,
	mainESCREVESERIAL_PRIO, NULL);
	logMessage("Tarefa EscreveSerial Adcionada\n");

	xTaskCreate(leSerialTask, "LeSerial", 512, NULL, mainLESERIAL_PRIO, NULL);
	logMessage("Tarefa LeSerial Adcionada\n");

	xTaskCreate(leBotao, "LeBotao", 512, NULL, mainLEBOTAO_PRIO, NULL);
	logMessage("Tarefa LeBotao Adcionada\n");

	xTaskCreate(enviaStepsTask, "EnviaSteps", 512, NULL, mainENVIASTEPS_PRIO,
	NULL);
	logMessage("Tarefa EnviaSteps Adcionada\n");

	//Deleta a tarefa se tudo correu bem
	vTaskDelete(NULL);
}

int main(void) {
	trace_printf("Iniciando com clock : %d\n\n", SystemCoreClock);
	//Recomendação para FreeRTOS com cortex M3, 4 bits pre-emption priority 0 bits subpriority
	HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4);

	//Tarefa de Inicializacao, só executa 1 vez
	xTaskCreate(vInicialize, "Inicialize", 512, NULL, mainVINICIALIZE_PRIO,
	NULL);

	vTaskStartScheduler();
	errorMessage("Nao foi possivel alocar memoria para a tarefa Idle");

	//Nunca deve chegar aqui
	while (1)
		;
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask,
		signed char *pcTaskName) {
	/* This function will get called if a task overflows its stack.   If the
	 parameters are corrupt then inspect pxCurrentTCB to find which was the
	 offending task. */

	trace_printf("Stack Overflow na tarefa %s", pcTaskName);

	for (;;)
		;
}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

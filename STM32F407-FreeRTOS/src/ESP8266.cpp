//=================================================================================//
//	Arquivo : ESP8266.cpp
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

#include <ESP8266.h>

//Configurações de conexão
const char * WIFI_RSID = "Netvirtua apt 1004";
const char * WIFI_PASSWORD = "27857000";

//const char * WIFI_RSID = "LGK10";
//const char * WIFI_PASSWORD = "12345678";

ESP8266::ESP8266(Serial& serial) :
		_serial(serial), _init(pdFALSE), telnetEnabled(pdFALSE) {
	xMutex = xSemaphoreCreateMutex();
	if (xMutex == NULL) {
		errorMessage("Erro ao criar MUTEX");
	}
}

ESP8266::~ESP8266() {
	// TODO Auto-generated destructor stub
}

BaseType_t ESP8266::begin() {
	BaseType_t xReturn = pdFAIL;

	// Reinicia o modulo
	_serial.print("AT+RST\r\n");
	vTaskDelay(2000 / portTICK_RATE_MS);
	_serial.gets(cRxBuffer, espRX_BUFFER_SIZE);

	if (cmd("AT", "Verifica Status", 3) == pdFAIL) {
		return xReturn;
	}

	// Desliga o Echo
	if (cmd("ATE1", "Desliga Echo", 3) == pdPASS) {
		xReturn = pdPASS;
		_init = pdTRUE;
		logMessage("ESP8266 Inicializado com Sucesso");
	}
	if (xReturn == pdFAIL) {
		logMessage("Falha na inicializacao do ESP8266");
	}
	return xReturn;
}

WifiStatus ESP8266::connStatus(void) {
	WifiStatus wsReturn = WifiStatus::FAIL;
	if (cmd("AT+CIPSTATUS", nullptr, 3)) {
		if (strstr(cRxBuffer, "STATUS:2") != nullptr) {
			wsReturn = WifiStatus::GOTIP;
		} else if (strstr(cRxBuffer, "STATUS:3") != nullptr) {
			wsReturn = WifiStatus::CONNECTED;
		} else if (strstr(cRxBuffer, "STATUS:4") != nullptr) {
			wsReturn = WifiStatus::DISCONNECTED;
		} else if (strstr(cRxBuffer, "STATUS:5") != nullptr) {
			wsReturn = WifiStatus::FAIL;
		}
	}
	return wsReturn;
}

BaseType_t ESP8266::connect() {
	BaseType_t xReturn = pdFAIL;

	if (_init == pdFALSE) {
		logMessage("Modulo nao inicializado com sucesso, inicialize");
		return xReturn;
	}
	//Verifica se já Conectado
	if (connStatus() != WifiStatus::FAIL) {
		logMessage("Wifi Conectado");
		return (pdPASS);
	}

	//Seta Modo para Station e ativa DHCP
	cmd("AT+CWMODE_CUR=1", "Ativa Modo Station", 3);
	cmd("AT+CWDHCP_CUR=1,1", "Ativa DHCP", 3);

	//Tenta conectar
	for (long lRetries = 3; lRetries > 0; lRetries--) {
		_serial.printf("AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", WIFI_RSID,
				WIFI_PASSWORD);
		do {
			vTaskDelay(1000 / portTICK_RATE_MS);
			_serial.gets(cRxBuffer, espRX_BUFFER_SIZE);
			if (strstr(cRxBuffer, "ERROR") != nullptr) {
				break;
			}
		} while (strstr(cRxBuffer, "GOT") == nullptr);

		if (connStatus() == WifiStatus::GOTIP) {
			xReturn = pdPASS;
			logMessage("Conectado!!");
			break;
		} else {
			logMessage("Falha ao Conectar, Tentando Novamente");
			vTaskDelay(1000 / portTICK_RATE_MS);
		}
	}
	return xReturn;
}

BaseType_t ESP8266::cmd(const char * command, const char *logMsg,
		unsigned char retries) {
	BaseType_t xReturn = pdFAIL;
	static char logMsgF[100];
	while (retries > 0) {
		if (logMsg != nullptr) {
			sprintf(logMsgF, "%s %d Tentativas Restantes", logMsg, retries);
			logMessage(logMsgF);
		}

		_serial.printf("%s\r\n", command);
		vTaskDelay(espWAIT_FOR_MSG / portTICK_RATE_MS);
		_serial.gets(cRxBuffer, espRX_BUFFER_SIZE);
		if (*cRxBuffer != '\0') {
			while (strstr(cRxBuffer, "busy") != nullptr) {
				_serial.printf("%s\r\n", command);
				vTaskDelay(espWAIT_FOR_MSG / portTICK_RATE_MS);
				_serial.gets(cRxBuffer, espRX_BUFFER_SIZE);
			}

			if (strstr(cRxBuffer, "OK") != nullptr) {
				return pdPASS;
			}
		}
		retries--;
		if (retries != 0) {
			vTaskDelay(espWAIT_FOR_MSG / portTICK_RATE_MS);
		}
	}
	trace_printf("LOG : Falha no comando %s\n", command);
	return xReturn;
}

BaseType_t ESP8266::startTelnet(void) {
	BaseType_t xReturn = pdFAIL;
	static char ip[30];

	// Verifica se está conectado
	if (connStatus() != WifiStatus::GOTIP) {
		logMessage("Nao conectado, impossivel iniciar Telnet");
		return xReturn;
	}

	// Verifica se Telnet já foi inicializado antes
	if (telnetEnabled == pdTRUE) {
		return (pdPASS);
	}

	// Ativa multiplas Conexões
	if (cmd("AT+CIPMUX=1", "Ativa multiplas conexoes", 3) == pdFAIL) {
		return xReturn;
	}

	// Estabelece o servidor para a porta 2222
	if (cmd("AT+CIPSERVER=1,2222", "Estabelece servidor na porta 2222",
			3)== pdFAIL) {
		return xReturn;
	}

	//Exibe endereço de ip
	if (cmd("AT+CIFSR", "Endereco de IP Atual", 3) == pdFAIL) {
		return xReturn;
	}
	char * pcBegin = strchr(cRxBuffer, '\"') + 1;
	char * pcEnd = strchr(pcBegin + 1, '\"');
	strncpy(ip, pcBegin, pcEnd - pcBegin);
	trace_printf("LOG : Conectado no endereco %s\n", ip);
	telnetEnabled = pdTRUE;
	return (pdPASS);
}

BaseType_t ESP8266::sendTelnet(const char * pcStr) {
	BaseType_t xReturn = pdFAIL;
	size_t sSize = strlen(pcStr);

	// Verifica se Telnet já inicializou
	if (telnetEnabled == pdFALSE) {
		return xReturn;
	}

	xSemaphoreTake(xMutex, portMAX_DELAY);
	// Verifica se está conectado
	WifiStatus cStatus = connStatus();
	if (cStatus != WifiStatus::CONNECTED) {
		if (cStatus == WifiStatus::DISCONNECTED) {
			logMessage("Cliente desconectou, Aguardando conexao Telnet");
		}
		if (cStatus == WifiStatus::GOTIP) {
			logMessage("Aguardando conexao Telnet");
		}
	} else {
		_serial.printf("AT+CIPSEND=0,%d\r\n", sSize);

		do {
			vTaskDelay(espWAIT_FOR_MSG / portTICK_RATE_MS);
			_serial.gets(cRxBuffer, espRX_BUFFER_SIZE);
		} while (strchr(cRxBuffer, '>') == nullptr);

		_serial.print(pcStr);
		vTaskDelay(espWAIT_FOR_MSG / portTICK_RATE_MS);
		_serial.gets(cRxBuffer, espRX_BUFFER_SIZE);
		char * pcSendOk = strstr(cRxBuffer, "SEND OK");
		char * pcError = strstr(cRxBuffer, "ERROR");
		if (pcSendOk != nullptr) {
			xReturn = pdPASS;
		} else if (pcError != nullptr) {
			logMessage("Erro ao enviar para telnet");
		}
		vTaskDelay(10 / portTICK_RATE_MS);
	}
	xSemaphoreGive(xMutex);
	return xReturn;
}

BaseType_t ESP8266::sendfTelnet(const char *format, ...) {
	static char buffer[128];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 128, format, args);
	va_end(args);
	return (sendTelnet(buffer));
}

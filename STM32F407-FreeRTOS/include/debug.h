//=================================================================================//
//	Arquivo : debug.h
//	Projeto : FreeRTOS9CMSIS
//	Autor : Maikeu Locatelli
//	Copyright : Locatelli Engenharia
//
//	Descricão: Manipulação de erros
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
#include "diag/Trace.h"
#include <stdlib.h>
//#include <string>

#ifndef INCLUDE_DEBUG_H_
#define INCLUDE_DEBUG_H_
#ifdef DEBUG

inline void logMessage(const char *pMessage){
	trace_printf("LOG : %s\n", pMessage);
}

#ifdef _GLIBCXX_STRING
inline void logMessage(std::string sMessage){
	const char * pMessage = sMessage.c_str();
	trace_printf("LOG : %s\n", pMessage);
}
#endif

inline void errorMessage(const char *pMessage){
	trace_printf("ERROR : %s\n", pMessage);
	trace_printf("Arquivo : %s Linha : \n ", __FILE__, __LINE__);
	abort ();
}
#else
#define logMessage(X) (0)
#define errorMessage(X) (0)
#endif




#endif /* INCLUDE_DEBUG_H_ */

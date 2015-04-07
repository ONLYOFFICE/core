/*! \file dcrawmessage.h
 *	\brief Вывод ошибок модуля dcraw
 */

#pragma once

#define DCRAW_MESSAGE(code,...)	do {fprintf(stderr,"[dcraw][%d]:",code); fprintf(stderr,...);} while(0);


/***************************************************************************
 *   Copyright (C) 2007 by Jesus Arias Fisteus                             *
 *   jaf@it.uc3m.es                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/*
 * xchar.h
 *
 * (Jesús Arias Fisteus)
 *
 * para tratar con tipos alternativos de
 * caracteres al tipo 'char'.
 *
 * hecho para que si en el futuro se manejan
 * otros juegos de caracteres distintos de ASCII
 * no sea necesario cambiar todo el código del
 * proyecto.
 *
 * Define el tipo xchar, que de momento es char,
 * pero que en un futuro se puede ajustar a otro
 * tipo, como wchar_t, sin tener que realizar demasiados
 * cambios en el resto del programa
 * 
 */


#ifndef XCHAR_H
#define XCHAR_H

#include <stdlib.h>

/* en principio, como estaba */
typedef unsigned char xchar;


/* conversión a minúsculas */
void xtolower(xchar *dest, const xchar *src, size_t dest_len);

/* búsqueda de subcadena en cadena */
xchar *xsearch(const xchar *str, const xchar *substr); 


/* devuelve la longitud de una cadena */
size_t xstrlen(const xchar *str);

/*
 * devuelve la longitud de una cadena (en caracteres)
 *
 */
size_t xstrnlen(const xchar *str, int maxlen);

/* devuelve la longitud de una cadena (bytes) contando
 * el treminador
 */
size_t xstrsize(const xchar *str);


#endif

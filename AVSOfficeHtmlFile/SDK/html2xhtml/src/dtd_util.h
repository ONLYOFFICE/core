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
 * dtd_util.h
 * 
 * (Jesús Arias Fisteus)
 *
 * Funciones para acceder a la información 
 * codificada de los DTD de XHTML en dtd.c y dtd.h
 *
 * Hay funciones de búsqueda de elementos y atributos por
 * su nombre y de comprobación de contenido de elementos
 * y de valores de atributos...
 *
 *
 */

#ifndef DTD_UTIL_H
#define DTD_UTIL_H

#ifdef NODEBUG
#undef CHILD_DEBUG
#endif

#include "dtd.h"
#include "dtd_names.h"
#include "xchar.h"


/*
 * cadenas de texto para cubrir el DOCTYPE de cada tipo
 * de documento XHTML
 *
 * Valores definidos en dtd_names.c
 *
 */
extern char doctype_string[DTD_NUM][DOC_STR_LEN];
extern char dtd_string[DTD_NUM][DTD_STR_LEN];
extern char dtd_key[DTD_NUM][DTD_KEY_LEN];
extern char dtd_name[DTD_NUM][DTD_NAM_LEN];

/*
 * devuelve el índice (número) del dtd cuya clave (key)
 * coincida con la proporcionada
 *
 */
int dtd_get_dtd_index(const char *key);

/*
 * devuelve un puntero al elemento o -1 si no se encuentra
 *
 */
int dtd_elm_search(const char *elm_name);


/*
 * devuelve un puntero a la referencia a entidad o -1 si no se encuentra
 * ent_name llega como '&nombre;'
 *
 */
int dtd_ent_search(const char *ent_name);



/*
 * devuelve un puntero al primer atributo coincidente
 *   o -1 si no se encuentra
 *
 * from:   '-1' desde el inicio de la lista de atributos
 *         n>=0 busca de (n+1) en adelante en la lista 
 *
 */
int dtd_att_search(const char *att_name, int from);


/*
 * busca un atributo con nombre 'att_name'
 * en la lista de atributos dada por 'in'
 *
 * devuelve -1 si no está, o el índice del atributo
 *
 */
int dtd_att_search_list(const char *att_name, const int *in);



/*
 * busca un atributo con id 'att_id'
 * en la lista de atributos dada por 'in'
 *
 * devuelve -1 si no está, o el índice del atributo
 *
 */
int dtd_att_search_list_id(int att_id, const int *in);



/*
 * lee una cadena de caracteres del buffer de elementos
 *
 */
char *dtd_elm_read_buffer(int buff_ptr);

/*
 * lee una cadena de caracteres del buffer de atributos
 *
 */
char *dtd_att_read_buffer(int buff_ptr);








/*
 * comprueba si el valor de un atributo es válido
 *
 * devuelve 1 si el valor cumple con las especificaciones
 * del DTD o 0 si no
 *
 */
int  dtd_att_is_valid(int att_id, xchar *value);




/**
 * comprueba el valor de un atributo desde el
 * punto de vista de aparición de caracteres ilegales
 * ('<' o '&' que no pertenezca a referencia a entidad)
 *
 * devuelve la posición en que se encuentre '<' o '&'
 *
 * o -1 si todo es correcto
 *
 */
int dtd_att_val_search_errors(const xchar *value);




/*
 * comprueba si el elemento 'child' es un hijo válido
 * de 'father'
 *
 * DEVUELVE: 1 si lo es o 0 si no lo es
 *
 */
/*int dtd_is_mixed_valid(int child, int father, int dtd_num);*/



/*
 * comprueba si la secuencia identificadores de elemento
 * concuerda con la regla de tipo 'child' dada por el
 * puntero.
 *
 * la secuencia 'elements' es un array de identificadores ordenada
 * el número de elementos del array lo indica 'num'
 * el puntero a la regla es 'rule_ptr'
 *
 * DEVUELVE: 1 si lo es o 0 si no lo es
 *
 */
int dtd_is_child_valid(int rule_ptr, int elements[], int num);


/*
 * comprueba si el elemento child_id puede ser hijo de father_id
 *
 * devuelve 1 si lo puede ser o 0 si no
 *
 * para tipo children, sólo comprueba que dicho elemento
 * esté dentro de su cadena
 *
 */ 
int dtd_can_be_child(int child, int father, int dtd_num);


/*
 * comprueba si el elemento elm es de tipo bloque
 *
 * devuelve 1 es de bloque; 0 si es inline
 *
 */ 
int dtd_elm_is_block(int elm);




#endif

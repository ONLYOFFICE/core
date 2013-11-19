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
 * procesador.h
 *
 * (Jesús Arias Fisteus)
 * 
 * Funciones que se ejecutan según se van encontrando
 * elementos... en el documento html de entrada.
 * El analizador sintáctico (yacc) notifica a este
 * módulo los componentes que se va encontrando en
 * el documento HTML de entrada, mediante
 * una interfaz semejante a la de SAX.
 *
 * En este módulo se llevan a cabo todas las 
 * comprobaciones necesarias y la conversión,
 * y se va generando un árbol en memoria para
 * almacenar el documento XHTML resultante
 * (ver tree.c)
 *
 * Se implementan también las funciones necesarias
 * para imprimir el documento XHTML a partir
 * del árbol. 
 *
 * Se activa el módulo mediante una llamada a
 * saxStartDocument() y se le indica el final del procesado
 * mediante saxEndDocument(). Se puede volcar la salida
 * con writeOutput() una vez invocada esta última función.
 *
 * -------
 * 
 * These functions are executed during the parsing of
 * the input document. The syntactic analyzer (yacc) notifies
 * this module about elements, attributes, etc. through its
 * SAX-like interface.
 *
 * This module makes on-the-fly the translation to XHTML and 
 * checks it. As a result, an XHTML tree is progressively 
 * constructed in memory (see tree.c and tree.h).
 *
 * This module has also the code necessary to write out the
 * XHTML tree (writeOutput function).
 *
 * This module must be activated first by invoking
 * saxStartDocument() and saxEndDocument() must be invoked
 * before writing the output. 
 *   
 */



#ifndef PROCESADOR_H
#define PROCESADOR_H

#include "xchar.h"
#include "tree.h"

/* árbol del documento */
extern document_t *document;

/* funciones implementadas y documentadas en procesador.c 
 * (these functions are implemented and docummented in procesador.c)
 */
void saxStartDocument(void);
void saxEndDocument(void);
void saxStartElement(const xchar *fullname, xchar **atts);
void saxEndElement(const xchar *name);
void saxReference(const xchar *name);
void saxCharacters(const xchar *ch, int len);
void saxCDataSection(const xchar *ch, int len);
void saxWhiteSpace(void);
void saxComment(const xchar *value);
void saxDoctype(const xchar *data);
void saxXmlProcessingInstruction(const xchar *fullname, xchar **atts);
void saxError(xchar *data);
int  writeOutput(void);
void freeMemory(void);

#endif


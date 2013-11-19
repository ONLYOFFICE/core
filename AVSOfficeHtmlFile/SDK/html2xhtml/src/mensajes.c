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
 * mensajes.c
 *
 * (Jesús Arias Fisteus)
 * 
 * Functions that write messages (debug, errors, warnings, etc.)
 *
 */
#include <stdio.h>

#include "mensajes.h"


int num_warning = 0;
int num_inform = 0;

extern unsigned int tree_allocated_memory();

void write_end_messages(void)
{
  EPRINTF3("!!TOTAL: warnings(%d) informs(%d) memory(%d B)\n",
	   num_warning, num_inform, tree_allocated_memory());

  if (num_warning) 
    fprintf(stderr, "WARNING: the output file might not be valid XHTML\n");
}


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
 * tree.c
 *
 * (Jesús Arias Fisteus)
 *
 * tipos de datos y funciones necesarios para construir
 * y navegar por un árbol que represente el documento.
 *
 * El documento XHTML resultante de la conversión se
 * va almacenando en memoria en forma de árbol.
 * Los nodos del árbol serán estructuras de datos que
 * pueden representar a un elemento, datos textuales
 * o un comentario. El primer nodo del árbol será
 * un nodo especial que contiene el tipo de documento
 * y un puntero al primer nodo (elemento raíz).
 * A su vez, los nodos de elemento tendrán un puntero
 * a una lista enlazada de nodos de atributo, que contienen
 * el identificador y el valor de un atributo. 
 *
 * En este fichero se encuentran las funciones necesarias
 * para acceder a este árbol.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mensajes.h"
#include "xchar.h"
#include "tree.h"

  

/* buffer de datos 16384 bloques de 64 KB */
#define NUM_DATA_BUFFERS        16384
#define DATA_BUFFER_SIZE        65536
#define BUF_POS(ptr)            ((ptr)&0xFFFF)
#define BUF_NUM(ptr)            (((ptr)&0x3FFF0000)>>16)
#define BUF_PTR(num,pos)        ((((num)&0x3FFF)<<16)|((pos)&0xFFFF))


static unsigned char *data_buffer[NUM_DATA_BUFFERS]; 
static int num_buffer, pos_buffer;
static int active = 0;

static buff_index_t get_data_buffer(int len, const unsigned char *data);


static void *my_malloc_internal(size_t size);


/*
 * inicializa las estructuras de datos
 *
 * ¡hay que ejecutarlo antes que cualquier
 * otra función de este módulo!
 *
 * No se ejecuta (se ignora) si no se liberó memoria desde
 * la última inicialización.
 *
 */
void tree_init(void)
{
  int i;

  /* 
   * pone a NULL todos los buffers menos el primero
   *
   */ 
  if (!active) {
    for (i=1; i<NUM_DATA_BUFFERS; i++) data_buffer[i]= NULL;
    
    
    data_buffer[0]= my_malloc_internal(DATA_BUFFER_SIZE);
    num_buffer= 0;
    pos_buffer= 0;
    active = 1;
  }
}





/*
 * crea un nuevo árbol de documento
 * devuelve un puntero a él, que será necesario
 * para realizar operaciones futuras sobre él
 *
 */
document_t *new_tree_document(int xhtml_type, int html_vers)
{
  document_t *doc;
  
  doc= (document_t*)tree_malloc(sizeof(document_t));
  doc->xhtml_doctype= xhtml_type;
  doc->html_version= html_vers;
  doc->inicio= NULL;

  doc->encoding[0]= 0;

  return doc;
}




/*
 * crea un nuevo nodo para el árbol de documento
 *
 * tipo:
 *  - node_element
 *  - node_chardata
 *
 */
tree_node_t *new_tree_node(node_type_t tipo)
{
  tree_node_t *nodo;

  nodo= tree_malloc(sizeof(tree_node_t));
  nodo->tipo= tipo;
  nodo->padre= NULL;
  nodo->sig= NULL;


  switch (tipo) {
  case Node_comment:
  case Node_chardata:
    nodo->cont.chardata.data= -1;
    break;

  case Node_element:
    nodo->cont.elemento.attlist= NULL;
    nodo->cont.elemento.hijo= NULL;
    break;
  }


  return nodo;
}


/*
 * enlaza un nuevo nodo de atributos en un nodo de elemento
 *
 */
void tree_set_node_att(tree_node_t *nodo, int att_id, const xchar *value,
		       int is_valid)
{
  att_node_t *p, *att;


  /* busca dónde insertar el atributo */
  if (nodo->cont.elemento.attlist) {
    for (p=nodo->cont.elemento.attlist;
	 p->sig && (p->att_id!=att_id ); 
	 p= p->sig);
    if (p->sig) {
      /* atributo repetido: no lo inserta */
      INFORM("repeated attribute");
      return;
    } else {
      att= (att_node_t*) tree_malloc(sizeof(att_node_t));
      att->sig= NULL;
      p->sig= att;
    }
  }
  else {
    att= (att_node_t*) tree_malloc(sizeof(att_node_t));
    att->sig= NULL;
    nodo->cont.elemento.attlist= att;
  }

  /* rellena los campos */
  att->att_id= att_id;
  att->es_valido= is_valid;
  att->valor= get_data_buffer(xstrsize(value), value);

  if (att->valor < 0) WARNING("the attribute value is too big");
}



/*
 * establece los datos de un nodo de datos (Node_chardata)
 *
 */
void tree_set_node_data(tree_node_t *nodo, const xchar *data, int len_data)
{
  if (len_data > DATA_BUFFER_SIZE) 
    WARNING("the data block is too big");

  nodo->cont.chardata.data_len= len_data;
  nodo->cont.chardata.data= get_data_buffer(len_data, data);
}


/*
 * crea y enlaza uno o más nodos de datos o comentario consecutivos.
 * si es necesario, se parte el nodo de datos en varios bloques.
 * Todos aquellos nodos con tamaño posiblemente superior 
 * al tamaño de bloque DATA_BUFFER_SIZE deben ser creados y enlazados
 * mediante esta función.
 */
void tree_link_data_node(node_type_t tipo, tree_node_t *actual_element,
			 const xchar *data, int len_data)
{
  int node_size;
  int saved = 0;

  if (len_data > DATA_BUFFER_SIZE - pos_buffer) {
    node_size = DATA_BUFFER_SIZE - pos_buffer;
  } else {
    node_size = len_data;
  }

  while (saved < len_data) {
    tree_node_t *node = new_tree_node(tipo);
    link_node(node, actual_element, LINK_MODE_CHILD);
    tree_set_node_data(node, &data[saved], node_size);
    saved += node_size;
    if (len_data - saved > DATA_BUFFER_SIZE - pos_buffer) {
      node_size = DATA_BUFFER_SIZE - pos_buffer;
    } else {
      node_size = len_data - saved;
    }
  }
}



/*
 * establece el elemento padre del documento
 *
 * si el documento ya tiene algo, no lo enlaza
 *
 */
void link_root_node(document_t *doc, tree_node_t *nodo)
{
  if (!doc->inicio) doc->inicio= nodo;
}



/*
 * enlaza 'nodo' en el árbol sobre 'to':
 *
 * mode:
 *  - LINK_MODE_CHILD: enlaza como (último) hijo de 'to'
 *  - LINK_MODE_BROTHER: enlaza como (siguiente) hermano de 'to'
 *  - LINK_NODE_FIRST_CHILD: enlaza como (primer) hijo de 'to'
 *
 */
void link_node( tree_node_t *nodo, tree_node_t *to, int mode)
{
  if (mode==LINK_MODE_CHILD) {

    nodo->padre= to;
    nodo->sig= NULL;

    if (to->cont.elemento.hijo) {
      tree_node_t *p;

      /* busca el último hijo */
      for (p= to->cont.elemento.hijo; p->sig; p= p->sig);
      p->sig= nodo;
    } else to->cont.elemento.hijo= nodo;
  }

  else if (mode==LINK_MODE_BROTHER) {
    nodo->sig= to->sig;

    to->sig= nodo;
    nodo->padre= to->padre;
  }

  else if (mode==LINK_MODE_FIRST_CHILD) {
    nodo->sig= to->cont.elemento.hijo;
    to->cont.elemento.hijo= nodo;
  }

}



/**
 * desenlaza el nodo del árbol (no puede ser el nodo raíz)
 * 
 * si tiene hijos, se mantienen unidos a él
 *
 */
void tree_unlink_node(tree_node_t *node)
{
  tree_node_t *padre,*hermano;
  
  if (!(padre= node->padre)) return;
  
  for (hermano= padre->cont.elemento.hijo; hermano && hermano->sig!=node; 
       hermano= hermano->sig);
  
  if (!hermano && (padre->cont.elemento.hijo!=node))
    EXIT("error in the structure of the tree");

  if (!hermano) padre->cont.elemento.hijo= node->sig;
  else {
    hermano->sig= node->sig;
  }
}



/*
 * busca el elemento con id elm_id desde el nodo actual
 * subiendo sucesivamente al padre
 *
 * devuelve el elemento o NULL si se alcanza el root
 * del documento
 *
 */
tree_node_t *tree_search_elm_up(tree_node_t *src, int elm_id)
{
  tree_node_t *p;

  /* busca */
  for (p=src; (p) && (p->cont.elemento.elm_id!=elm_id); p=p->padre); 

  return p;
}


/*
 * busca el elemento con id elm_id hijo del nodo src
 *
 * devuelve el elemento o NULL si no se encuentra
 *
 */
tree_node_t *tree_search_elm_child(tree_node_t *padre, int elm_id)
{
  tree_node_t *p;

  /* busca */
  for (p=padre->cont.elemento.hijo; 
       (p) && ((p->tipo!=Node_element) || (ELM_ID(p)!=elm_id)); 
       p=p->sig); 

  return p;
}


/*
 * recorre el árbol desde src hacia abajo
 *
 * devuelve:
 *    
 * NODE_NONE        fin de árbol
 * NODE_CHILD       hijo
 * NODE_BROTHER     hermano
 * NODE_FATHER      retorno al padre
 *
 * y actualiza el puntero al nuevo nodo 
 *
 * (si *src==NULL, devuelve NODE_NONE)
 *
 * va hacia (hasta que uno sea no nulo
 *   - src->hijo si (allow_child==1)
 *   - src->sig
 *   - src->father
 *   - NULL
 *
 */
int tree_walk(tree_node_t **src, int allow_child)
{
  if (!(*src)) return NODE_NONE;

  /* si tiene hijos, y (allow_child==1) se va hacia ellos */
  if ((allow_child)&&((*src)->tipo==Node_element)
      &&((*src)->cont.elemento.hijo)) {
    *src= (*src)->cont.elemento.hijo;
    return NODE_CHILD;
  }

  /* si tiene hermanos, se va hacia ellos */
  if ((*src)->sig) {
    *src= (*src)->sig;
    return NODE_BROTHER;
  }

  /* si no, al padre */
  if ((*src)->padre) {
    *src= (*src)->padre;
    return NODE_FATHER;
  }

  /* se acabó el árbol */
  *src= NULL;
  return NODE_NONE;
}





/*
 * devuelve un puntero a la zona de datos
 * apuntada por el índice 'index' en el
 * buffer de datos
 *
 */
void *tree_index_to_ptr(buff_index_t index)
{
  if (index<0) return NULL;
  else return (void*)(data_buffer[BUF_NUM(index)]+BUF_POS(index));
}





/*
 * busca en la lista de atributos de un nodo de elemento
 * aquel cuyo id sea att_id
 *
 * devuelve un puntero al nodo o NULL si no se encuentra
 *
 */
att_node_t *tree_node_search_att(tree_node_t *node, int att_id)
{
  att_node_t *p;

  for (p= node->cont.elemento.attlist;
       p && (p->att_id!=att_id); 
       p= p->sig);

  return p;
}

/*
 * Sustituto para malloc(size_t size)
 *
 */
void *tree_malloc(size_t size)
{
  void *p;

  p= tree_index_to_ptr(get_data_buffer(size, NULL));
  if (!p) {
    EXIT("out of memory in tree_malloc()");
    return NULL;
  } else return p;
}

/*
 * Sustituto para strdup(char *str)
 * "str" debe acabar en 0.
 * invoca a tree_malloc: no debe ser liberado con free()
 *
 */
char *tree_strdup(const char *str)
{
  char *dup;
  int i;
  
  dup = (char*) tree_malloc(strlen(str) + 1);

  if (!dup) {
    EXIT("out of memory in tree_strdup()");
  }

  for (i = 0; str[i]; i++) {
    dup[i] = str[i];
  }
  dup[i] = 0;

  return dup;
}


/*
 * devuelve el número de bytes de memoria utilizados a través
 * de este módulo.
 *
 */
unsigned int tree_allocated_memory()
{
  return num_buffer * DATA_BUFFER_SIZE + pos_buffer;
}


/*
 * libera la memoria reservada
 *
 */
void tree_free(void)
{
  int i;

  if (active) {
    EPRINTF1("Memory blocks freed: %d\n", num_buffer + 1);
    
    for (i = 0; i < num_buffer; i++) {
      free(data_buffer[i]);
    }
    active = 0;
  }
}







/*
 * ==============================================================
 * funciones internas
 * ==============================================================
 *
 */


/*
 * devuelve un índice al buffer de datos interno
 * o <0 si hay error (bloque de datos demasiado grande)
 *
 * aborta si se agota completamente la memoria
 *
 */ 
static buff_index_t get_data_buffer(int len, const unsigned char *data)
{
  buff_index_t buff_ptr;

  if (len>DATA_BUFFER_SIZE) return -1;

  if ((pos_buffer+len) > DATA_BUFFER_SIZE) {
    /* nuevo buffer */
    INFORM("creating a new memory block");
    EPRINTF1("%d memory blocks allocated\n", num_buffer + 1);
    num_buffer++;
    if (num_buffer>=NUM_DATA_BUFFERS)
      EXIT("get_data_buffer(), no more data buffers available");

    data_buffer[num_buffer]= my_malloc_internal(DATA_BUFFER_SIZE);
    pos_buffer= 0;
  }


  buff_ptr= BUF_PTR(num_buffer, pos_buffer);
  if (data) memcpy(data_buffer[num_buffer]+pos_buffer, data, len);
  pos_buffer+= len;

  return buff_ptr;
} 



static void *my_malloc_internal(size_t size)
{
  void *p;

  p= malloc(size);
  if (!p) {
    EXIT("out of memory");
    return NULL;
  } else return p;
}

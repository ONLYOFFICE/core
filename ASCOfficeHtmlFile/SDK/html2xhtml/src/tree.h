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
 * tree.h
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
 * para acceder a este árbol (prototipos) y todos los
 * tipos de datos que representan los nodos del árbol.
 *
 */


#ifndef TREE_H
#define TREE_H

#include "xchar.h"



typedef int buff_index_t;



/*
 * un nodo para la lista de
 *  atributos de un elemento
 *
 */ 
struct att_node_{
  int att_id;
  int es_valido;
  struct att_node_ *sig;
  buff_index_t valor;
};

typedef struct att_node_ att_node_t;




/*
 * datos específicos para un elemento
 * en tree_node_t
 *
 */
typedef struct {
  int elm_id;
  att_node_t *attlist;
  struct tree_node_ *hijo;
} node_element_t;





/*
 * datos específicos para datos 
 * de caracteres en tree_node_t
 *
 */
typedef struct {
  int data_len;
  int is_cdata_sec;
  buff_index_t data;
} node_chardata_t;



/*
 * tipos de nodo
 *
 */
typedef enum {
  Node_element,
  Node_chardata,
  Node_cdata_sec,
  Node_comment
} node_type_t;


/*
 * un nodo del árbol
 *
 */
struct tree_node_{

  node_type_t tipo;
  struct tree_node_ *padre;
  struct tree_node_ *sig;   /* siguiente hermano */

  union {
    node_element_t elemento;
    node_chardata_t chardata;
  } cont;
};

typedef struct tree_node_ tree_node_t;




/*
 * raíz para todo elemento
 *
 */
typedef struct {

  int  xhtml_doctype;
  int  html_version;
  char encoding[32];


  tree_node_t *inicio;  /* puntero al primer nodo del doc */

} document_t;








/*
 * ========================================================
 *  funciones asociadas
 * ========================================================
 *
 */



/*
 * inicializa las estructuras de datos
 *
 * ¡hay que ejecutarlo antes que cualquier
 * otra función de este módulo!
 *
 */
void tree_init(void);




/* 
 * crea un nuevo árbol y devuelve su puntero
 *
 */
document_t *new_tree_document(int xhtml_type, int html_vers);




/*
 * crea un nuevo nodo para el árbol, pero NO LO ENLAZA
 *
 * tipo:
 *  - node_element
 *  - node_chardata
 *  
 */
tree_node_t *new_tree_node(node_type_t tipo);



/*
 * establece el elemento padre del documento
 *
 * si el documento ya tiene algo, no lo enlaza
 *
 */
void link_root_node(document_t *doc, tree_node_t *nodo);




/*
 * enlaza 'nodo' en el árbol sobre 'to':
 *
 * mode:
 *  - LINK_MODE_CHILD: enlaza como (último) hijo de 'to'
 *  - LINK_MODE_BROTHER: enlaza como (siguiente) hermano de 'to'
 *  - LINK_NODE_FIRST_CHILD: enlaza como (primer) hijo de 'to' 
 *
 */
#define LINK_MODE_CHILD       1
#define LINK_MODE_BROTHER     2
#define LINK_MODE_FIRST_CHILD 3
void link_node( tree_node_t *nodo, tree_node_t *to, int mode);

/*
 * crea y enlaza uno o más nodos de datos o comentario consecutivos.
 * si es necesario, se parte el nodo de datos en varios bloques.
 * Todos aquellos nodos con tamaño posiblemente superior 
 * al tamaño de bloque DATA_BUFFER_SIZE deben ser creados y enlazados
 * mediante esta función.
 */
void tree_link_data_node(node_type_t tipo, tree_node_t *actual_element,
			 const xchar *data, int len_data);

/**
 * desenlaza el nodo del árbol (no puede ser el nodo raíz)
 * 
 * si tiene hijos, se mantienen unidos a él
 *
 */
void tree_unlink_node(tree_node_t *node);

/*
 * enlaza un nuevo nodo de atributos en un nodo de elemento
 *
 */
void tree_set_node_att(tree_node_t *nodo, int att_id, const xchar *value,
		       int is_valid);
/*
 * establece los datos de un nodo de datos (Node_chardata)
 *
 */
void tree_set_node_data(tree_node_t *nodo, const xchar *data, int len_data);

/*
 * busca el elemento con id elm_id desde el nodo src
 * subiendo sucesivamente al padre
 *
 * devuelve el elemento o NULL si se alcanza el root
 * del documento
 *
 */
tree_node_t *tree_search_elm_up(tree_node_t *src, int elm_id);

/*
 * busca el elemento con id elm_id hijo del nodo src
 *
 * devuelve el elemento o NULL si no se encuentra
 *
 */
tree_node_t *tree_search_elm_child(tree_node_t *padre, int elm_id);




#define NODE_NONE          0  /* fin de árbol */
#define NODE_CHILD         1  /* hijo */
#define NODE_BROTHER       2  /* hermano */
#define NODE_FATHER        3  /* padre */
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
int tree_walk(tree_node_t **src, int allow_child);

/*
 * devuelve un puntero a la zona de datos
 * apuntada por el índice 'index' en el
 * buffer de datos
 *
 */
void *tree_index_to_ptr(buff_index_t index);

/*
 * busca en la lista de atributos de un nodo de elemento
 * aquel cuyo id sea att_id
 *
 * devuelve un puntero al nodo o NULL si no se encuentra
 *
 */
att_node_t *tree_node_search_att(tree_node_t *node, int att_id);

/*
 * Sustituto para malloc(size_t size)
 *
 */
void *tree_malloc(size_t size);

/*
 * Sustituto para strdup(char *str)
 * "str" debe acabar en 0.
 * invoca a tree_malloc: no debe ser liberado con free()
 *
 */
char *tree_strdup(const char *str);

/*
 * devuelve el número de bytes de memoria utilizados a través
 * de este módulo.
 *
 */
unsigned int tree_allocated_memory();

/*
 * libera la memoria reservada
 *
 */
void tree_free(void);

/*
 * macros
 *
 */

#define ELM_ID(tree_node)     (tree_node->cont.elemento.elm_id)
#define ELM_PTR(tree_node)    (elm_list[tree_node->cont.elemento.elm_id])

#define ATT_PTR(att_node)     (att_list[att_node]->att_id])








/*
 * =============================================================
 * variables
 * =============================================================
 *
 */



#endif

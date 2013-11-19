%start input

%{
#include <stdio.h>
#include <string.h>
#include "xchar.h"
#include "mensajes.h"

#include "procesador.h"

#define HTML_FIELD_NAME "html"

extern char *tree_strdup(const char *str);
extern void *tree_malloc();
extern int cgi_mode;

/* lista de atributos del elemento actual */
#define MAX_ELEMENT_ATTRIBUTES  255
static void setAttributeData(char *data);
/* static void freeAttributeData(void); */

static char *element_attributes[MAX_ELEMENT_ATTRIBUTES];
static int num_element_attributes= 0;
static char *content= NULL;

/* para establecer modo script en el lexer */
void lexer_begin_script(char *nombre);
void lexer_cgi_begin_html(void);

/* establece un parámetro */
void cgi_parse_param(char *nombre, char *valor);

/* busca name="nombre" y devuelve un puntero a "nombre", o NULL */
static char *get_nombre(char *cadena);

/* control de <PRE> */
extern int pre_state;


#ifdef DEBUG_MEM
#define free(x)      fprintf(stderr,"==%p  [%s]\n",x,x); free(x)  
#endif

%}

%union {
  int  ent;
  char *cad;
}


%token <cad> TOK_DOCTYPE TOK_COMMENT TOK_BAD_COMMENT TOK_STAG_INI TOK_ETAG TOK_CDATA
%token <cad> TOK_ATT_NAME TOK_ATT_NAMECHAR TOK_ATT_VALUE TOK_EREF TOK_CREF
%token <cad> TOK_CDATA_SEC TOK_XMLPI_INI
%token <ent> TOK_STAG_END TOK_EMPTYTAG_END TOK_ATT_EQ TOK_XMLPI_END
%token <ent> TOK_WHITESPACE
%token <cad> TOK_BOUNDARY TOK_PARAM_HEADER TOK_PARAM_CONTENT

%type <cad> param_header param
 
%%

input: html
| cgi
;

cgi: params TOK_BOUNDARY ;

params: 
| params param 
;

param: param_header TOK_PARAM_CONTENT {
  cgi_parse_param($1, $2 +4);
}
| param_header html 
;

param_header: TOK_BOUNDARY TOK_PARAM_HEADER {
  char* nombre= get_nombre($2);
  if (!strcmp(nombre, HTML_FIELD_NAME)) {
    lexer_cgi_begin_html();
    /* reinitialize the converter to allow the doctype parameter to
     * take effect.
     */
    saxStartDocument();
  }
  $$ = nombre ;
}
;

html: 
| html doctype 
| html comment
| html bad_comment 
| html stag
| html etag
| html cdata
| html cdata_sec
| html whitespace
| html ref
| html xmldecl
;

doctype: TOK_DOCTYPE {
  //fprintf(stderr,"TOK_DOCTYPE: %s\n",$1);
  saxDoctype($1);
}
;

comment: TOK_COMMENT {
  //fprintf(stderr,"TOK_COMMENT: %s\n",$1);
  saxComment($1);
}
;

bad_comment: TOK_BAD_COMMENT {
  //fprintf(stderr,"TOK_BAD_COMMENT: %s\n",$1);
  INFORM("bad comment");
}
;

stag: TOK_STAG_INI attributes TOK_STAG_END {   
  //fprintf(stderr,"STAG-: %s\n",$1);
  setAttributeData(NULL);
  saxStartElement($1,(xchar**)element_attributes);
/*   freeAttributeData(); */
  num_element_attributes = 0;

  /* modo script del lexer (para SCRIPT y STYLE) */
  if ((!strcasecmp($1,"script")) ||(!strcasecmp($1,"style"))) 
    lexer_begin_script($1);

  if (!strcasecmp($1,"pre")) {DEBUG("inicio de modo PRE");pre_state++;}

/*   free($1); */
}
;

etag: TOK_ETAG {
  //fprintf(stderr,"ETAG-: %s\n",$1);
  saxEndElement($1);
  if (!strcasecmp($1,"pre")) {DEBUG("fin de modo PRE");pre_state--;}

/*   free($1); */
}
;

stag: TOK_STAG_INI attributes TOK_EMPTYTAG_END {   
  //fprintf(stderr,"EMTYTAG-: %s\n",$1);
  setAttributeData(NULL);
  saxStartElement($1,(xchar**)element_attributes);
/*   freeAttributeData(); */
  num_element_attributes = 0;
/*   free($1); */
  saxEndElement($1);
}
;

cdata: TOK_CDATA {
  //fprintf(stderr,"CDATA: <%s>\n",$1);
  saxCharacters($1,strlen($1));
}
;

cdata_sec: TOK_CDATA_SEC {
  //fprintf(stderr,"CDATA_SEC: <%s>\n",$1);
  saxCDataSection($1,strlen($1));
}
;

whitespace: TOK_WHITESPACE {
  saxWhiteSpace();
}

ref: TOK_EREF {
  //fprintf(stderr,"EREF: %s\n",$1);
  saxReference($1);
/*   free($1); */
}
| TOK_CREF {
  //fprintf(stderr,"CREF: %s\n",$1);
  saxReference($1);
}
;

xmlpi_end: TOK_XMLPI_END
| TOK_STAG_END
;

xmldecl: TOK_XMLPI_INI attributes xmlpi_end {   
  /*fprintf(stderr,"XMLDECL-: %s\n",$1);*/
  setAttributeData(NULL);
  saxXmlProcessingInstruction($1,(xchar**)element_attributes);
  num_element_attributes = 0;
}
;


attributes: 
| attributes attribute 
;

attribute: TOK_ATT_NAME TOK_ATT_EQ TOK_ATT_VALUE {
                     setAttributeData($1);
                     setAttributeData($3);
                     }
| TOK_ATT_NAME {
                     setAttributeData($1);
                     setAttributeData($1);
}
| TOK_ATT_NAME TOK_ATT_EQ {
                     char *cad= (char*) tree_malloc(1);
                     cad[0]= 0;
                     setAttributeData($1);
                     setAttributeData(cad);                     
}
;

%%




/*
 * setAttributeData
 *
 * inserta 'data' en la lista de atributos de elementos
 * (sólo se mete el puntero, los datos deben seguir en memoria
 * hasta que no sean necesarios)
 *
 */
static void setAttributeData(char *data)
{
  char *ptr;
  
  /* si data==NULL, se cierra la lista de atributos */ 
  if (!data) {
    element_attributes[num_element_attributes]= NULL;
    return;
  }

  if (num_element_attributes == MAX_ELEMENT_ATTRIBUTES-1) {
    EXIT("máximo de atributos de elemento alcanzado\n");
  }

  element_attributes[num_element_attributes++]= data;  
}

/* busca name="nombre" y devuelve un puntero a "nombre", o NULL */
static char *get_nombre(char *cadena) 
{
  char *pos,*tmp;

  pos= strstr(cadena, "name=\"");

  if (pos) {
    pos+= 6;
    for (tmp=pos; *tmp!='\"' && *tmp; tmp++);
    if (*tmp=='\"') *tmp= 0;
    else pos= NULL;
  }

  return pos;
}


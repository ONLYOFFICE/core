%option noyywrap

%{

#include <stdio.h>
#include <string.h>

#include "mensajes.h"
#include "htmlgr.h"

extern char *tree_strdup(const char *str);

int parser_num_bytes=0;
int parser_num_linea=1;
int is_ascii= 1;

int pre_state= 0;
char script_name[32];

void lexer_cgi_init(void);
void lexer_cgi_begin_html(void);

/* funciones internas */
static int cuenta_lineas(char *texto);
static char *trim(char *cad);
static char *trimf(char *cad);
static char *filtcom(char *cad);
static char *comm(char *cad);
static char *cdata_sec(char *str);
static char *etag_val(char *cad);
void lexer_begin_script(char *nombre);
int lexer_end_script(void);
int lexer_is_boundary(char *texto);

char *cgi_boundary= NULL;


#define YY_USER_ACTION {int i;\
                        parser_num_bytes+= yyleng;\
                        parser_num_linea+= cuenta_lineas(yytext);\
                        if (is_ascii) \
                          for(i=0;yytext[i];i++) \
                            if (yytext[i]<0) is_ascii=0; \
                        }


%}



LETTER     ([\x41-\x5a]|[\x61-\x7a]|[\xc0-\xd6]|[\xd8-\xf6]|[\xf8-\xff])
DIGIT      ([\x30-\x39])
EXTENDER   (\xb7)
CHAR       ((\x09)|(\x0a)|(\x0d)|[\x20-\xff])
S          ((\x09)|(\x0a)|(\x0d)|(\x20))
NAMECHAR   ({LETTER}|{DIGIT}|\.|\-|\_|\:|{EXTENDER})
NAME       ({LETTER}|\_|\:){NAMECHAR}*

ALL_IN_TAG ([\x00-\x3d]|[\x3f-\xff])*
IN_COMMENT ([\x00-\x2c]|[\x2e-\xff]|\-([\x00-\x2c]|[\x2e-\xff]))*
IN_BAD_COMMENT {ALL_IN_TAG}
IN_SCRIPT  ([^\<\]]|(\<[^\/!])|(\<![^\-\[])|(\<!\-[^\-])|(\<!\[[^C])|(\<!\[C[^D])|(\<!\[CD[^A])|(\<!\[CDA[^T])|(\<!\[CDAT[^A])|(\<!\[CDATA[^\[])|(\][^\]])|(\]\][^\>]))*
ESP        {S}*

CDATA      ([^\<\]&\x00-\x08\x0b\x0c\x0e-\x1f]|(\][^\]\<&\x00-\x08\x0b\x0c\x0e-\x1f])|(\]\][^\>\<&\x00-\x08\x0b\x0c\x0e-\x1f]))*
BAD_CDATA  ([\x00-\x08]|\x0b|\x0c|[\x0e-\x1f])

CDATASEC   ([^\]]|(\][^\]])|(\]\][^\>]))*
CDATASEC_INI ((\/\/)?(\<!CDATA\[))
CDATASEC_END ((\/\/)?(\]\]\>))

CHAR_REF   (((&#{DIGIT}+)\;)|(&#x({DIGIT}|[a-f]|[A-F])+)\;)
ENTITY_REF (&{NAME}\;)
REF        ({CHAR_REF}|{ENTITY_REF})
ATT_VAL    (\"([^\>\"]|{REF})*\")|('([^\>']|{REF}])*')
ATT_VAL2   ([^\"'\x09\x0a\x0d\x20\>\<])*

CGI_HEADER_LINES  (([^\x0d])*|(\x0d[^\x0a])|(\x0d\x0a[^\x0d])|(\x0d\x0a\x0d[^\x0a]))*
CGI_CONTENT_LINES  \x0d\x0a\x0d\x0a(([^\x0d])*|(\x0d[^\x0a])|(\x0d\x0a[^\-])|(\x0d\x0a\-[^\-]))*
BOUNDARY_LINE ({S}*\-\-[\x20-\xff]*)\x0d\x0a



%x TAG TAG_ATT_VAL SCRIPT SCRIPT_END CGI_CONTENT CGI_HEADER CGI_INIT

%%


{ESP} {if (pre_state) {yylval.cad= yytext; return TOK_CDATA;}
       else {return TOK_WHITESPACE;}}

<TAG_ATT_VAL>{BOUNDARY_LINE} {if (!lexer_is_boundary(yytext)) {
                                BEGIN(TAG);yylval.cad= tree_strdup(filtcom(trim(yytext)));
                                return TOK_ATT_VALUE;
                              } else {BEGIN(CGI_HEADER);return TOK_BOUNDARY;}}

<SCRIPT>{BOUNDARY_LINE} {if (!lexer_is_boundary(yytext)) {
                             yylval.cad= yytext; return TOK_CDATA_SEC;
                         } else {BEGIN(CGI_HEADER);return TOK_BOUNDARY;}}

<CGI_INIT,CGI_HEADER,CGI_CONTENT>{BOUNDARY_LINE} {if (!lexer_is_boundary(yytext)) yyerror(); else {BEGIN(CGI_HEADER);return TOK_BOUNDARY;}}

{BOUNDARY_LINE} {if (!lexer_is_boundary(yytext)) {
                                 yylval.cad= yytext; 
                                 return TOK_CDATA;
                              } else {BEGIN(CGI_HEADER);return TOK_BOUNDARY;}}

"<!DOCTYPE"{ALL_IN_TAG}\>{ESP} {yylval.cad=trimf(yytext);return TOK_DOCTYPE;}
"<!doctype"{ALL_IN_TAG}\>{ESP} {yylval.cad=trimf(yytext);return TOK_DOCTYPE;}
"<!Doctype"{ALL_IN_TAG}\>{ESP} {yylval.cad=trimf(yytext);return TOK_DOCTYPE;}


"<!--"{IN_COMMENT}"--"{ESP}\>  {yylval.cad=comm(yytext);return TOK_COMMENT;}
"<!--"{IN_BAD_COMMENT}"--"{ESP}\> {yylval.cad=comm(yytext);return TOK_BAD_COMMENT;}
"<COMMENT"{ESP}\>{IN_COMMENT}"</COMMENT"{ESP}\> {yylval.cad=yytext;return TOK_COMMENT;}
"<comment"{ESP}\>{IN_COMMENT}"</comment"{ESP}\> {yylval.cad=yytext;return TOK_COMMENT;}




\<{NAME}       {yylval.cad= tree_strdup(trim(yytext+1));BEGIN(TAG);return TOK_STAG_INI;}

\<\/{ESP}{NAME}{ESP}\> {yylval.cad= tree_strdup(etag_val(yytext)); return TOK_ETAG;} 


<TAG>{ESP}{NAME}    {yylval.cad= tree_strdup(trim(yytext));return TOK_ATT_NAME;}
<TAG>{ESP}=         {BEGIN(TAG_ATT_VAL);return TOK_ATT_EQ;}
<TAG_ATT_VAL>{ESP}{ATT_VAL} {BEGIN(TAG);yylval.cad= tree_strdup(filtcom(trim(yytext)));return TOK_ATT_VALUE;}
<TAG_ATT_VAL>{ESP}{ATT_VAL2} {BEGIN(TAG);yylval.cad= tree_strdup(trim(yytext));
                      return TOK_ATT_VALUE;}
<TAG_ATT_VAL>{ESP}\>  {BEGIN(0);return TOK_STAG_END;}
<TAG_ATT_VAL>{ESP}\/\>  {BEGIN(0);return TOK_EMPTYTAG_END;}
<TAG>{ESP}\>        {BEGIN(0);return TOK_STAG_END;}
<TAG>{ESP}\/\>      {BEGIN(0);return TOK_EMPTYTAG_END;}

"<?"{NAME}          {yylval.cad= tree_strdup(trim(yytext+2));BEGIN(TAG);return TOK_XMLPI_INI;}
<TAG>{ESP}"?>"      {BEGIN(0);return TOK_XMLPI_END;}

{CDATA}             {yylval.cad= yytext; return TOK_CDATA;}
"]"                 {yylval.cad= yytext; return TOK_CDATA;}

{ENTITY_REF}   {yylval.cad= tree_strdup(yytext); return TOK_EREF;}
{CHAR_REF}     {yylval.cad= yytext; return TOK_CREF;}


<SCRIPT>"<!--"{IN_COMMENT}"--"{ESP}\>  {yylval.cad=comm(yytext);return TOK_CDATA_SEC;}
<SCRIPT>("<![CDATA["|"//<![CDATA["){CDATASEC}("]]>"|"//]]>")  {yylval.cad=cdata_sec(yytext);return TOK_CDATA_SEC;}
<SCRIPT>{IN_SCRIPT}                    {yylval.cad= yytext; return TOK_CDATA_SEC;}
<SCRIPT>\<\/{ESP}{NAME}{ESP}\>         {return lexer_end_script();}
<SCRIPT>"<!--"                         {yylval.cad= yytext; return TOK_CDATA_SEC;}
<SCRIPT>"</"                           {yylval.cad= yytext; return TOK_CDATA_SEC;}
<SCRIPT>"]]>"                          {yylval.cad= tree_strdup("]]&gt;"); return TOK_CDATA_SEC;}

"<![CDATA["{CDATASEC}"]]>" {yylval.cad=cdata_sec(yytext);return TOK_CDATA_SEC;}

<CGI_HEADER>{CGI_HEADER_LINES} {yylval.cad= yytext; BEGIN(CGI_CONTENT);return TOK_PARAM_HEADER;}
<CGI_CONTENT>{CGI_CONTENT_LINES} {yylval.cad= yytext; BEGIN(CGI_INIT);return TOK_PARAM_CONTENT;}

&                   {yylval.cad= tree_strdup("&amp;"); return TOK_EREF;}
\<                  {yylval.cad= tree_strdup("&lt;"); return TOK_EREF;}
"]]>"               {yylval.cad= tree_strdup("]]&gt;"); return TOK_CDATA;}

{BAD_CDATA}         {DEBUG("carácter descartado");}

<*>.                {/*saxError(yytext);*/INFORM("carácter no emparejado en la entrada");}


%%


/*
 * cuenta el número de líneas del texto
 *
 */ 
static int cuenta_lineas(char *texto)
{
  int num=0;
  int i;

  for (i=0;texto[i];i++)
    num+= (texto[i]=='\n');

  return num;
}
 

/*
 * filtra los espacios al principio 
 * de una cadena de texto
 *
 */
static char *trim(char *cad)
{
   char *resul= cad;

   /* filtra al principio */
   for ( ;(*resul==0x20)||(*resul==0x0a)||(*resul==0x0d)||(*resul==0x09); 
	resul++);

   return resul;
}


/*
 * filtra los espacios al final 
 * de una cadena de texto
 *
 */
static char *trimf(char *cad)
{
   char *resul= cad;

   if (!(*resul)) return cad;

   for (resul= &cad[strlen(cad)-1] ;
        (resul>cad)&&((*resul==0x20)||(*resul==0x0a)||(*resul==0x0d)||
            (*resul==0x09)); 
	resul--);
   
   /* finaliza la cadena */
   *(resul+1)= 0;

   return cad;
}



/*
 * filtra las comillas al principio y al final de 
 * la cadena de texto que recibe
 *
 */
static char *filtcom(char *cad)
{
  cad[strlen(cad)-1]= 0;
  return cad+1;
} 



/*
 * devuelve el nombre de elemento de un tag de fin de elemento
 *
 */
static char *etag_val(char *cad)
{  
   char *resul= cad+2;
   char *cad2;

   /* filtra al principio */
   for ( ;*resul &&((*resul==0x20)||(*resul==0x0a)||(*resul==0x0d)||
            (*resul==0x09)); resul++);

   cad2= resul;

   /* filtra al final */
   for ( ;*cad2 &&(*cad2!=0x20)&&(*cad2!=0x0a)&&(*cad2!=0x0d)&&
            (*cad2!=0x09)&&(*cad2!='>'); cad2++);

   *cad2= 0;

   return resul;
}




/*
 * devuelve el comentario sacando el marcado,
 * es decir, el texto entre <!-- y el siguiente --
 *
 */
char *comm(char *cad)
{
   char *resul;
   
   if ((resul= strstr(cad+4, "--"))) *resul= 0;

   return cad+4;
}


/*
 * returns a CDATA section without its markup,
 * i.e. without "<![CDATA[" and "]]>"
 * (or "//<![CDATA[" and "//]]>")
 *
 */
char *cdata_sec(char *str)
{
   char *result;
   int len;

   if (str[0] == '/')
      str = &str[11];
   else
      str = &str[9];

   if ((result = strstr(str, "]]>"))) {
      if (*(result - 1) == '/' && *(result - 2) == '/')
         *(result - 2) = 0;
      else
         *(result) = 0;
   } 

   return str;
}


/*
 * establece el estado de SCRIPT
 *
 */
void lexer_begin_script(char *nombre)
{
   strcpy(script_name, nombre);
   DEBUG("comienza script");
   BEGIN(SCRIPT);
}


int lexer_end_script(void)
{
  int i,k;
  char car;

  /* busca el nombre */
  for (i=2; yytext[i] && ((yytext[i]==0x20)||(yytext[i]==0x09)||(yytext[i]==0x0a)
                         ||(yytext[i]==0x0d)); i++); 

  /* sigue hasta el final del nombre */
  for (k=i; yytext[k] && (yytext[k]!=0x20)&&(yytext[k]!=0x09)&&(yytext[k]!=0x0a)
                         &&(yytext[k]!=0x0d)&&(yytext[k]!='>'); k++); 

  car= yytext[k];
  yytext[k]= 0;
  
  if (!strcasecmp(&yytext[i],script_name)) {
    BEGIN(0);
    DEBUG("fin de script");
    yylval.cad= tree_strdup(&yytext[i]);
    return TOK_ETAG;
  } else {
    yytext[k]= car;
    yylval.cad= yytext;
    return TOK_CDATA_SEC;
  }
}

void lexer_cgi_begin_html(void) {
  DEBUG("HTML mode set");
  parser_num_linea= -1; /* reset position */
  BEGIN(0);
}

void lexer_cgi_init(void) {
  DEBUG("Lexer initialized in CGI mode");
  BEGIN(CGI_INIT);
}

int lexer_is_boundary(char *texto) {
  int i;

  DEBUG("¿es frontera?");

  if (!cgi_boundary) {
    /* primera aparición */
    cgi_boundary= tree_strdup(trimf(texto));
    return 1;
  }
  else {
    /* se compara */
    if (!strncmp(trim(texto), cgi_boundary, strlen(cgi_boundary))) return 1;
    else 
      return 0;
  }
}

/*
 * Resets the input file for this parser
 *
 */ 
void parser_set_input(FILE *input)
{
  yyin = input;
  yy_flush_buffer(YY_CURRENT_BUFFER);
  BEGIN(0);
}

/***************************************************************************
 *   Copyright (C) 2007 by Jesus Arias Fisteus   *
 *   jaf@it.uc3m.es   *
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
 * html2xhtml.c
 *
 * 'main' module for the command line version of html2xhtml.
 *
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>

#include "procesador.h"
#include "dtd_names.h"
#include "dtd_util.h"
#include "mensajes.h"

#include "xchar.h"

/* parser Yacc / lex */
int yyparse(void);
void parser_set_input(FILE *input);  /* in html.l */

static void set_default_parameters(void);
static void process_parameters(int argc,char **argv);
static void help(void);
static void print_doctypes(void);
void print_version(void);

char *param_charset;
char *param_charset_default;
int   param_strict;
int   param_doctype;
FILE *outputf;
int   param_chars_per_line;
int   param_tab_len;
int   param_pre_comments; /* preserve spacing inside comments */
int   param_protect_cdata;
int   param_cgi_html_output;
int   param_compact_block_elms;

static int cgi_mode;
static int cgi_multipart_input;

#ifdef WITH_CGI
void lexer_cgi_init(void);

void cgi_parse_param(char *nombre, char *valor);
static int is_a_cgi_request(void);
static void cgi_write_error_bad_req(int code);
static void cgi_write_output(void);
static void cgi_write_header(void);
static void cgi_write_footer(void);
static void cgi_exit_on_error(char *msg);
#ifdef CGI_DEBUG
static void cgi_debug_write_input(void);
static void cgi_debug_write_state(void);
#endif
#endif

int main(int argc,char **argv)
{
#ifdef WITH_CGI
  int cgi_valid_request;

  /* check if it is a CGI request or not */
  cgi_valid_request = is_a_cgi_request();
  if (cgi_valid_request) {
    cgi_mode = 1;
    if (cgi_valid_request < 0) {
      cgi_write_error_bad_req(cgi_valid_request);
      return 0;
    } 
  } else {
    cgi_mode = 0;
  }

  set_default_parameters();

  /* initializations, depending on the mode */
  if (!cgi_mode) {
    /* process command line arguments */
    process_parameters(argc, argv); 
  } else {
    tree_init();
    if (cgi_multipart_input)
      lexer_cgi_init();
/*     cgi_debug_write_input(); */
/*     exit(0); */
  }
#else
  cgi_mode = 0;
  set_default_parameters();
  /* process command line arguments */
  process_parameters(argc, argv); 
#endif

  /* intialize the converter */
  saxStartDocument();
  /* parse the input file and convert it */
  yyparse();
  saxEndDocument();

#ifdef WITH_CGI
  if (!cgi_mode) {
    /* write the output */
    if (writeOutput()) 
      EXIT("Bad state in writeOutput()");

    /* close de output file */
    if (outputf != stdout)
      fclose(outputf);
  } else {
    cgi_write_output();
  }
#else
  /* write the output */
  if (writeOutput()) 
    EXIT("Bad state in writeOutput()");
  
  /* close de output file */
  if (outputf != stdout)
    fclose(outputf);
#endif

  /* show final messages */
  write_end_messages();
  freeMemory();

  return 0;
}

static void set_default_parameters()
{
  param_charset = NULL;
  param_charset_default = "ISO-8859-1";
  param_strict = 1;
  param_doctype = -1;
  outputf = stdout;
  param_chars_per_line = 80;
  param_tab_len = 2;
  param_pre_comments = 0;
  param_protect_cdata = 1;
  param_compact_block_elms = 0;
  if (cgi_mode && cgi_multipart_input)
    param_cgi_html_output = 1;
  else
    param_cgi_html_output = 0;
}

static void process_parameters(int argc,char **argv)
{
  int i, fich, tmpnum;

  /* process command line arguments */
  for (i=1, fich=0; i<argc; i++) {
    if (!strcmp(argv[i],"-e")) {
      param_strict= 0;
    } else if (!strcmp(argv[i],"-c") && ((i+1)<argc)) {
      i++;
      param_charset= argv[i];
    } else if (!strcmp(argv[i],"-t") && ((i+1)<argc)) {
      i++;
      param_doctype= dtd_get_dtd_index(argv[i]);
    } else if (!strcmp(argv[i],"-d") && ((i+1)<argc)) {
      i++;
      param_charset_default= argv[i];
    } else if (!strcmp(argv[i],"-o") && ((i+1)<argc)) {
      i++;
      /* open the output file */
      outputf = fopen(argv[i], "w");
      if (!outputf) {
	perror("fopen");
	EXIT("Could not open the output file for writing");
      }
    } else if (!strcmp(argv[i],"-l") && ((i+1)<argc)) {
      i++;
      tmpnum= atoi(argv[i]);
      if (tmpnum >= 40)
	param_chars_per_line= tmpnum; 
    } else if (!strcmp(argv[i],"-b") && ((i+1)<argc)) {
      i++;
      tmpnum= atoi(argv[i]);
      if (tmpnum >= 0 && tmpnum <= 16)
	param_tab_len= tmpnum; 
    } else if (!strcmp(argv[i],"--preserve-space-comments")) {
      param_pre_comments = 1;
    } else if (!strcmp(argv[i],"--no-protect-cdata")) {
      param_protect_cdata = 0;
    } else if (!strcmp(argv[i],"--compact-block-elements")) {
      param_compact_block_elms = 1;
    } else if (!fich && argv[i][0]!='-') {
      fich= 1;
      parser_set_input(fopen(argv[i],"r"));
    } else if (!strcmp(argv[i],"--help") || !strcmp(argv[i],"-h")) {
      help();
      exit(0);
    } else if (!strcmp(argv[i],"--version") || !strcmp(argv[i],"-v")) {
      print_version();
      exit(0);
    } else {
      help();
      exit(1);
    }
  } 
}

int yyerror (char *e)
{
  EXIT(e);
}


void exit_on_error(char *msg)
{
#ifdef WITH_CGI
  if (cgi_mode)
    /* this function exits the program */
    cgi_exit_on_error(msg); 
#endif

  fprintf(stderr,"!!%s(%d)[l%d]: %s\n",__FILE__,__LINE__,
	  parser_num_linea,msg);

  write_end_messages();
  freeMemory();

  exit(1);
}

static void help(void)
{
  print_version();
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "html2xhtml [<input_html_file>] [-c <input_encoding>]\n"); 
  fprintf(stderr, "           [-d <input_encoding>] [-t <output_doctype_key>] [-e]\n");
  fprintf(stderr, "           [-o <output_file>]\n");
  fprintf(stderr, "           [-l <line_length>] [-b <tab_length>]\n");
  fprintf(stderr, "           [--preserve-space-comments] [--no-protect-cdata]\n");
  fprintf(stderr, "           [--compact-block-elements]\n");
  fprintf(stderr, "\n");
  print_doctypes();
}

static void print_doctypes(void)
{
  int i;
  fprintf(stderr, "Setting output doctype:\n");
  for (i = 0; i < XHTML_NUM_DTDS; i++) {
    fprintf(stderr, "'-t %s': document type %s.\n",
	    dtd_key[i], dtd_name[i]);
  }
}

void print_version(void)
{
  fprintf(stderr, "html2xhtml version %s\n\n", VERSION);
}


void cgi_parse_param(char *name, char *value) 
{
#ifdef WITH_CGI
  int tmpnum;

  if (cgi_mode) {
    DEBUG("cgi_param()");
    EPRINTF2("name: %s, value: %s\n", name, value);
    
    if (!strcmp(name, "type") || !strcmp(name, "tipo")) {
      tmpnum = dtd_get_dtd_index(value);
      if (tmpnum >= 0)
	param_doctype = tmpnum;
    } else if ((!strcmp(name, "salida") || !strcmp(name, "output"))
	       && !strcmp(value, "plain")) {
      param_cgi_html_output = 0;
      EPRINTF("param_cgi_html_output: deactivated\n");
    } else if (!strcmp(name,"linelength")) {
      tmpnum= atoi(value);
      if (tmpnum >= 40)
	param_chars_per_line= tmpnum; 
    } else if (!strcmp(name,"tablength")) {
      tmpnum= atoi(value);
      if (tmpnum >= 0 && tmpnum <= 16)
	param_tab_len= tmpnum; 
    }
  } else {
    EXIT("Error: multipart/form-data input but CGI mode has not been detected");
  }
#else
  EXIT("Error: multipart/form-data input but not compiled with CGI support");
#endif
}


#ifdef WITH_CGI

/*
 * Checks if the request comes from the CGI interface.
 *
 * Returns:
 *          0 if it is not a CGI request
 *          1 if it is a valid CGI request
 *         -1 if it is a non-valid CGI request (bad method)
 *         -2 if it is a non-valid CGI request because of other reasons
 *
 */  
static int is_a_cgi_request() {
  char *method;
  char *type;
  char *query_string;
  char *l;
  int length= -1;

  method = getenv("REQUEST_METHOD");
  type = getenv("CONTENT_TYPE");
  query_string = getenv("QUERY_STRING");

  if (type && method && query_string) {
    l= getenv("CONTENT_LENGTH");
    if (l) 
      length= atoi(l);

    if (strcasecmp(method,"POST"))
      return -1; /* bad method */

    if (length <= 0) {
      return -2;
    }

    if (!strncmp(type, "multipart/form-data", 19))
      cgi_multipart_input = 1;
    else if (!strncmp(type,"text/html", 9))
      cgi_multipart_input = 0;
    else
      return -2;
    
    return 1;
  } else {
    return 0;
  }
} 

static void cgi_write_error_bad_req(int code)
{
  fprintf(stdout,"Content-Type:%s\n","text/html");
  
  /* invalid request */
  if (code == -1) { /* method != POST */
    fprintf(stdout,"Status:405 Method not allowed\n\n");
    if (param_cgi_html_output) {
      fprintf(stdout,"<html><head><title>html2xhtml-Error</title></head><body>");
      fprintf(stdout,"<h1>405 Method not allowed</h1></body></html>");
    }
  }
  else {
    fprintf(stdout,"Status:400 Bad request\n\n");
    if (param_cgi_html_output) {
      fprintf(stdout,"<html><head><title>html2xhtml-Error</title></head><body>");
      fprintf(stdout,"<h1>400 Bad Request</h1></body></html>");
    }
  }  
}

static void cgi_exit_on_error(char *msg)
{  
  fprintf(stdout,"Content-Type:%s\n","text/html");
  fprintf(stdout,"Status:400 Bad request\n\n");
  fprintf(stdout,"<html><head><title>html2xhtml-Error</title></head><body>");
  fprintf(stdout,"<h1>400 Bad Request</h1>");
  fprintf(stdout,"<p>An error has been detected while parsing the input");
  if (parser_num_linea > 0) 
    fprintf(stdout," at line %d. Please, ", parser_num_linea);
  else fprintf(stdout,". Please, ");
  fprintf(stdout,"check that you have uploaded a HTML document.</p>");
  if (msg)
    fprintf(stdout,"<p>Error: %s</p>", msg);

#ifdef CGI_DEBUG
  cgi_debug_write_state();
#endif

  fprintf(stdout,"</body></html>");  

  freeMemory();

  exit(0);
}

static void cgi_write_output()
{
  fprintf(stdout, "Content-Type:%s; charset=iso-8859-1\n\n", "text/html");

  if (param_cgi_html_output) 
    cgi_write_header();

  /* write the XHTML output */
  if (writeOutput()) 
    EXIT("Incorrect state in writeOutput()");

  if (param_cgi_html_output)
    cgi_write_footer();
}

static void cgi_write_header() 
{
  fprintf(stdout, "<?xml version=\"1.0\"");
  if (document->encoding[0]) 
    fprintf(outputf," encoding=\"%s\"", document->encoding);
  fprintf(outputf,"?>\n\n");

  fprintf(stdout,
"<!DOCTYPE html\n\
   PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n\
   \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n\n");

  fprintf(stdout,
"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n\
  <head>\n\
    <title>html2xhtml - page translated</title>\n\
    <link type=\"text/css\" href=\"/jaf/xhtmlpedia/xhtmlpedia.css\" rel=\"stylesheet\"/>\n\
  </head>\n");
  
  fprintf(stdout,
" <body>\n\
    <table class=\"navigation\">\n\
      <tr>\n\
        <td class=\"nav-left\"><a href=\"/jaf/html2xhtml/\">back to main page</a></td>\n\
        <td class=\"nav-center\"><a href=\"/jaf/xhtmlpedia/index.html\">go to the xhtmlpedia</a></td>\n\
        <td class=\"nav-right\"><a href=\"/jaf/html2xhtml/download.html\">download html2xhtml</a></td>\n\
      </tr>\n\
    </table>");

  fprintf(stdout,
"    <div class=\"title\">\n\
      <h1>html2xhtml</h1>\n\
      <p>The document has been converted</p>\n\
    </div>\n");

  fprintf(stdout,
"    <p>The input document has been succesfully converted. If you want\n\
      to save it in a file, copy and paste it in a text editor.\n\
      You can also <a href=\"/jaf/html2xhtml/download.html\">download\n\
      html2xhtml</a> and run it in your computer.</p>\n\
    <pre class=\"document\" xml:space=\"preserve\">\n");
}

static void cgi_write_footer() 
{
  fprintf(stdout,
"</pre>\n\
    <p class=\"boxed\">\n\
    <img src=\"/jaf/html2xhtml/h2x.png\" alt=\"html2xhtml logo\" />\n\
      <i>html2xhtml %s</i>, copyright 2001-2008 <a href=\"http://www.it.uc3m.es/jaf/index.html\">Jesús Arias Fisteus</a>; 2001 Rebeca Díaz Redondo, Ana Fernández Vilas\n\
    </p>\n", VERSION);

#ifdef CGI_DEBUG
  cgi_debug_write_state();
#endif

  fprintf(stdout, "</body>\n</html>\n");
}

#ifdef CGI_DEBUG
static void cgi_debug_write_input()
{
  int i;
  int c;

  fprintf(stdout,"Content-Type:%s\n\n","text/plain");

  while (1){
    c=fgetc(stdin);
    if (c==EOF) break;
    fputc(c,stdout);
  }
}

static void cgi_debug_write_state()
{
  fprintf(stdout,"<hr/><p>Internal state:</p>");
  fprintf(stdout,"<ul>");
  fprintf(stdout,"<li>Multipart input: %d</li>", cgi_multipart_input);
  fprintf(stdout,"<li>HTML output: %d</li>", param_cgi_html_output);
  fprintf(stdout,"</ul>");
}
#endif
#endif




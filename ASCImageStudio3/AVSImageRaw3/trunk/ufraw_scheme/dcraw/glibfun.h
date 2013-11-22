/*! \file	flibfun.h
 *	\brief	Функции, выдернутые из библиотеки glib
 *
 *	\todo	Надо убрать совсем
 */

#pragma once


//#include "dcrawcommon.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <io.h>


#define _strcasecmp			_stricmp
#define strcasecmp			_strcasecmp

typedef unsigned int		gsize;
typedef signed int			gssize;

typedef char				gchar;
typedef unsigned char		guchar;

typedef int					gint;
typedef unsigned int		guint;

typedef short				gshort;
typedef unsigned short		gushort;

typedef long				glong;
typedef unsigned long		gulong;

typedef signed char			gint8;
typedef unsigned char		guint8;
typedef signed short		gint16;
typedef unsigned short		guint16;
typedef signed int			gint32;
typedef unsigned int		guint32;
typedef signed __int64		gint64;
typedef unsigned __int64	guint64;
typedef unsigned short		gushort;

typedef void*				gpointer;
typedef signed long			ssize_t;

#define g_slice_new(type)		new type
#define g_slice_free(type,obj)	delete obj


#define g_unlink	_unlink


#define	g_free(m) free(m)
#define g_realloc(m,s)		realloc((m),(s))
#define g_try_realloc(m,s)	realloc((m),(s))

#define g_new(t,s)	((t*)malloc((s)*sizeof(t)))

#define g_new0(t,s)	((t*)calloc((s),sizeof(t)))

#define g_strlcpy(dest,scr,dest_size)	strcpy_s((dest),(dest_size),(scr))
#define g_strdup strdup
#define g_filename_display_name strdup
#define g_locale_to_utf8(opsysstring, len, bytes_read, bytes_written, error) strdup(opsysstring)

#define g_fopen fopen

#define g_snprintf	_snprintf


typedef struct {
	gpointer *pdata;
	guint	    len;
} GPtrArray;

typedef gint   gboolean;
typedef const void *gconstpointer;
typedef gint	(*GCompareFunc) (gconstpointer a, gconstpointer b);


#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#define g_memmove(dest,src,len) { memmove ((dest), (src), (len)); } 

#define G_LIKELY(expr) (expr)
#define G_UNLIKELY(expr) (expr)


#define g_return_val_if_fail(expr, val)		{	\
	if (expr) { } else							\
{												\
	_ASSERT(expr);								\
	return (val);								\
};	}

#define g_return_if_fail(expr)		{	\
	if (expr) { } else							\
{												\
	_ASSERT(expr);								\
	return;								\
};	}


typedef struct _GString		GString;

struct _GString
{
	gchar  *str;
	gsize len;    
	gsize allocated_len;
};

typedef struct _GRealPtrArray  GRealPtrArray;

struct _GRealPtrArray
{
  gpointer *pdata;
  guint     len;
  guint     alloc;
}; 

#define MY_MAXSIZE ((gsize)-1)

static gint
g_nearest_pow (gint num)
{
  gint n = 1;

  while (n < num)
    n <<= 1;

  return n;
}

static inline gsize	nearest_power (gsize base, gsize num)    
{
	if (num > MY_MAXSIZE / 2)
	{
		return MY_MAXSIZE;
	}
	else
	{
		gsize n = base;

		while (n < num)
			n <<= 1;

		return n;
	}
} 


/* Strings.
*/
static void
g_string_maybe_expand (GString* string,
					   gsize    len) 
{
	if (string->len + len >= string->allocated_len)
	{
		string->allocated_len = nearest_power (1, string->len + len + 1);
		string->str = (gchar *)g_realloc (string->str, string->allocated_len);
	}
}

/**
* g_string_sized_new:
* @dfl_size: the default size of the space allocated to 
*            hold the string
*
* Creates a new #GString, with enough space for @dfl_size 
* bytes. This is useful if you are going to add a lot of 
* text to the string and don't want it to be reallocated 
* too often.
*
* Returns: the new #GString
*/
GString*	g_string_sized_new (gsize dfl_size);  

/**
* g_string_insert_len:
* @string: a #GString
* @pos: position in @string where insertion should 
*       happen, or -1 for at the end
* @val: bytes to insert
* @len: number of bytes of @val to insert
* 
* Inserts @len bytes of @val into @string at @pos.  
* Because @len is provided, @val may contain embedded 
* nuls and need not be nul-terminated. If @pos is -1, 
* bytes are inserted at the end of the string.
*
* Since this function does not stop at nul bytes, it is 
* the caller's responsibility to ensure that @val has at 
* least @len addressable bytes.
*
* Returns: @string
*/
GString*	g_string_insert_len (GString     *string,
					 gssize       pos,    
					 const gchar *val,
					 gssize       len);


/**
* g_string_append_len:
* @string: a #GString
* @val: bytes to append
* @len: number of bytes of @val to use
* 
* Appends @len bytes of @val to @string. Because @len is 
* provided, @val may contain embedded nuls and need not 
* be nul-terminated.
* 
* Since this function does not stop at nul bytes, it is 
* the caller's responsibility to ensure that @val has at 
* least @len addressable bytes.
*
* Returns: @string
*/
GString*	g_string_append_len (GString	*string, const gchar *val,	gssize	len);

/**
* g_string_new:
* @init: the initial text to copy into the string
* 
* Creates a new #GString, initialized with the given string.
*
* Returns: the new #GString
*/
GString*	g_string_new (const gchar *init);


#define g_malloc malloc

//extern static const guint16 ascii_table_data[256];

//const guint16 * const g_ascii_table = ascii_table_data;
//extern const guint16 * const g_ascii_table;


/**
* g_path_get_basename:
* @file_name: the name of the file.
*
* Gets the last component of the filename. If @file_name ends with a 
* directory separator it gets the component before the last slash. If 
* @file_name consists only of directory separators (and on Windows, 
* possibly a drive letter), a single separator is returned. If
* @file_name is empty, it gets ".".
*
* Return value: a newly allocated string containing the last component of 
*   the filename.
*/
gchar* g_path_get_basename (const gchar   *file_name);

/**
* g_string_free:
* @string: a #GString
* @free_segment: if %TRUE the actual character data is freed as well
*
* Frees the memory allocated for the #GString.
* If @free_segment is %TRUE it also frees the character data.
*
* Returns: the character data of @string 
*          (i.e. %NULL if @free_segment is %TRUE)
*/
gchar*	g_string_free (GString *string,	gboolean free_segment);

/**
* g_string_append:
* @string: a #GString
* @val: the string to append onto the end of @string
* 
* Adds a string onto the end of a #GString, expanding 
* it if necessary.
*
* Returns: @string
*/
GString*	g_string_append (GString     *string,	 const gchar *val);

static gchar *	g_build_pathname_va (const gchar  *first_element,va_list	*args,gchar	**str_array);

/**
* g_build_filenamev:
* @args: %NULL-terminated array of strings containing the path elements.
* 
* Behaves exactly like g_build_filename(), but takes the path elements 
* as a string array, instead of varargs. This function is mainly
* meant for language bindings.
*
* Return value: a newly-allocated string that must be freed with g_free().
* 
* Since: 2.8
*/
gchar *	g_build_filenamev (gchar **args);


/**
* g_build_filename:
* @first_element: the first element in the path
* @Varargs: remaining elements in path, terminated by %NULL
* 
* Creates a filename from a series of elements using the correct
* separator for filenames.
*
* On Unix, this function behaves identically to <literal>g_build_path
* (G_DIR_SEPARATOR_S, first_element, ....)</literal>.
*
* On Windows, it takes into account that either the backslash
* (<literal>\</literal> or slash (<literal>/</literal>) can be used
* as separator in filenames, but otherwise behaves as on Unix. When
* file pathname separators need to be inserted, the one that last
* previously occurred in the parameters (reading from left to right)
* is used.
*
* No attempt is made to force the resulting filename to be an absolute
* path. If the first element is a relative path, the result will
* be a relative path. 
* 
* Return value: a newly-allocated string that must be freed with g_free().
**/
gchar *	g_build_filename (const gchar *first_element,  ...);

static gboolean	get_contents_stdio (const gchar *display_filename,
					FILE        *f,
					gchar      **contents,
					gsize       *length,
					/*GError*/ void     **error);

static gboolean	get_contents_win32 (const gchar *filename,
					gchar      **contents,
					gsize       *length,
					/*GError*/ void     **error);

/**
* g_file_get_contents:
* @filename: name of a file to read contents from, in the GLib file name encoding
* @contents: location to store an allocated string
* @length: location to store length in bytes of the contents, or %NULL
* @error: return location for a #GError, or %NULL
* 
* Reads an entire file into allocated memory, with good error
* checking. 
*
* If the call was successful, it returns %TRUE and sets @contents to the file 
* contents and @length to the length of the file contents in bytes. The string 
* stored in @contents will be nul-terminated, so for text files you can pass 
* %NULL for the @length argument. If the call was not successful, it returns 
* %FALSE and sets @error. The error domain is #G_FILE_ERROR. Possible error  
* codes are those in the #GFileError enumeration. In the error case, 
* @contents is set to %NULL and @length is set to zero.
*
* Return value: %TRUE on success, %FALSE if an error occurred
**/
gboolean g_file_get_contents (const gchar *filename,
							  gchar      **contents,
							  gsize       *length,
							  /*GError*/void     **error);


/**
* g_path_get_dirname:
* @file_name: the name of the file.
*
* Gets the directory components of a file name.  If the file name has no
* directory components "." is returned.  The returned string should be
* freed when no longer needed.
* 
* Returns: the directory components of the file.
*/
gchar*	g_path_get_dirname (const gchar	   *file_name);

/**
 * g_strndup:
 * @str: the string to duplicate
 * @n: the maximum number of bytes to copy from @str
 *
 * Duplicates the first @n bytes of a string, returning a newly-allocated
 * buffer @n + 1 bytes long which will always be nul-terminated.
 * If @str is less than @n bytes long the buffer is padded with nuls.
 * If @str is %NULL it returns %NULL.
 * The returned value should be freed when no longer needed.
 * 
 * <note><para>
 * To copy a number of characters from a UTF-8 encoded string, use
 * g_utf8_strncpy() instead.
 * </para></note>
 * 
 * Returns: a newly-allocated buffer containing the first @n bytes 
 *          of @str, nul-terminated 
 */
gchar*	g_strndup (const gchar *str, gsize n);

gchar*	g_strconcat (const gchar *string1, ...);

gboolean	g_path_is_absolute (const gchar *file_name);

/**
* g_stpcpy:
* @dest: destination buffer.
* @src: source string.
* 
* Copies a nul-terminated string into the dest buffer, include the
* trailing nul, and return a pointer to the trailing nul byte.
* This is useful for concatenating multiple strings together
* without having to repeatedly scan for the end.
* 
* Return value: a pointer to trailing nul byte.
**/
gchar *	g_stpcpy (gchar	*dest,  const gchar *src);


/**
 * g_markup_vprintf_escaped:
 * @format: printf() style format string
 * @args: variable argument list, similar to vprintf()
 * 
 * Formats the data in @args according to @format, escaping
 * all string and character arguments in the fashion
 * of g_markup_escape_text(). See g_markup_printf_escaped().
 * 
 * Return value: newly allocated result from formatting
 *  operation. Free with g_free().
 *
 * Since: 2.4
 **/
char * g_markup_vprintf_escaped (const char *format, va_list     args);

gchar* g_strdup_vprintf (const gchar *format, va_list args);

/**
* g_vasprintf:
* @string: the return location for the newly-allocated string.
* @format: a standard printf() format string, but notice
*          <link linkend="string-precision">string precision pitfalls</link>.
* @args: the list of arguments to insert in the output.
*
* An implementation of the GNU vasprintf() function which supports 
* positional parameters, as specified in the Single Unix Specification.
* This function is similar to g_vsprintf(), except that it allocates a 
* string to hold the output, instead of putting the output in a buffer 
* you allocate in advance.
*
* Returns: the number of bytes printed.
*
* Since: 2.4
**/
gint	g_vasprintf (gchar      **string, gchar const *format,	va_list      args);

int _g_gnulib_vasprintf (char **result, char const *format, va_list args);

void g_ptr_array_set_size  (GPtrArray   *farray,	gint length);
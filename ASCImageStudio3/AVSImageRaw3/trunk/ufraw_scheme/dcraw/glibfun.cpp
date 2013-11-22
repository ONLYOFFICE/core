/*! \file	flibfun.cpp
 *	\brief	Функции, выдернутые из библиотеки glib
 *
 *	\todo	Надо убрать совсем
 */

#include "stdafx.h"
#include "glibfun.h"

enum __GAsciiType{
	G_ASCII_ALNUM  = 1 << 0,
	G_ASCII_ALPHA  = 1 << 1,
	G_ASCII_CNTRL  = 1 << 2,
	G_ASCII_DIGIT  = 1 << 3,
	G_ASCII_GRAPH  = 1 << 4,
	G_ASCII_LOWER  = 1 << 5,
	G_ASCII_PRINT  = 1 << 6,
	G_ASCII_PUNCT  = 1 << 7,
	G_ASCII_SPACE  = 1 << 8,
	G_ASCII_UPPER  = 1 << 9,
	G_ASCII_XDIGIT = 1 << 10
} ;

typedef __GAsciiType	GAsciiType;

static gboolean g_mem_gc_friendly = FALSE;

static const guint16 ascii_table_data[256] = {
	0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
	0x004, 0x104, 0x104, 0x004, 0x104, 0x104, 0x004, 0x004,
	0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
	0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
	0x140, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
	0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
	0x459, 0x459, 0x459, 0x459, 0x459, 0x459, 0x459, 0x459,
	0x459, 0x459, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
	0x0d0, 0x653, 0x653, 0x653, 0x653, 0x653, 0x653, 0x253,
	0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253,
	0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253,
	0x253, 0x253, 0x253, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
	0x0d0, 0x473, 0x473, 0x473, 0x473, 0x473, 0x473, 0x073,
	0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073,
	0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073,
	0x073, 0x073, 0x073, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x004
	/* the upper 128 are all zeroes */
};

const guint16 * const g_ascii_table = ascii_table_data;

#define G_DIR_SEPARATOR '\\'
#define G_DIR_SEPARATOR_S "\\"
#define G_IS_DIR_SEPARATOR(c) ((c) == G_DIR_SEPARATOR || (c) == '/')

#define g_ascii_isalpha(c)  ((g_ascii_table[(guchar) (c)] & G_ASCII_ALPHA) != 0)


GString* g_string_sized_new (gsize dfl_size)    
{
	GString *string = g_slice_new (GString);

	string->allocated_len = 0;
	string->len   = 0;
	string->str   = NULL;

	g_string_maybe_expand (string, MAX (dfl_size, 2));
	string->str[0] = 0;

	return string;
} 

GString*	g_string_insert_len (GString     *string,
					 gssize       pos,    
					 const gchar *val,
					 gssize       len)    
{
	_ASSERT(string != NULL);
	_ASSERT(val != NULL);

	if (len < 0)
		len = strlen (val);

	if (pos < 0)
		pos = string->len;
	else
	{
		g_return_val_if_fail (pos <= string->len, string);
	}

	/* Check whether val represents a substring of string.  This test
	probably violates chapter and verse of the C standards, since
	">=" and "<=" are only valid when val really is a substring.
	In practice, it will work on modern archs.  */
	if (val >= string->str && val <= string->str + string->len)
	{
		gsize offset = val - string->str;
		gsize precount = 0;

		g_string_maybe_expand (string, len);
		val = string->str + offset;
		/* At this point, val is valid again.  */

		/* Open up space where we are going to insert.  */
		if (pos < string->len)
			g_memmove (string->str + pos + len, string->str + pos, string->len - pos);

		/* Move the source part before the gap, if any.  */
		if (offset < pos)
		{
			precount = MIN (len, pos - offset);
			memcpy (string->str + pos, val, precount);
		}

		/* Move the source part after the gap, if any.  */
		if (len > precount)
			memcpy (string->str + pos + precount,
			val + /* Already moved: */ precount + /* Space opened up: */ len,
			len - precount);
	}
	else
	{
		g_string_maybe_expand (string, len);

		/* If we aren't appending at the end, move a hunk
		* of the old string to the end, opening up space
		*/
		if (pos < string->len)
			g_memmove (string->str + pos + len, string->str + pos, string->len - pos);

		/* insert the new string */
		if (len == 1)
			string->str[pos] = *val;
		else
			memcpy (string->str + pos, val, len);
	}

	string->len += len;

	string->str[string->len] = 0;

	return string;
}



GString*	g_string_append_len (GString	 *string,
					 const gchar *val,
					 gssize       len)    
{
	_ASSERT(string != NULL);
	_ASSERT(val != NULL);

	return g_string_insert_len (string, -1, val, len);
}


GString*	g_string_new (const gchar *init)
{
	GString *string;

	if (init == NULL || *init == '\0')
		string = g_string_sized_new (2);
	else 
	{
		gint len;

		len = strlen (init);
		string = g_string_sized_new (len + 2);

		g_string_append_len (string, init, len);
	}

	return string;
}

gchar* g_path_get_basename (const gchar   *file_name)

{
	register gssize base;             
	register gssize last_nonslash;    
	gsize len;    
	gchar *retval;

	_ASSERT(file_name != NULL);

	if (file_name[0] == '\0')
		/* empty string */
		return g_strdup (".");

	last_nonslash = strlen (file_name) - 1;

	while (last_nonslash >= 0 && G_IS_DIR_SEPARATOR (file_name [last_nonslash]))
		last_nonslash--;

	if (last_nonslash == -1)
		/* string only containing slashes */
		return g_strdup (G_DIR_SEPARATOR_S);

	//#ifdef G_OS_WIN32
#if 1
	if (last_nonslash == 1 && g_ascii_isalpha (file_name[0]) && file_name[1] == ':')
		/* string only containing slashes and a drive */
		return g_strdup (G_DIR_SEPARATOR_S);
#endif /* G_OS_WIN32 */

	base = last_nonslash;

	while (base >=0 && !G_IS_DIR_SEPARATOR (file_name [base]))
		base--;

	//#ifdef G_OS_WIN32
#if 1
	if (base == -1 && g_ascii_isalpha (file_name[0]) && file_name[1] == ':')
		base = 1;
#endif /* G_OS_WIN32 */

	len = last_nonslash - base;
	retval = (gchar*)g_malloc (len + 1);
	memcpy (retval, file_name + base + 1, len);
	retval [len] = '\0';
	return retval;
} 


gchar*	g_string_free (GString *string,	gboolean free_segment)
{
	gchar *segment;

	g_return_val_if_fail (string != NULL, NULL);

	if (free_segment)
	{
		g_free (string->str);
		segment = NULL;
	}
	else
		segment = string->str;

	g_slice_free (sizeof(GString), string);

	return segment;
}

GString*	g_string_append (GString     *string,
				 const gchar *val)
{  
	g_return_val_if_fail (string != NULL, NULL);
	g_return_val_if_fail (val != NULL, string);

	return g_string_insert_len (string, -1, val, -1);
} 

static gchar *	g_build_pathname_va (const gchar  *first_element,va_list	*args,gchar	**str_array)
{
	/* Code copied from g_build_pathv(), and modified to use two
	* alternative single-character separators.
	*/
	GString *result;
	gboolean is_first = TRUE;
	gboolean have_leading = FALSE;
	const gchar *single_element = NULL;
	const gchar *next_element;
	const gchar *last_trailing = NULL;
	gchar current_separator = '\\';
	gint i = 0;

	result = g_string_new (NULL);

	if (str_array)
		next_element = str_array[i++];
	else
		next_element = first_element;

	while (TRUE)
	{
		const gchar *element;
		const gchar *start;
		const gchar *end;

		if (next_element)
		{
			element = next_element;
			if (str_array)
				next_element = str_array[i++];
			else
				next_element = va_arg (*args, gchar *);
		}
		else
			break;

		/* Ignore empty elements */
		if (!*element)
			continue;

		start = element;

		if (TRUE)
		{
			while (start &&
				(*start == '\\' || *start == '/'))
			{
				current_separator = *start;
				start++;
			}
		}

		end = start + strlen (start);

		if (TRUE)
		{
			while (end >= start + 1 &&
				(end[-1] == '\\' || end[-1] == '/'))
			{
				current_separator = end[-1];
				end--;
			}

			last_trailing = end;
			while (last_trailing >= element + 1 &&
				(last_trailing[-1] == '\\' || last_trailing[-1] == '/'))
				last_trailing--;

			if (!have_leading)
			{
				/* If the leading and trailing separator strings are in the
				* same element and overlap, the result is exactly that element
				*/
				if (last_trailing <= start)
					single_element = element;

				g_string_append_len (result, element, start - element);
				have_leading = TRUE;
			}
			else
				single_element = NULL;
		}

		if (end == start)
			continue;

		if (!is_first)
			g_string_append_len (result, &current_separator, 1);

		g_string_append_len (result, start, end - start);
		is_first = FALSE;
	}

	if (single_element)
	{
		g_string_free (result, TRUE);
		return g_strdup (single_element);
	}
	else
	{
		if (last_trailing)
			g_string_append (result, last_trailing);

		return g_string_free (result, FALSE);
	}
} 

gchar *	g_build_filenamev (gchar **args)
{
	gchar *str;

	/*#ifndef G_OS_WIN32
	str = g_build_path_va (G_DIR_SEPARATOR_S, NULL, NULL, args);
	#else*/
#if 1
	str = g_build_pathname_va (NULL, NULL, args);
#endif

	return str;
} 


gchar *	g_build_filename (const gchar *first_element,  ...)
{
	gchar *str;
	va_list args;

	va_start (args, first_element);
	/*#ifndef G_OS_WIN32
	str = g_build_path_va (G_DIR_SEPARATOR_S, first_element, &args, NULL);
	#else*/
#if 1
	str = g_build_pathname_va (first_element, &args, NULL);
#endif
	va_end (args);

	return str;
}

static gboolean	get_contents_stdio (const gchar *display_filename,
					FILE        *f,
					gchar      **contents,
					gsize       *length,
					/*GError*/ void     **error)


{
	gchar buf[4096];
	gsize bytes;
	gchar *str = NULL;
	gsize total_bytes = 0;
	gsize total_allocated = 0;
	gchar *tmp;

	/*g_assert */ _ASSERT(f != NULL);

	while (!feof (f))
	{
		gint save_errno;

		bytes = fread (buf, 1, sizeof (buf), f);
		//save_errno = errno;

		while ((total_bytes + bytes + 1) > total_allocated)
		{
			if (str)
				total_allocated *= 2;
			else
				total_allocated = MIN (bytes + 1, sizeof (buf));

			tmp = (gchar*)g_try_realloc (str, total_allocated);

			if (tmp == NULL)
			{
				/*g_set_error (error,
				G_FILE_ERROR,
				G_FILE_ERROR_NOMEM,
				_("Could not allocate %lu bytes to read file \"%s\""),
				(gulong) total_allocated,
				display_filename);*/
				_ASSERT(FALSE);
				goto error;
			}

			str = tmp;
		}

		if (ferror (f))
		{
			/*g_set_error (error,
			G_FILE_ERROR,
			g_file_error_from_errno (save_errno),
			_("Error reading file '%s': %s"),
			display_filename,
			g_strerror (save_errno));
			*/
			_ASSERT(FALSE);

			goto error;
		}

		memcpy (str + total_bytes, buf, bytes);

		if (total_bytes + bytes < total_bytes) 
		{
			/*g_set_error (error,
			G_FILE_ERROR,
			G_FILE_ERROR_FAILED,
			_("File \"%s\" is too large"),
			display_filename);
			*/
			_ASSERT(FALSE);
			goto error;
		}

		total_bytes += bytes;
	}

	fclose (f);

	if (total_allocated == 0)
	{
		str = g_new (gchar, 1);
		total_bytes = 0;
	}

	str[total_bytes] = '\0';

	if (length)
		*length = total_bytes;

	*contents = str;

	return TRUE;

error:

	g_free (str);
	fclose (f);

	return FALSE;
}

static gboolean	get_contents_win32 (const gchar *filename,
					gchar      **contents,
					gsize       *length,
					/*GError*/ void     **error)
{
	FILE *f;
	gboolean retval;
	gchar *display_filename = g_filename_display_name (filename);
	int save_errno;

	f = g_fopen (filename, "rb");
	//save_errno = errno;

	if (f == NULL)
	{
		/*      g_set_error (error,
		G_FILE_ERROR,
		g_file_error_from_errno (save_errno),
		_("Failed to open file '%s': %s"),
		display_filename,
		g_strerror (save_errno));
		g_free (display_filename);
		*/

		return FALSE;
	}

	retval = get_contents_stdio (display_filename, f, contents, length, error);
	g_free (display_filename);

	return retval;
}


gboolean g_file_get_contents (const gchar *filename,
							  gchar      **contents,
							  gsize       *length,
							  /*GError*/void     **error)
{  
	g_return_val_if_fail (filename != NULL, FALSE);
	g_return_val_if_fail (contents != NULL, FALSE);

	*contents = NULL;
	if (length)
		*length = 0;

	//#ifdef G_OS_WIN32
	return get_contents_win32 (filename, contents, length, error);
	/*#else
	return get_contents_posix (filename, contents, length, error);
	#endif
	*/
}

gchar*	g_path_get_dirname (const gchar	   *file_name)
{
	register gchar *base;
	register gsize len;    

	g_return_val_if_fail (file_name != NULL, NULL);

	base = (gchar*)strrchr (file_name, G_DIR_SEPARATOR);
	//#ifdef G_OS_WIN32
#if 1
	{
		gchar *q = (gchar *)strrchr (file_name, '/');
		if (base == NULL || (q != NULL && q > base))
			base = q;
	}
#endif
	if (!base)
	{
		//#ifdef G_OS_WIN32
#if 1
		if (g_ascii_isalpha (file_name[0]) && file_name[1] == ':')
		{
			gchar drive_colon_dot[4];

			drive_colon_dot[0] = file_name[0];
			drive_colon_dot[1] = ':';
			drive_colon_dot[2] = '.';
			drive_colon_dot[3] = '\0';

			return g_strdup (drive_colon_dot);
		}
#endif
		return g_strdup (".");
	}

	while (base > file_name && G_IS_DIR_SEPARATOR (*base))
		base--;


#if 1

	/* base points to the char before the last slash.
	*
	* In case file_name is the root of a drive (X:\) or a child of the
	* root of a drive (X:\foo), include the slash.
	*
	* In case file_name is the root share of an UNC path
	* (\\server\share), add a slash, returning \\server\share\ .
	*
	* In case file_name is a direct child of a share in an UNC path
	* (\\server\share\foo), include the slash after the share name,
	* returning \\server\share\ .
	*/
	if (base == file_name + 1 && g_ascii_isalpha (file_name[0]) && file_name[1] == ':')
		base++;
	else if (G_IS_DIR_SEPARATOR (file_name[0]) &&
		G_IS_DIR_SEPARATOR (file_name[1]) &&
		file_name[2] &&
		!G_IS_DIR_SEPARATOR (file_name[2]) &&
		base >= file_name + 2)
	{
		const gchar *p = file_name + 2;
		while (*p && !G_IS_DIR_SEPARATOR (*p))
			p++;
		if (p == base + 1)
		{
			len = (guint) strlen (file_name) + 1;
			base = g_new (gchar, len + 1);
			strcpy (base, file_name);
			base[len-1] = G_DIR_SEPARATOR;
			base[len] = 0;
			return base;
		}
		if (G_IS_DIR_SEPARATOR (*p))
		{
			p++;
			while (*p && !G_IS_DIR_SEPARATOR (*p))
				p++;
			if (p == base + 1)
				base++;
		}
	}
#endif

	len = (guint) 1 + base - file_name;

	base = g_new (gchar, len + 1);
	g_memmove (base, file_name, len);
	base[len] = 0;

	return base;
}





gchar*	g_strndup (const gchar *str,gsize n)    
{
	gchar *new_str;

	if (str)
	{
		new_str = g_new (gchar, n + 1);
		strncpy (new_str, str, n);
		new_str[n] = '\0';
	}
	else
		new_str = NULL;

	return new_str;
}

gchar*	g_strconcat (const gchar *string1, ...)
{
	gsize	  l;     
	va_list args;
	gchar	  *s;
	gchar	  *concat;
	gchar   *ptr;

	if (!string1)
		return NULL;

	l = 1 + strlen (string1);
	va_start (args, string1);
	s = va_arg (args, gchar*);
	while (s)
	{
		l += strlen (s);
		s = va_arg (args, gchar*);
	}
	va_end (args);

	concat = g_new (gchar, l);
	ptr = concat;

	ptr = g_stpcpy (ptr, string1);
	va_start (args, string1);
	s = va_arg (args, gchar*);
	while (s)
	{
		ptr = g_stpcpy (ptr, s);
		s = va_arg (args, gchar*);
	}
	va_end (args);

	return concat;
}

gboolean	g_path_is_absolute (const gchar *file_name)
{
	g_return_val_if_fail (file_name != NULL, FALSE);

	if (G_IS_DIR_SEPARATOR (file_name[0]))
		return TRUE;

#ifdef G_OS_WIN32
	/* Recognize drive letter on native Windows */
	if (g_ascii_isalpha (file_name[0]) && 
		file_name[1] == ':' && G_IS_DIR_SEPARATOR (file_name[2]))
		return TRUE;
#endif /* G_OS_WIN32 */

	return FALSE;
}


gchar *	g_stpcpy (gchar       *dest,  const gchar *src)
{
#ifdef HAVE_STPCPY
	g_return_val_if_fail (dest != NULL, NULL);
	g_return_val_if_fail (src != NULL, NULL);
	return stpcpy (dest, src);
#else
	register gchar *d = dest;
	register const gchar *s = src;

	g_return_val_if_fail (dest != NULL, NULL);
	g_return_val_if_fail (src != NULL, NULL);
	do{
		*d++ = *s;
	}
	while (*s++ != '\0');
	return d - 1;
#endif
}


gchar* g_strdup_vprintf (const gchar *format, va_list args)
{
  gchar *string = NULL;

  g_vasprintf (&string, format, args);

  return string;
}

#define MIN_ARRAY_SIZE  16

static void	g_ptr_array_maybe_expand (GRealPtrArray *array,
			  gint        len)
{
  if ((array->len + len) > array->alloc)
    {
      guint old_alloc = array->alloc;
      array->alloc = g_nearest_pow (array->len + len);
      array->alloc = MAX (array->alloc, MIN_ARRAY_SIZE);
      array->pdata = (gpointer*)g_realloc (array->pdata, sizeof (gpointer) * array->alloc);
      if (G_UNLIKELY (g_mem_gc_friendly))
        for ( ; old_alloc < array->alloc; old_alloc++)
          array->pdata [old_alloc] = NULL;
    }
}

void
g_ptr_array_set_size  (GPtrArray   *farray,
		       gint	     length)
{
  GRealPtrArray* array_ = (GRealPtrArray*) farray;

  g_return_if_fail (array_);

  if (length > array_->len)
    {
      int i;
      g_ptr_array_maybe_expand (array_, (length - array_->len));
      /* This is not 
       *     memset (array->pdata + array->len, 0,
       *            sizeof (gpointer) * (length - array->len));
       * to make it really portable. Remember (void*)NULL needn't be
       * bitwise zero. It of course is silly not to use memset (..,0,..).
       */
      for (i = array_->len; i < length; i++)
	array_->pdata[i] = NULL;
    }
  if (G_UNLIKELY (g_mem_gc_friendly) && length < array_->len)
    {
      int i;
      for (i = length; i < array_->len; i++)
	array_->pdata[i] = NULL;
    }

  array_->len = length;
}


//char * g_markup_vprintf_escaped (const char *format, va_list     args)
//{
//  GString *format1;
//  GString *format2;
//  GString *result = NULL;
//  gchar *output1 = NULL;
//  gchar *output2 = NULL;
//  const char *p, *op1, *op2;
//  va_list args2;
//
//  /* The technique here, is that we make two format strings that
//   * have the identical conversions in the identical order to the
//   * original strings, but differ in the text in-between. We
//   * then use the normal g_strdup_vprintf() to format the arguments
//   * with the two new format strings. By comparing the results,
//   * we can figure out what segments of the output come from
//   * the the original format string, and what from the arguments,
//   * and thus know what portions of the string to escape.
//   *
//   * For instance, for:
//   *
//   *  g_markup_printf_escaped ("%s ate %d apples", "Susan & Fred", 5);
//   *
//   * We form the two format strings "%sX%dX" and %sY%sY". The results
//   * of formatting with those two strings are
//   *
//   * "%sX%dX" => "Susan & FredX5X"
//   * "%sY%dY" => "Susan & FredY5Y"
//   *
//   * To find the span of the first argument, we find the first position
//   * where the two arguments differ, which tells us that the first
//   * argument formatted to "Susan & Fred". We then escape that
//   * to "Susan &amp; Fred" and join up with the intermediate portions
//   * of the format string and the second argument to get
//   * "Susan &amp; Fred ate 5 apples".
//   */
//
//  /* Create the two modified format strings
//   */
//  format1 = g_string_new (NULL);
//  format2 = g_string_new (NULL);
//  p = format;
//  while (TRUE)
//    {
//      const char *after;
//      const char *conv = find_conversion (p, &after);
//      if (!conv)
//	break;
//
//      g_string_append_len (format1, conv, after - conv);
//      g_string_append_c (format1, 'X');
//      g_string_append_len (format2, conv, after - conv);
//      g_string_append_c (format2, 'Y');
//
//      p = after;
//    }
//
//  /* Use them to format the arguments
//   */
//  G_VA_COPY (args2, args);
//  
//  output1 = g_strdup_vprintf (format1->str, args);
//  if (!output1)
//    {
//      va_end (args2);
//      goto cleanup;
//    }
//  
//  output2 = g_strdup_vprintf (format2->str, args2);
//  va_end (args2);
//  if (!output2)
//    goto cleanup;
//
//  result = g_string_new (NULL);
//
//  /* Iterate through the original format string again,
//   * copying the non-conversion portions and the escaped
//   * converted arguments to the output string.
//   */
//  op1 = output1;
//  op2 = output2;
//  p = format;
//  while (TRUE)
//    {
//      const char *after;
//      const char *output_start;
//      const char *conv = find_conversion (p, &after);
//      char *escaped;
//      
//      if (!conv)	/* The end, after points to the trailing \0 */
//	{
//	  g_string_append_len (result, p, after - p);
//	  break;
//	}
//
//      g_string_append_len (result, p, conv - p);
//      output_start = op1;
//      while (*op1 == *op2)
//	{
//	  op1++;
//	  op2++;
//	}
//      
//      escaped = g_markup_escape_text (output_start, op1 - output_start);
//      g_string_append (result, escaped);
//      g_free (escaped);
//      
//      p = after;
//      op1++;
//      op2++;
//    }
//
// cleanup:
//  g_string_free (format1, TRUE);
//  g_string_free (format2, TRUE);
//  g_free (output1);
//  g_free (output2);
//
//  if (result)
//    return g_string_free (result, FALSE);
//  else
//    return NULL;
//}

//int _g_gnulib_vasprintf (char **result, char const *format, va_list args)
//{
//  size_t length;
//
//  *result = vasnprintf (NULL, &length, format, args);
//  if (*result == NULL) 
//    return -1;
//  
//  return length;  
//}


//gint	g_vasprintf (gchar      **string, gchar const *format,	va_list      args)
//{
//	gint len;
//	g_return_val_if_fail (string != NULL, -1);
//
//#if !defined(HAVE_GOOD_PRINTF)
//
//	len = _g_gnulib_vasprintf (string, format, args);
//	if (len < 0)
//		*string = NULL;
//
//#elif defined (HAVE_VASPRINTF)
//
//	len = vasprintf (string, format, args);
//	if (len < 0)
//		*string = NULL;
//	else if (!g_mem_is_system_malloc ()) 
//	{
//		/* vasprintf returns malloc-allocated memory */
//		gchar *string1 = g_strndup (*string, len);
//		free (*string);
//		*string = string1;
//	}
//
//#else
//
//	{
//		va_list args2;
//
//		G_VA_COPY (args2, args);
//
//		*string = g_new (gchar, g_printf_string_upper_bound (format, args));
//
//		len = _g_vsprintf (*string, format, args2);
//		va_end (args2);
//	}
//#endif
//
//	return len;
//}
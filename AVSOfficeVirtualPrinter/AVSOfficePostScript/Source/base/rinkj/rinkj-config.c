/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/*$Id: rinkj-config.c 8022 2007-06-05 22:23:38Z giles $ */
/* Support for reading Rinkj config files. */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rinkj-config.h"

/* A rinkj config string is a series of \n terminated lines, each with
   the format "Key: Value". */

/**
 * rinkj_strdup_size: Duplicate a string with given size.
 * @src: Source string to duplicate.
 * @size: Size of @src.
 *
 * Return value: newly allocated string.
 **/
char *
rinkj_strdup_size (const char *src, int size)
{
  char *result;

  result = malloc (size + 1);
  memcpy (result, src, size);
  result[size] = 0;
  return result;
}

/**
 * rinkj_config_get: Get a config value.
 * @config: A config string.
 * @key: A key.
 *
 * Looks up @key in @config. Returns a newly allocated string, if found.
 *
 * Return value: value of the key, or NULL if not found.
 **/
char *
rinkj_config_get (const char *config, const char *key)
{
  int ix;
  int ix_eol, ix_next;

  if (config == NULL)
    return NULL;

  for (ix = 0; config[ix]; ix = ix_next)
    {
      char *p_nl;
      int key_ix;

      p_nl = strchr (config + ix, '\n');
      if (p_nl == NULL)
	{
	  /* last line not \n terminated */
	  ix_eol = strlen (config + ix);
	  ix_next = ix_eol;
	}
      else
	{
	  ix_eol = ix + p_nl - config;
	  ix_next = ix_eol + 1;
	}

      for (key_ix = 0; ix + key_ix < ix_eol; key_ix++)
	{
	  if (key[key_ix] == 0 && config[ix + key_ix] == ':')
	    {
	      ix += key_ix + 1;
	      while (ix < ix_eol && isspace (config[ix]))
		ix++;
	      return rinkj_strdup_size (config + ix, ix_eol - ix);
	    }
	  else if (key[key_ix] != config[ix + key_ix])
	    break;
	}
    }
  return NULL;
}

/**
 * rinkj_config_keyval: Get key, value pair from config string.
 * @config: A config string.
 * @p_val: Where to store newly allocated val.
 * @p_next: Where to store pointer to next config line,
 *
 * Gets a key, value pair from the config string.
 *
 * Return value: Newly allocated key, or NULL if empty config.
 **/
char *
rinkj_config_keyval (const char *config, char **p_val, const char **p_next)
{
  char *key;
  int ix;
  int ix_eol, ix_next;

  if (config == NULL)
    return NULL;

  for (ix = 0; config[ix]; ix = ix_next)
    {
      char *p_nl;
      int key_ix;

      p_nl = strchr (config + ix, '\n');
      if (p_nl == NULL)
	{
	  /* last line not \n terminated */
	  ix_eol = strlen (config + ix);
	  ix_next = ix_eol;
	}
      else
	{
	  ix_eol = ix + p_nl - config;
	  ix_next = ix_eol + 1;
	}

      for (key_ix = 0; ix + key_ix < ix_eol; key_ix++)
	{
	  if (config[ix + key_ix] == ':')
	    {
	      key = rinkj_strdup_size (config + ix, key_ix);
	      ix += key_ix + 1;
	      while (ix < ix_eol && isspace (config[ix]))
		ix++;
	      if (p_val != NULL)
		*p_val = rinkj_strdup_size (config + ix, ix_eol - ix);
	      if (p_next != NULL)
		*p_next = config + ix_next;
	      return key;
	    }
	}
    }
  return NULL;
}

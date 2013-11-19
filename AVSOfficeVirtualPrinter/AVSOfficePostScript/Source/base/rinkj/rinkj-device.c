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

/*$Id: rinkj-device.c 8022 2007-06-05 22:23:38Z giles $ */
/* The device abstraction within the Rinkj driver. */

#include "rinkj-device.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Deprecated */
int
rinkj_device_set (RinkjDevice *self, const char *config)
{
  if (self->init_happened != 0)
    return -1;
  return self->set (self, config);
}

/* Preferred, as it matches IJS */
int
rinkj_device_set_param (RinkjDevice *self, const char *key,
			const char *value, int value_size)
{
  int keylen = strlen (key);
  int bufsize = keylen + value_size + 3;
  char *buf = malloc (bufsize);
  int status;

  /* This implementation is in terms of device_set, but we're going to
     change the prototype of the device so this is native. */
  memcpy (buf, key, keylen);
  memcpy (buf + keylen, ": ", 2);
  memcpy (buf + keylen + 2, value, value_size);
  buf[keylen + 2 + value_size] = 0;
  status = rinkj_device_set (self, buf);
  free (buf);
  return status;
}

int
rinkj_device_set_param_string (RinkjDevice *self, const char *key,
			       const char *value)
{
  return rinkj_device_set_param (self, key, value, strlen (value));
}

int
rinkj_device_set_param_int (RinkjDevice *self, const char *key, int value)
{
  char buf[32];
  int value_size = sprintf (buf, "%d", value);
  return rinkj_device_set_param (self, key, buf, value_size);
}

int
rinkj_device_init (RinkjDevice *self, const RinkjDeviceParams *params)
{
  int status;

  if (self->init_happened != 0)
    return -1;
  status = self->init (self, params);
  self->init_happened = 42;
  return status;
}

int
rinkj_device_write (RinkjDevice *self, const char **data)
{
  if (self->init_happened != 42)
    return -1;
  return self->write (self, data);
}

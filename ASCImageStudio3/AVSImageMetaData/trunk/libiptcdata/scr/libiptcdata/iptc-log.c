/* iptc-log.c
 *
 * Copyright © 2005 David Moore <dcm@acm.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <config.h>
#include <libiptcdata/iptc-log.h>
#include "i18n.h"

#include <string.h>

struct _IptcLog {
	unsigned int ref_count;

	IptcLogFunc func;
	void *data;

	IptcMem *mem;
};

static struct {
	IptcLogCode code;
	const char *title;
	const char *message;
} codes[] = {
	{ IPTC_LOG_CODE_DEBUG, N_("Debugging information"),
	  N_("Debugging information is available.") },
	{ IPTC_LOG_CODE_NO_MEMORY, N_("Not enough memory"),
	  N_("The system cannot provide enough memory.") },
	{ IPTC_LOG_CODE_CORRUPT_DATA, N_("Corrupt data"),
	  N_("The data provided does not follow the specification.") },
	{ 0, NULL, NULL }
};

const char *
iptc_log_code_get_title (IptcLogCode code)
{
	unsigned int i;

	for (i = 0; codes[i].title; i++) if (codes[i].code == code) break;
	return _(codes[i].title);
}

const char *
iptc_log_code_get_message (IptcLogCode code)
{
	unsigned int i;

	for (i = 0; codes[i].message; i++) if (codes[i].code == code) break;
	return _(codes[i].message);
}

IptcLog *
iptc_log_new_mem (IptcMem *mem)
{
	IptcLog *log;

	log = iptc_mem_alloc (mem, sizeof (IptcLog));
	if (!log) return NULL;
	log->ref_count = 1;

	log->mem = mem;
	iptc_mem_ref (mem);

	return log;
}

IptcLog *
iptc_log_new (void)
{
	IptcMem *mem = iptc_mem_new_default ();
	IptcLog *log = iptc_log_new_mem (mem);

	iptc_mem_unref (mem);

	return log;
}

void
iptc_log_ref (IptcLog *log)
{
	if (!log) return;
	log->ref_count++;
}

void
iptc_log_unref (IptcLog *log)
{
	if (!log) return;
	if (log->ref_count > 0) log->ref_count--;
	if (!log->ref_count) iptc_log_free (log);
}

void
iptc_log_free (IptcLog *log)
{
	if (!log) return;

	iptc_mem_free (log->mem, log);
}

void
iptc_log_set_func (IptcLog *log, IptcLogFunc func, void *data)
{
	if (!log) return;
	log->func = func;
	log->data = data;
}

void
iptc_log (IptcLog *log, IptcLogCode code, const char *domain,
	  const char *format, ...)
{
	va_list args;

	va_start (args, format);
	iptc_logv (log, code, domain, format, args);
	va_end (args);
}

void
iptc_logv (IptcLog *log, IptcLogCode code, const char *domain,
	   const char *format, va_list args)
{
	if (!log) return;
	if (!log->func) return;
	log->func (log, code, domain, format, args, log->data);
}

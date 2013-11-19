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

/* $Id: gdevpdfe.c 9368 2009-01-16 18:38:48Z ken $ */
/* Metadata writer. */
#include "gx.h"
#include "gserrors.h"
#include "string_.h"
#include "time_.h"
#include "stream.h"
#include "gp.h"
#include "smd5.h"
#include "gscdefs.h"
#include "gdevpdfx.h"
#include "gdevpdfg.h"
#include "gdevpdfo.h"
#include "gdevpdtf.h"
#include "ConvertUTF.h"


static void 
copy_bytes(stream *s, const byte **data, int *data_length, int n)
{
    while (n-- && (*data_length)--) {
	stream_putc(s, *((*data)++));
    }
}

/* Write XML data */
static void
pdf_xml_data_write(stream *s, const byte *data, int data_length)
{
    int l = data_length;
    const byte *p = data;

    while (l > 0) {
	switch (*p) {
	    case '<' : stream_puts(s, "&lt;"); l--; p++; break;
	    case '>' : stream_puts(s, "&gt;"); l--; p++; break;
	    case '&' : stream_puts(s, "&amp;"); l--; p++; break;
	    case '\'': stream_puts(s, "&apos;"); l--; p++; break;
	    case '"' : stream_puts(s, "&quot;"); l--; p++; break;
	    default:
		if (*p < 32) {
		    /* Not allowed in XML. */
		    pprintd1(s, "&#%d;", *p);
		    l--; p++;
		} else if (*p >= 0x7F && *p <= 0x9f) {
		    /* Control characters are discouraged in XML. */
		    pprintd1(s, "&#%d;", *p);
		    l--; p++;
		} else if ((*p & 0xE0) == 0xC0) {
		    /* A 2-byte UTF-8 sequence */
		    copy_bytes(s, &p, &l, 2);
		} else if ((*p & 0xF0) == 0xE0) {
		    /* A 3-byte UTF-8 sequence */
		    copy_bytes(s, &p, &l, 3);
		} else if ((*p & 0xF0) == 0xF0) {
		    /* A 4-byte UTF-8 sequence */
		    copy_bytes(s, &p, &l, 4);
		} else {
		    stream_putc(s, *p);
		    l--; p++;
		}
	}
    }
}

/* Write XML string */
static inline void
pdf_xml_string_write(stream *s, const char *data)
{
    pdf_xml_data_write(s, (const byte *)data, strlen(data));
}

/* Begin an opening XML tag */
static inline void
pdf_xml_tag_open_beg(stream *s, const char *data)
{
    stream_putc(s, '<');
    stream_puts(s, data);
}

/* End an XML tag */
static inline void
pdf_xml_tag_end(stream *s)
{
    stream_putc(s, '>');
}

/* End an empty XML tag */
static inline void
pdf_xml_tag_end_empty(stream *s)
{
    stream_puts(s, "/>");
}

/* Write an opening XML tag */
static inline void
pdf_xml_tag_open(stream *s, const char *data)
{
    stream_putc(s, '<');
    stream_puts(s, data);
    stream_putc(s, '>');
}

/* Write a closing XML tag */
static inline void
pdf_xml_tag_close(stream *s, const char *data)
{
    stream_puts(s, "</");
    stream_puts(s, data);
    stream_putc(s, '>');
}

/* Write an attribute name */
static inline void
pdf_xml_attribute_name(stream *s, const char *data)
{
    stream_putc(s, ' ');
    stream_puts(s, data);
    stream_putc(s, '=');
}

/* Write a attribute value */
static inline void
pdf_xml_attribute_value(stream *s, const char *data)
{
    stream_putc(s, '\'');
    pdf_xml_string_write(s, data);
    stream_putc(s, '\'');
}
/* Write a attribute value */
static inline void
pdf_xml_attribute_value_data(stream *s, const byte *data, int data_length)
{
    stream_putc(s, '\'');
    pdf_xml_data_write(s, data, data_length);
    stream_putc(s, '\'');
}

/* Begin an  XML instruction */
static inline void
pdf_xml_ins_beg(stream *s, const char *data)
{
    stream_puts(s, "<?");
    stream_puts(s, data);
}

/* End an XML instruction */
static inline void
pdf_xml_ins_end(stream *s)
{
    stream_puts(s, "?>");
}

/* Write a newline character */
static inline void
pdf_xml_newline(stream *s)
{
    stream_puts(s, "\n");
}

/* Copy to XML output */
static inline void
pdf_xml_copy(stream *s, const char *data)
{
    stream_puts(s, data);
}


/* --------------------------------------------  */

static int
pdf_xmp_time(char *buf, int buf_length)
{
    /* We don't write a day time because we don't have a time zone. */
    struct tm tms;
    time_t t;
    char buf1[4+1+2+1+2+1]; /* yyyy-mm-dd\0 */

    time(&t);
    tms = *localtime(&t);
    sprintf(buf1,
	    "%04d-%02d-%02d",
	    tms.tm_year + 1900, tms.tm_mon + 1, tms.tm_mday);
    strncpy(buf, buf1, buf_length);
    return strlen(buf);
}

static int
pdf_xmp_convert_time(char *dt, int dtl, char *buf, int bufl)
{   /* The 'dt' buffer is of same size as 'buf'. */
    /* Input  sample : D:199812231952?08'00' */
    /* Output sample : 1997-07-16T19:20:30+01:00 */
    int l = dtl;

    if (l > bufl)
	l = bufl;
    if (dt[0] == 'D' && dt[1] == ':') {
	l -= 2;
	memcpy(buf, dt + 2, l);
    } else
	memcpy(buf, dt, l);
    memcpy(dt, buf, 4); /* year */
    if (l <= 4)
	return 4;

    dt[4] = '-';
    memcpy(dt + 5, buf + 4, 2); /* month */
    if (l <= 6)
	return 7;

    dt[7] = '-';
    memcpy(dt + 8, buf + 6, 2); /* day */
    if (l <= 8)
	return 10;

    dt[10] = 'T';
    memcpy(dt + 11, buf + 8, 2); /* hour */
    dt[13] = ':';
    memcpy(dt + 14, buf + 10, 2); /* minute */
    if (l <= 12) {
	dt[16] = 'Z'; /* Default time zone 0. */
	return 17;
    }

    dt[16] = ':';
    memcpy(dt + 17, buf + 12, 2); /* second */
    if (l <= 14) {
	dt[18] = 'Z'; /* Default time zone 0. */
	return 19;
    }

    dt[19] = buf[14]; /* designator */
    if (dt[19] == 'Z')
	return 20;
    if (l <= 15)
	return 20;
    memcpy(dt + 20, buf + 15, 2); /* Time zone hour difference. */
    if (l <= 17)
	return 22;

    dt[22] = ':';
    /* Skipping '\'' in 'buf'. */
    memcpy(dt + 23, buf + 18, 2); /* Time zone hour difference. */
    return 25;
}
	
static int
pdf_get_docinfo_item(gx_device_pdf *pdev, const char *key, char *buf, int buf_length)
{
    const cos_value_t *v = cos_dict_find(pdev->Info, (const byte *)key, strlen(key));
    int l;
    const byte *s;

    if (v != NULL && (v->value_type == COS_VALUE_SCALAR || 
			v->value_type == COS_VALUE_CONST)) {
	if (v->contents.chars.size > 2 && v->contents.chars.data[0] == '(') {
	    s = v->contents.chars.data + 1;
	    l = v->contents.chars.size - 2;
	} else {
	    s = v->contents.chars.data;
	    l = v->contents.chars.size;
	}
    } else
	return 0;
    if (l < 0)
	l = 0;
    if (l > buf_length)
	l = buf_length;
    memcpy(buf, s, l);
    return l;
}

static inline byte
decode_escape(const byte *data, int data_length, int *index)
{
    byte c;

    (*index)++; /* skip '\' */
    if (*index >= data_length)
	return 0; /* Must_not_happen, because the string is PS encoded. */
    c = data[*index];
    switch (c) {
	case 'n': (*index)++; return '\n';
	case 'r': (*index)++; return '\r';
	case 't': (*index)++; return '\t';
	default:
	    break;
    }
    if (c >= '0' && c <= '7') {
	/* octal */
	byte v = c - '0';

	for (;;) {
	    (*index)++;
	    if (*index >= data_length)
		return v;
	    c = data[*index];
	    if (c < '0' || c > '7')
		break;
	    v = v * 8 + (c - '0');
	}
	return v;	
    }
    return c; /* A wrong escapement sequence. */
}

static int
pdf_xmp_write_translated(gx_device_pdf *pdev, stream *s, const byte *data, int data_length,
			 void(*write)(stream *s, const byte *data, int data_length))
{
    if (pdev->DSCEncodingToUnicode.data == 0) {
	write(s, data, data_length);
	return 0;
    } else {
	UTF16 *buf0;
	const UTF16 *buf0b;
	UTF8 *buf1, *buf1b;
	int i, j = 0;

	buf0 = (UTF16 *)gs_alloc_bytes(pdev->memory, data_length * sizeof(UTF16), 
			"pdf_xmp_write_translated");
	if (buf0 == NULL)
	    return_error(gs_error_VMerror);
	buf1 = (UTF8 *)gs_alloc_bytes(pdev->memory, data_length * 2, 
			"pdf_xmp_write_translated");
	if (buf1 == NULL)
	    return_error(gs_error_VMerror);
	buf0b = buf0;
	buf1b = buf1;
	for (i = 0; i < data_length; i++) {
	    byte c = data[i];
	    int v;

	    if (c == '\\') 
		c = decode_escape(data, data_length, &i);
	    if (c > pdev->DSCEncodingToUnicode.size)
		return_error(gs_error_rangecheck);

	    v = pdev->DSCEncodingToUnicode.data[c];
	    if (v == -1)
		v = '?'; /* Arbitrary. */
	    buf0[j] = v;
	    j++;
	}
	switch (ConvertUTF16toUTF8(&buf0b, buf0 + j,
			     &buf1b, buf1 + data_length * 2, strictConversion)) {
	    case conversionOK:
		write(s, buf1, buf1b - buf1);
		break;
	    case sourceExhausted:
	    case targetExhausted:
	    case sourceIllegal:
	    default:
		return_error(gs_error_rangecheck);
	}
	gs_free_object(pdev->memory, buf0, "pdf_xmp_write_translated");
	gs_free_object(pdev->memory, buf1, "pdf_xmp_write_translated");
	return 0;
    }
}

static int
pdf_xmp_write_docinfo_item(gx_device_pdf *pdev, stream *s, const char *key, const char *default_value,
			   void(*write)(stream *s, const byte *data, int data_length))
{
    const cos_value_t *v = cos_dict_find(pdev->Info, (const byte *)key, strlen(key));

    if (v != NULL && (v->value_type == COS_VALUE_SCALAR || 
			v->value_type == COS_VALUE_CONST)) {
	if (v->contents.chars.size > 2 && v->contents.chars.data[0] == '(')
	    return pdf_xmp_write_translated(pdev, s, v->contents.chars.data + 1, 
			v->contents.chars.size - 2, write);
	else
	    return pdf_xmp_write_translated(pdev, s, v->contents.chars.data, 
			v->contents.chars.size, write);
    } else {
	stream_puts(s, default_value);
	return 0;
    }
}

static uint64_t
pdf_uuid_time(gx_device_pdf *pdev)
{   
    long *dt = pdev->uuid_time; /* In seconds since Jan. 1, 1980 and fraction in nanoseconds. */
    uint64_t t;

    /* UUIDs use time in 100ns ticks since Oct 15, 1582. */
    t = (uint64_t)10000000 * dt[0] + dt[0] / 100; /* since Jan. 1, 1980 */
    t += (uint64_t) (1000*1000*10)         /* seconds */
       * (uint64_t) (60 * 60 * 24)         /* days */
       * (uint64_t) (17+30+31+365*397+99); /* # of days */
    return t;
}

static void writehex(char **p, ulong v, int l)
{
    int i = l * 2;
    static const char digit[] = "0123456789abcdef";

    for (; i--;)
	*((*p)++) = digit[v >> (i * 4) & 15];
}

static void
pdf_make_uuid(const byte node[6], uint64_t uuid_time, ulong time_seq, char *buf, int buf_length)
{   
    char b[40], *p = b;
    ulong  uuid_time_lo = (ulong)(uuid_time & 0xFFFFFFFF);       /* MSVC 7.1.3088           */
    ushort uuid_time_md = (ushort)((uuid_time >> 32) & 0xFFFF);  /* cannot compile this     */
    ushort uuid_time_hi = (ushort)((uuid_time >> 48) & 0x0FFF);  /* as function arguments.  */

    writehex(&p, uuid_time_lo, 4); /* time_low */
    *(p++) = '-';
    writehex(&p, uuid_time_md, 2); /* time_mid */
    *(p++) = '-';
    writehex(&p, uuid_time_hi | (ushort)(1 << 12), 2); /* time_hi_and_version */
    *(p++) = '-';
    writehex(&p, (time_seq & 0x3F00) >> 8, 1); /* clock_seq_hi_and_reserved */
    writehex(&p, time_seq & 0xFF, 1); /* clock_seq & 0xFF */
    *(p++) = '-';
    writehex(&p, node[0], 1);
    writehex(&p, node[1], 1);
    writehex(&p, node[2], 1);
    writehex(&p, node[3], 1);
    writehex(&p, node[4], 1);
    writehex(&p, node[5], 1);
    *p = 0;
    strncpy(buf, b, buf_length);
}

static int
pdf_make_instance_uuid(gx_device_pdf *pdev, const byte digest[6], char *buf, int buf_length)
{
    if (pdev->InstanceUUID.size) {
	int l = min(buf_length - 1, pdev->InstanceUUID.size);

	memcpy(buf, pdev->InstanceUUID.data, l);
	buf[l] = 0;
    } else
	pdf_make_uuid(digest, pdf_uuid_time(pdev), pdev->DocumentTimeSeq, buf, buf_length);
    return 0;
}

static int
pdf_make_document_uuid(gx_device_pdf *pdev, const byte digest[6], char *buf, int buf_length)
{
    if (pdev->DocumentUUID.size) {
	int l = min(buf_length - 1, pdev->DocumentUUID.size);

	memcpy(buf, pdev->DocumentUUID.data, l);
	buf[l] = 0;
    } else
	pdf_make_uuid(digest, pdf_uuid_time(pdev), pdev->DocumentTimeSeq, buf, buf_length);
    return 0;
}

static const char dd[]={'\'', '\357', '\273', '\277', '\'', 0};

/* --------------------------------------------  */

/* Write Document metadata */
static int
pdf_write_document_metadata(gx_device_pdf *pdev, const byte digest[6])
{
    char instance_uuid[40], document_uuid[40], cre_date_time[40], mod_date_time[40], date_time_buf[40];
    int cre_date_time_len, mod_date_time_len;
    int code;
    stream *s = pdev->strm;

    code = pdf_make_instance_uuid(pdev, digest, instance_uuid, sizeof(instance_uuid));
    if (code < 0)
	return code;
    code = pdf_make_document_uuid(pdev, digest, document_uuid, sizeof(document_uuid));
    if (code < 0)
	return code;

    /* PDF/A XMP reference recommends setting UUID to empty. If not empty must be a URI */
    if (pdev->PDFA) 
	instance_uuid[0] = 0x00;

    cre_date_time_len = pdf_get_docinfo_item(pdev, "/CreationDate", cre_date_time, sizeof(cre_date_time));
    if (!cre_date_time_len)
	cre_date_time_len = pdf_xmp_time(cre_date_time, sizeof(cre_date_time));
    else
	cre_date_time_len = pdf_xmp_convert_time(cre_date_time, cre_date_time_len, date_time_buf, sizeof(date_time_buf));
    mod_date_time_len = pdf_get_docinfo_item(pdev, "/CreationDate", mod_date_time, sizeof(mod_date_time));
    if (!mod_date_time_len)
	mod_date_time_len = pdf_xmp_time(mod_date_time, sizeof(mod_date_time));
    else
	mod_date_time_len = pdf_xmp_convert_time(mod_date_time, mod_date_time_len, date_time_buf, sizeof(date_time_buf));

    pdf_xml_ins_beg(s, "xpacket");
    pdf_xml_attribute_name(s, "begin");
    pdf_xml_copy(s, dd);
    pdf_xml_attribute_name(s, "id");
    pdf_xml_attribute_value(s, "W5M0MpCehiHzreSzNTczkc9d");
    pdf_xml_ins_end(s);
    pdf_xml_newline(s);

    pdf_xml_copy(s, "<?adobe-xap-filters esc=\"CRLF\"?>\n");
    pdf_xml_copy(s, "<x:xmpmeta xmlns:x='adobe:ns:meta/'"
	                      " x:xmptk='XMP toolkit 2.9.1-13, framework 1.6'>\n");
    {
	pdf_xml_copy(s, "<rdf:RDF xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#' "
	                         "xmlns:iX='http://ns.adobe.com/iX/1.0/'>\n");
	{

    	    pdf_xml_tag_open_beg(s, "rdf:Description");
	    pdf_xml_attribute_name(s, "rdf:about");
	    pdf_xml_attribute_value(s, instance_uuid);
	    pdf_xml_attribute_name(s, "xmlns:pdf");
	    pdf_xml_attribute_value(s, "http://ns.adobe.com/pdf/1.3/");

	    if (cos_dict_find(pdev->Info, (const byte *)"/Keywords", 9)) {
		pdf_xml_tag_end(s);
		pdf_xml_tag_open_beg(s, "pdf:Producer");
		pdf_xml_tag_end(s);
		code = pdf_xmp_write_docinfo_item(pdev, s,  "/Producer", "UnknownProducer",
			pdf_xml_data_write);
		if (code < 0)
		    return code;
		pdf_xml_tag_close(s, "pdf:Producer");
		pdf_xml_newline(s);

		pdf_xml_tag_open_beg(s, "pdf:Keywords");
		pdf_xml_tag_end(s);
		code = pdf_xmp_write_docinfo_item(pdev, s,  "/Keywords", "Unknown", 
			pdf_xml_data_write);
		if (code < 0)
		    return code;
		pdf_xml_tag_close(s, "pdf:Keywords");
		pdf_xml_newline(s);

		pdf_xml_tag_close(s, "rdf:Description");
		pdf_xml_newline(s);
	    } else {
		pdf_xml_attribute_name(s, "pdf:Producer");
		code = pdf_xmp_write_docinfo_item(pdev, s,  "/Producer", "UnknownProducer",
			pdf_xml_attribute_value_data);
		if (code < 0)
		    return code;
		pdf_xml_tag_end_empty(s);
		pdf_xml_newline(s);
	    }

	    pdf_xml_tag_open_beg(s, "rdf:Description");
	    pdf_xml_attribute_name(s, "rdf:about");
	    pdf_xml_attribute_value(s, instance_uuid);
	    pdf_xml_attribute_name(s, "xmlns:xmp");
	    pdf_xml_attribute_value(s, "http://ns.adobe.com/xap/1.0/");
	    pdf_xml_tag_end(s);
	    {
		pdf_xml_tag_open_beg(s, "xmp:ModifyDate");
		pdf_xml_tag_end(s);
		mod_date_time[mod_date_time_len] = 0x00;
		pdf_xml_copy(s, mod_date_time);
		pdf_xml_tag_close(s, "xmp:ModifyDate");
		pdf_xml_newline(s);
	    }
	    {
		pdf_xml_tag_open_beg(s, "xmp:CreateDate");
		pdf_xml_tag_end(s);
		cre_date_time[cre_date_time_len] = 0x00;
		pdf_xml_copy(s, cre_date_time);
		pdf_xml_tag_close(s, "xmp:CreateDate");
		pdf_xml_newline(s);
	    }
	    {
		pdf_xml_tag_open_beg(s, "xmp:CreatorTool");
		pdf_xml_tag_end(s);
		code = pdf_xmp_write_docinfo_item(pdev, s,  "/Creator", "UnknownApplication",
			pdf_xml_data_write);
		if (code < 0)
		    return code;
		pdf_xml_tag_close(s, "xmp:CreatorTool");
	    }
	    pdf_xml_tag_close(s, "rdf:Description");
	    pdf_xml_newline(s);

	    pdf_xml_tag_open_beg(s, "rdf:Description");
	    pdf_xml_attribute_name(s, "rdf:about");
	    pdf_xml_attribute_value(s, instance_uuid);
	    pdf_xml_attribute_name(s, "xmlns:xapMM");
	    pdf_xml_attribute_value(s, "http://ns.adobe.com/xap/1.0/mm/");
	    pdf_xml_attribute_name(s, "xapMM:DocumentID");
	    pdf_xml_attribute_value(s, document_uuid);
	    pdf_xml_tag_end_empty(s);
	    pdf_xml_newline(s);

	    pdf_xml_tag_open_beg(s, "rdf:Description");
	    pdf_xml_attribute_name(s, "rdf:about");
	    pdf_xml_attribute_value(s, instance_uuid);
	    pdf_xml_attribute_name(s, "xmlns:dc");
	    pdf_xml_attribute_value(s, "http://purl.org/dc/elements/1.1/");
	    pdf_xml_attribute_name(s, "dc:format");
	    pdf_xml_attribute_value(s,"application/pdf");
	    pdf_xml_tag_end(s);
	    {
		pdf_xml_tag_open(s, "dc:title");
		{
		    pdf_xml_tag_open(s, "rdf:Alt");
		    {
			pdf_xml_tag_open_beg(s, "rdf:li");
			pdf_xml_attribute_name(s, "xml:lang");
			pdf_xml_attribute_value(s, "x-default");
			pdf_xml_tag_end(s);
			{
			   code = pdf_xmp_write_docinfo_item(pdev, s,  "/Title", "Untitled", 
				    pdf_xml_data_write);
			    if (code < 0)
				return code;
			}
			pdf_xml_tag_close(s, "rdf:li");
		    }
		    pdf_xml_tag_close(s, "rdf:Alt");
		}
		pdf_xml_tag_close(s, "dc:title");

		if (cos_dict_find(pdev->Info, (const byte *)"/Author", 7)) {
		    pdf_xml_tag_open(s, "dc:creator");
		    {   /* According to the PDF/A specification
			   "it shall be represented by an ordered Text array of
			   length one whose single entry shall consist 
			   of one or more names". */
			pdf_xml_tag_open(s, "rdf:Seq");
			{
			    pdf_xml_tag_open(s, "rdf:li");
			    {
    				code = pdf_xmp_write_docinfo_item(pdev, s,  "/Author", "Unknown", 
					    pdf_xml_data_write);
				if (code < 0)
				    return code;
			    }
			    pdf_xml_tag_close(s, "rdf:li");
			}
			pdf_xml_tag_close(s, "rdf:Seq");
		    }
		    pdf_xml_tag_close(s, "dc:creator");
		}
		if (cos_dict_find(pdev->Info, (const byte *)"/Subject", 8)) {
		    pdf_xml_tag_open(s, "dc:description");
		    {   
			pdf_xml_tag_open(s, "rdf:Seq");
			{
			    pdf_xml_tag_open(s, "rdf:li");
			    {
    				code = pdf_xmp_write_docinfo_item(pdev, s,  "/Subject", "No Subject", 
					    pdf_xml_data_write);
				if (code < 0)
				    return code;
			    }
			    pdf_xml_tag_close(s, "rdf:li");
			}
			pdf_xml_tag_close(s, "rdf:Seq");
		    }
		    pdf_xml_tag_close(s, "dc:description");
		}
	    }
	    pdf_xml_tag_close(s, "rdf:Description");
	    pdf_xml_newline(s);
	    if (pdev->PDFA) {
		pdf_xml_tag_open_beg(s, "rdf:Description");
		pdf_xml_attribute_name(s, "rdf:about");
		pdf_xml_attribute_value(s, instance_uuid);
		pdf_xml_attribute_name(s, "xmlns:pdfaid");
		pdf_xml_attribute_value(s, "http://www.aiim.org/pdfa/ns/id/");
		pdf_xml_attribute_name(s, "pdfaid:part");
		pdf_xml_attribute_value(s,"1");
		pdf_xml_attribute_name(s, "pdfaid:conformance");
		pdf_xml_attribute_value(s,"B");
		pdf_xml_tag_end_empty(s);
	   }
	}
	pdf_xml_copy(s, "</rdf:RDF>\n");
    }
    pdf_xml_copy(s, "</x:xmpmeta>\n");

    pdf_xml_copy(s, "                                                                        \n");
    pdf_xml_copy(s, "                                                                        \n");
    pdf_xml_copy(s, "<?xpacket end='w'?>");
    return 0;
}

int
pdf_document_metadata(gx_device_pdf *pdev)
{   
    if (pdev->CompatibilityLevel < 1.4)
	return 0;
    if (pdev->ParseDSCCommentsForDocInfo || pdev->PreserveEPSInfo) {
	pdf_resource_t *pres;
	char buf[20];
	byte digest[6];
	int code;
	int options = DATA_STREAM_NOT_BINARY;

	sflush(pdev->strm);
	s_MD5C_get_digest(pdev->strm, digest, sizeof(digest));
	if (pdev->EncryptMetadata)
	    options |= DATA_STREAM_ENCRYPT;
	code = pdf_open_aside(pdev, resourceOther, gs_no_id, &pres, true, options);
	if (code < 0)
	    return code;
	code = cos_dict_put_c_key_string((cos_dict_t *)pres->object, "/Type", (const byte *)"/Metadata", 9);
	if (code < 0)
	    return code;
	code = cos_dict_put_c_key_string((cos_dict_t *)pres->object, "/Subtype", (const byte *)"/XML", 4);
	if (code < 0)
	    return code;
	code = pdf_write_document_metadata(pdev, digest);
	if (code < 0)
	    return code;
	code = pdf_close_aside(pdev);
	if (code < 0)
	    return code;
	code = COS_WRITE_OBJECT(pres->object, pdev);
	if (code < 0)
	    return code;
	sprintf(buf, "%ld 0 R", pres->object->id);
	cos_dict_put_c_key_object(pdev->Catalog, "/Metadata", pres->object);
    }
    return 0;
}

/* --------------------------------------------  */

/* Write Font metadata */
static int
pdf_write_font_metadata(gx_device_pdf *pdev, const pdf_base_font_t *pbfont, 
			const byte *digest, int digest_length)
{
    char instance_uuid[40];
    int code;
    stream *s = pdev->strm;
    gs_font_info_t info;
    gs_font_base *pfont = pbfont->complete;

    if (pfont == NULL)
	pfont = pbfont->copied;
    /* Fixme: For True Type fonts need to get Coipyright, Owner from the TT data. */
    pdf_make_uuid(digest, pdf_uuid_time(pdev), pdev->DocumentTimeSeq, instance_uuid, sizeof(instance_uuid));
    code = pfont->procs.font_info((gs_font *)pfont, NULL,
		    (FONT_INFO_COPYRIGHT | FONT_INFO_NOTICE |
			FONT_INFO_FAMILY_NAME | FONT_INFO_FULL_NAME),
					&info);
    if (code < 0)
	return code;
    pdf_xml_ins_beg(s, "xpacket");
    pdf_xml_attribute_name(s, "begin");
    pdf_xml_copy(s, dd);
    pdf_xml_attribute_name(s, "id");
    pdf_xml_attribute_value(s, "W5M0MpCehiHzreSzNTczkc9d");
    pdf_xml_ins_end(s);
    pdf_xml_newline(s);

    pdf_xml_copy(s, "<?adobe-xap-filters esc=\"CRLF\"?>\n");
    pdf_xml_copy(s, "<x:xmpmeta xmlns:x='adobe:ns:meta/'"
	                      " x:xmptk='XMP toolkit 2.9.1-13, framework 1.6'>\n");
    {
	pdf_xml_copy(s, "<rdf:RDF xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#' "
	                         "xmlns:iX='http://ns.adobe.com/iX/1.0/'>\n");
	{

	    pdf_xml_tag_open_beg(s, "rdf:Description");
	    pdf_xml_attribute_name(s, "rdf:about");
	    pdf_xml_attribute_value(s, instance_uuid);
	    pdf_xml_attribute_name(s, "xmlns:xmp");
	    pdf_xml_attribute_value(s, "http://ns.adobe.com/xap/1.0/");
	    pdf_xml_tag_end(s);
	    {
		pdf_xml_tag_open_beg(s, "xmp:Title");
		pdf_xml_tag_end(s);
		{
		    pdf_xml_tag_open(s, "rdf:Alt");
		    {
			pdf_xml_tag_open_beg(s, "rdf:li");
			pdf_xml_attribute_name(s, "xml:lang");
			pdf_xml_attribute_value(s, "x-default");
			pdf_xml_tag_end(s);
			{
			   pdf_xml_data_write(s, pbfont->font_name.data, pbfont->font_name.size);
			}
			pdf_xml_tag_close(s, "rdf:li");
		    }
		    pdf_xml_tag_close(s, "rdf:Alt");
		}
		pdf_xml_tag_close(s, "xmp:Title");
	    }
	    pdf_xml_tag_close(s, "rdf:Description");
	    pdf_xml_newline(s);


	    pdf_xml_tag_open_beg(s, "rdf:Description");
	    pdf_xml_attribute_name(s, "rdf:about");
	    pdf_xml_attribute_value(s, instance_uuid);
	    pdf_xml_attribute_name(s, "xmlns:xmpRights");
	    pdf_xml_attribute_value(s, "http://ns.adobe.com/xap/1.0/rights/");
	    pdf_xml_tag_end(s);
	    if (info.members & FONT_INFO_COPYRIGHT) {
		pdf_xml_tag_open_beg(s, "xmpRights:Copyright");
		pdf_xml_tag_end(s);
		{
		    pdf_xml_tag_open(s, "rdf:Alt");
		    {
			pdf_xml_tag_open_beg(s, "rdf:li");
			pdf_xml_attribute_name(s, "xml:lang");
			pdf_xml_attribute_value(s, "x-default");
			pdf_xml_tag_end(s);
			{
			   pdf_xml_data_write(s, info.Copyright.data, info.Copyright.size);
			}
			pdf_xml_tag_close(s, "rdf:li");
		    }
		    pdf_xml_tag_close(s, "rdf:Alt");
		}
		pdf_xml_tag_close(s, "xmpRights:Copyright");

		/* Don't have an Owner, write Copyright instead. */
		pdf_xml_tag_open_beg(s, "xmpRights:Owner");
		pdf_xml_tag_end(s);
		{
		    pdf_xml_tag_open(s, "rdf:Alt");
		    {
			pdf_xml_tag_open_beg(s, "rdf:li");
			pdf_xml_attribute_name(s, "xml:lang");
			pdf_xml_attribute_value(s, "x-default");
			pdf_xml_tag_end(s);
			{
			   pdf_xml_data_write(s, info.Copyright.data, info.Copyright.size);
			}
			pdf_xml_tag_close(s, "rdf:li");
		    }
		    pdf_xml_tag_close(s, "rdf:Alt");
		}
		pdf_xml_tag_close(s, "xmpRights:Owner");
	    }
	    {
		pdf_xml_tag_open_beg(s, "xmpRights:Marked");
		pdf_xml_tag_end(s);
		{
		    pdf_xml_string_write(s, "True");
		}
		pdf_xml_tag_close(s, "xmpRights:Marked");
	    }
	    if (info.members & FONT_INFO_NOTICE) {
		pdf_xml_tag_open_beg(s, "xmpRights:UsageTerms");
		pdf_xml_tag_end(s);
		{
		    pdf_xml_tag_open(s, "rdf:Alt");
		    {
			pdf_xml_tag_open_beg(s, "rdf:li");
			pdf_xml_attribute_name(s, "xml:lang");
			pdf_xml_attribute_value(s, "x-default");
			pdf_xml_tag_end(s);
			{
			   pdf_xml_data_write(s, info.Notice.data, info.Notice.size);
			}
			pdf_xml_tag_close(s, "rdf:li");
		    }
		    pdf_xml_tag_close(s, "rdf:Alt");
		}
		pdf_xml_tag_close(s, "xmpRights:UsageTerms");
	    }
	    pdf_xml_tag_close(s, "rdf:Description");
	    pdf_xml_newline(s);
	}
	pdf_xml_copy(s, "</rdf:RDF>\n");
    }
    pdf_xml_copy(s, "</x:xmpmeta>\n");

    pdf_xml_copy(s, "                                                                        \n");
    pdf_xml_copy(s, "                                                                        \n");
    pdf_xml_copy(s, "<?xpacket end='w'?>");
    return 0;
}

int
pdf_font_metadata(gx_device_pdf *pdev, const pdf_base_font_t *pbfont, 
		  const byte *digest, int digest_length, gs_id *metadata_object_id)
{  
    /* Acrobat Distiller does not create font Metadata, and the ISO spec says 
     * only that files 'should' contain font Metadata. Attempts to include 
     * font metadata cause Acrobat to complain about XMP schemas (because 
     * the ISO spec uses non-existent tags).
     * For now we disable writing of font Metadata
     */
    return 0;

    *metadata_object_id = gs_no_id;
    if (pdev->CompatibilityLevel < 1.4)
	return 0;
    /* The PDF/A specification redss about 
	"embedded Type 0, Type 1, or TrueType font programs",
	but we believe that "embedded Type 0 font programs"
	do not exist.
	We create Metadata for Type 1,2,42,9,11.
    */
    if (pdev->PDFA) {
	pdf_resource_t *pres;
	int code;
	int options = DATA_STREAM_NOT_BINARY;

	if (pdev->EncryptMetadata)
	    options |= DATA_STREAM_ENCRYPT;
	code = pdf_open_aside(pdev, resourceOther, gs_no_id, &pres, true, options);
	if (code < 0)
	    return code;
	code = pdf_write_font_metadata(pdev, pbfont, digest, digest_length);
	if (code < 0)
	    return code;
	code = pdf_close_aside(pdev);
	if (code < 0)
	    return code;
	code = COS_WRITE_OBJECT(pres->object, pdev);
	if (code < 0)
	    return code;
	*metadata_object_id = pres->object->id;
    }
    return 0;
}

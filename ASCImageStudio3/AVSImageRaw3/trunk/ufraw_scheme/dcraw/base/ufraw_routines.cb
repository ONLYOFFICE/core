/*
 * UFRaw - Unidentified Flying Raw converter for digital camera images
 *
 * ufraw_routines.c - general routines
 * Copyright 2004-2008 by Udi Fuchs
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(HAVE_CANONICALIZE_FILE_NAME) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE /* needed for canonicalize_file_name() */
#endif

#include <errno.h>
#include <locale.h>
#include <stdlib.h> /* needed for canonicalize_file_name() */
#include <string.h>
#include "uf_glib.h"
#include <glib/gi18n.h>
#include "ufraw.h"

/* we start by some general purpose functions that mostly take care of
 * making the rest of the code platform independ */

const char *uf_get_home_dir()
{
    const char *hd = g_get_home_dir();
    if (hd==NULL)
#ifdef WIN32
	hd = "C:\\";
#else
	hd = "/";
#endif
    return hd;
}

void uf_init_locale(const char *exename)
{
    const char *locale = setlocale(LC_ALL, "");
    /* Disable the Hebrew and Arabic locale, since the right-to-left setting
     * does not go well with the preview window. */
    if ( locale!=NULL &&
	(!strncmp(locale, "he", 2) || !strncmp(locale, "iw", 2) ||
	!strncmp(locale, "ar", 2) ||
	!strncmp(locale, "Hebrew", 6) || !strncmp(locale, "Arabic", 6) ) ) {
	/* I'm not sure why the following doesn't work (on Windows at least) */
	/* locale = setlocale(LC_ALL, "C");
	 * gtk_disable_setlocale(); */
	/* so I'm using setenv */
	g_setenv("LC_ALL", "C", TRUE);
    }
    /* Try getting the localedir from the environment */
    char *localedir = g_strconcat(g_getenv("UFRAW_LOCALEDIR"), NULL);
    if (localedir==NULL) {
	/* If that fails, there are two defaults: */
#ifdef WIN32
	/* In Windows the localedir is found relative to the exe file.
	 * The exact location here should match ufraw-setup.iss.in */
	char *basename = g_path_get_basename(exename);
	if ( strcasecmp(basename, "ufraw-gimp.exe")==0 ) {
	    localedir = g_strconcat(g_path_get_dirname(exename),
		    "/../../../locale", NULL);
	} else {
	    localedir = g_strconcat(g_path_get_dirname(exename),
		    "/../lib/locale", NULL);
	}
	g_free(basename);
#else
	exename = exename; /* suppress warning */
	/* In other environments localedir is set at compile time */
	localedir = g_strconcat(UFRAW_LOCALEDIR, NULL);
#endif
    }
    bindtextdomain("ufraw", localedir);
    g_free(localedir);
    bind_textdomain_codeset("ufraw", "UTF-8");
    textdomain("ufraw");
}

char *uf_file_set_type(const char *filename, const char *type)
{
    char *infile = (char *)filename, *outfile, *tmpfile=NULL, *dotPosition;
    if ( (dotPosition=strrchr(infile, '.'))==NULL) {
	outfile = g_strconcat(infile, type, NULL);
	return outfile;
    }
    if ( strcasecmp(dotPosition, ".gz")==0 ||
	 strcasecmp(dotPosition, ".bz2")==0 ) {
	char *tmpfile = g_strndup(infile, dotPosition - infile);
	if ( (dotPosition=strrchr(tmpfile, '.'))==NULL) {
	    outfile = g_strconcat(tmpfile, type, NULL);
	    g_free(tmpfile);
	    return outfile;
	}
	infile = tmpfile;
    }
    outfile = g_new(char, dotPosition - infile + strlen(type) + 1);
    g_strlcpy(outfile, infile, dotPosition - infile + 1);
    g_strlcpy(outfile + (dotPosition - infile), type, strlen(type) + 1);
    g_free(tmpfile);
    return outfile;
}

/* Make sure filename has asolute path */
char *uf_file_set_absolute(const char *filename)
{
    if (g_path_is_absolute(filename)) {
	return g_strdup(filename);
    } else {
#ifdef HAVE_CANONICALIZE_FILE_NAME
	// canonicalize_file_name() requires the file to exist.
	// This is why we need to split 'filename' to dirname and basename.
	char *path = g_path_get_dirname(filename);
	char *canon = canonicalize_file_name(path);
	if ( canon==NULL ) {
	    // We should never reach this code
	    g_message("Error in canonicalize_file_name(""%s""): %s",
		    path, strerror(errno));
	    g_free(path);
	    return g_strdup(filename);
	}
	// If filename ends with a separator there is no basename
	if ( strlen(path)==strlen(filename)-1 ) {
	    g_free(path);
	    return canon;
	}
	g_free(path);
	char *base = g_path_get_basename(filename);
	char *abs = g_build_filename(canon, base, NULL);
	g_free(base);
	g_free(canon);
	return abs;
#else
	// We could use realpath(filename, NULL)
	// if we add a check that it is not buggy
	// This code does not remove '/./' or '/../'
	char *cd = g_get_current_dir();
	char *fn = g_build_filename(cd, filename, NULL);
	g_free(cd);
	return fn;
#endif
    }
}

char *uf_markup_buf(char *buffer, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    char *line = g_markup_vprintf_escaped(format, ap);
    va_end(ap);
    if (buffer==NULL) {
	return line;
    } else {
	char *buf;
	buf = g_strconcat(buffer, line, NULL);
	g_free(line);
	g_free(buffer);
	return buf;
    }
}

const char raw_ext[]= "3fr,arw,bay,bmq,cine,cr2,crw,cs1,dc2,dcr,dng,erf,fff,"
	"hdr,ia,jpg,k25,kc2,kdc,mdc,mef,mos,mrw,nef,nrw,orf,pef,pxn,qtk,raf,"
	"raw,rdc,rw2,sr2,srf,sti,tif,ufraw,x3f";

const char *file_type[] = { ".ppm", ".ppm", ".tif", ".tif", ".jpg",
	".png", ".png", ".embedded.jpg", ".embedded.png", ".fits" };

/* Set locale of LC_NUMERIC to "C" to make sure that printf behaves correctly.*/
char *uf_set_locale_C()
{
    char *locale = NULL;
    char *test = g_markup_printf_escaped("%.1f", 1234.5);
    if ( strcmp(test, "1234.5")!=0 ) {
	locale = setlocale(LC_NUMERIC, NULL);
	if ( locale!=NULL ) {
	    locale = g_strdup(locale);
	} else {
	    ufraw_message(UFRAW_ERROR, _("Fatal error setting C locale"));
	}
	setlocale(LC_NUMERIC, "C");
	g_free(test);
	test = g_markup_printf_escaped("%.1f", 1234.5);
	if ( strcmp(test, "1234.5")!=0 ) {
	    ufraw_message(UFRAW_ERROR, _("Fatal error setting C locale"));
	    if ( locale!=NULL ) {
		setlocale(LC_NUMERIC, locale);
		g_free(locale);
		locale = NULL;
	    }
	}
    }
    g_free(test);
    return locale;
}

void uf_reset_locale(char *locale)
{
    if ( locale==NULL )
	return;
    setlocale(LC_NUMERIC, locale);
    g_free(locale);
}

double profile_default_linear(profile_data *p)
{
    if ( !strcmp(p->name, "No profile")
       || !strcmp(p->name, "Color matrix") )
	return 0.1;
    else
	return 0.0;
}

double profile_default_gamma(profile_data *p)
{
    if ( !strcmp(p->name, "No profile")
       || !strcmp(p->name, "Color matrix") )
	return 0.45;
    else if ( !strncmp(p->productName, "Nikon DBase for NEF", 19) /*For D100*/
	    || !strncmp(p->productName, "Nikon D1 for NEF", 16)
	    || !strncmp(p->productName, "Nikon D1/H/X for NEF", 20)
	    || !strncmp(p->productName, "Nikon D50 for NEF", 17)
	    || !strncmp(p->productName, "Nikon D70 for NEF", 17)
	    || !strncmp(p->productName, "Nikon D80 for NEF", 17)
	    || !strncmp(p->productName, "Nikon D100 for NEF", 18)
	    || !strncmp(p->productName, "Adobe RGB (1998)", 16) )
	return 0.45;
    else
	return 1.0;
}

/* Convert between Temperature and RGB.
 * Base on information from http://www.brucelindbloom.com/
 * The fit for D-illuminant between 4000K and 15000K are from CIE
 * The generalization to 2000K < T < 4000K and the blackbody fits
 * are my own and should be taken with a grain of salt.
 */
static const double XYZ_to_RGB[3][3] = {
    { 3.24071,	-0.969258,  0.0556352 },
    {-1.53726,	1.87599,    -0.203996 },
    {-0.498571,	0.0415557,  1.05707 } };

void Temperature_to_RGB(double T, double RGB[3])
{
    int c;
    double xD, yD, X, Y, Z, max;
    // Fit for CIE Daylight illuminant
    if (T<= 4000) {
	xD = 0.27475e9/(T*T*T) - 0.98598e6/(T*T) + 1.17444e3/T + 0.145986;
    } else if (T<= 7000) {
	xD = -4.6070e9/(T*T*T) + 2.9678e6/(T*T) + 0.09911e3/T + 0.244063;
    } else {
	xD = -2.0064e9/(T*T*T) + 1.9018e6/(T*T) + 0.24748e3/T + 0.237040;
    }
    yD = -3*xD*xD + 2.87*xD - 0.275;

    // Fit for Blackbody using CIE standard observer function at 2 degrees
    //xD = -1.8596e9/(T*T*T) + 1.37686e6/(T*T) + 0.360496e3/T + 0.232632;
    //yD = -2.6046*xD*xD + 2.6106*xD - 0.239156;

    // Fit for Blackbody using CIE standard observer function at 10 degrees
    //xD = -1.98883e9/(T*T*T) + 1.45155e6/(T*T) + 0.364774e3/T + 0.231136;
    //yD = -2.35563*xD*xD + 2.39688*xD - 0.196035;

    X = xD/yD;
    Y = 1;
    Z = (1-xD-yD)/yD;
    max = 0;
    for (c=0; c<3; c++) {
	RGB[c] = X*XYZ_to_RGB[0][c] + Y*XYZ_to_RGB[1][c] + Z*XYZ_to_RGB[2][c];
	if (RGB[c]>max) max = RGB[c];
    }
    for (c=0; c<3; c++) RGB[c] = RGB[c]/max;
}

void RGB_to_Temperature(double RGB[3], double *T, double *Green)
{
    double Tmax, Tmin, testRGB[3];
    Tmin = 2000;
    Tmax = 15000;
    for (*T=(Tmax+Tmin)/2; Tmax-Tmin>10; *T=(Tmax+Tmin)/2) {
	Temperature_to_RGB(*T, testRGB);
	if (testRGB[2]/testRGB[0] > RGB[2]/RGB[0])
	    Tmax = *T;
	else
	    Tmin = *T;
    }
    *Green = (testRGB[1]/testRGB[0]) / (RGB[1]/RGB[0]);
}

static void curve_parse_start(GMarkupParseContext *context,
    const gchar *element, const gchar **names, const gchar **values,
    gpointer user, GError **error)
{
    CurveData *c = user;
    int int_value;
    GQuark ufrawQuark = g_quark_from_static_string("UFRaw");

    context = context;
    while (*names!=NULL) {
	sscanf(*values, "%d", &int_value);
	if (!strcmp(element, "Curve") && !strcmp(*names, "Version")) {
	    /* We never changed the curve format so we support all
	     * previous versions */
	    if (int_value>conf_default.version)
		g_set_error(error, ufrawQuark, UFRAW_RC_VERSION,
		    _("Curve version is not supported"));
	}
	names++;
	values++;
    }
    if (!strcmp("Curve", element)) {
	/* m_gamma==-1 marks that we are inside a XML Curve block.
	 * This is ok since we never set m_gamma. */
	c->m_gamma = -1.0;
	/* m_numAnchors==0 marks that no anchors where read from the XML */
	c->m_numAnchors = 0;
    }
}

static void curve_parse_end(GMarkupParseContext *context, const gchar *element,
	gpointer user, GError **error)
{
    CurveData *c = user;
    context = context;
    error = error;
    if (!strcmp("Curve", element)) {
	c->m_gamma = conf_default.curve[0].m_gamma;
	if (c->m_numAnchors==0)
	    c->m_numAnchors = conf_default.curve[0].m_numAnchors;
    }
}

static void curve_parse_text(GMarkupParseContext *context, const gchar *text,
	gsize len, gpointer user, GError **error)
{
    CurveData *c = user;
    const gchar *element = g_markup_parse_context_get_element(context);
    char temp[max_path];
    error = error;
    for(; len>0 && g_ascii_isspace(*text); len--, text++);
    for(; len>0 && g_ascii_isspace(text[len-1]); len--);
    if (len==0) return;
    if (len>max_path-1) len=max_path-1;
    strncpy(temp, text, len);
    temp[len] = '\0';
    if (!strcmp("Curve", element)) {
	g_strlcpy(c->name, temp, max_name);
    }
    /* A negative gamma marks that we are in a Curve XML block */
    if (c->m_gamma < 0) {
	if (!strcmp("MinXY", element)) {
	    sscanf(temp, "%lf %lf", &c->m_min_x, &c->m_min_y);
	    c->m_min_x = LIM(c->m_min_x, 0, 1);
	    c->m_min_y = LIM(c->m_min_y, 0, 1);
	}
	if (!strcmp("MaxXY", element)) {
	    sscanf(temp, "%lf %lf", &c->m_max_x, &c->m_max_y);
	    c->m_max_x = LIM(c->m_max_x, 0, 1);
	    c->m_max_y = LIM(c->m_max_y, 0, 1);
	}
	if (!strcmp("AnchorXY", element)) {
	    /* If one anchor is supplied then all anchors should be supplied */
	    sscanf(temp, "%lf %lf",
		    &c->m_anchors[c->m_numAnchors].x,
		    &c->m_anchors[c->m_numAnchors].y);
	    c->m_anchors[c->m_numAnchors].x = 
	     	LIM(c->m_anchors[c->m_numAnchors].x, 0, 1);
	    c->m_anchors[c->m_numAnchors].y = 
	     	LIM(c->m_anchors[c->m_numAnchors].y, 0, 1);
	    c->m_numAnchors++;
	}
    }
}

int curve_load(CurveData *cp, char *filename)
{
    NikonData data;

    if ( !strcasecmp(filename + strlen(filename) - 4, ".ntc") ||
	 !strcasecmp(filename + strlen(filename) - 4, ".ncv") ) {
	/* Try loading ntc/ncv files */
	if (LoadNikonData(filename, &data)!=UFRAW_SUCCESS) {
	    ufraw_message(UFRAW_ERROR, _("Invalid Nikon curve file '%s'"),
		    filename);
	    return UFRAW_ERROR;
	}
	*cp = data.curves[TONE_CURVE];
    } else {
	/* Load UFRaw's curve file format */
	char line[max_path], *locale;
	FILE *in;
	GMarkupParser parser={&curve_parse_start, &curve_parse_end,
		&curve_parse_text, NULL, NULL};
	GMarkupParseContext *context;
	GError *err = NULL;

	*cp = conf_default.curve[0];
	if ( (in=g_fopen(filename, "r"))==NULL ) {
	    ufraw_message(UFRAW_ERROR, _("Error opening Curve file '%s': %s"),
			    filename, strerror(errno));
	    return UFRAW_ERROR;
	}
	locale = uf_set_locale_C();
	context = g_markup_parse_context_new(&parser, 0, cp, NULL);
	line[max_path-1] = '\0';
	fgets(line, max_path-1, in);
	while (!feof(in)) {
	    if (!g_markup_parse_context_parse(context, line,
		    strlen(line), &err)) {
		ufraw_message(UFRAW_ERROR, _("Error parsing '%s'\n%s"),
			filename, err->message);
		g_markup_parse_context_free(context);
		uf_reset_locale(locale);
		fclose(in);
		g_error_free(err);
		uf_reset_locale(locale);
		return UFRAW_ERROR;
	    }
	    fgets(line, max_path, in);
	}
	g_markup_parse_context_end_parse(context, NULL);
	g_markup_parse_context_free(context);
	uf_reset_locale(locale);
	fclose(in);
    }
    char *base = g_path_get_basename(filename);
    char *name = uf_file_set_type(base, "");
    char *utf8 = g_filename_display_name(name);
    g_strlcpy(cp->name, utf8, max_name);
    g_free(utf8);
    g_free(name);
    g_free(base);
    return UFRAW_SUCCESS;
}

int curve_save(CurveData *cp, char *filename)
{
    int nikon_file_type = -1;

    /* Try saving ntc/ncv format */
    if ( !strcasecmp(filename+strlen(filename)-4, ".ntc") )
	nikon_file_type = NTC_FILE;
    else if (!strcasecmp(filename+strlen(filename)-4, ".ncv") )
	nikon_file_type = NCV_FILE;

    //if it's ntc or ncv
    if (nikon_file_type != -1)  {
	NikonData data;

	//clear it out
	memset(&data,0,sizeof(NikonData));

	data.curves[TONE_CURVE] = *cp;

	if (SaveNikonDataFile(&data, filename, nikon_file_type,
			    NIKON_VERSION_4_1)!=UFRAW_SUCCESS) {
	    ufraw_message(UFRAW_ERROR, _("Invalid Nikon curve file '%s'"),
			filename);
	    return UFRAW_ERROR;
	}
    } else {
	/* Save UFRaw's curve format */
	FILE *out;

	if ( (out=g_fopen(filename, "w"))==NULL ) {
	    ufraw_message(UFRAW_ERROR, _("Error opening file '%s': %s"),
			filename, g_strerror(errno));
	    return UFRAW_ERROR;
	}
	char *locale = uf_set_locale_C();
	fprintf(out, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	char *base = g_path_get_basename(filename);
	char *name = uf_file_set_type(base, "");
	char *utf8 = g_filename_display_name(name);
	fprintf(out, "<Curve Version='%d'>%s\n", conf_default.version, utf8);
	g_free(utf8);
	g_free(name);
	g_free(base);
	char *buf = curve_buffer(cp);
	if (buf!=NULL) fprintf(out, buf);
	g_free(buf);
	fprintf(out, "</Curve>\n");
	uf_reset_locale(locale);
	fclose(out);
    }
    return UFRAW_SUCCESS;
}

char *curve_buffer(CurveData *c)
{
    char *buf = NULL;
    int i;
    if ( c->m_min_x!=conf_default.curve[0].m_min_x ||
	 c->m_min_y!=conf_default.curve[0].m_min_y ||
	 c->m_max_x!=conf_default.curve[0].m_max_x ||
	 c->m_max_y!=conf_default.curve[0].m_max_y ) {
	buf = uf_markup_buf(buf,
		"\t<MinXY>%lf %lf</MinXY>\n", c->m_min_x, c->m_min_y);
	buf = uf_markup_buf(buf,
		"\t<MaxXY>%lf %lf</MaxXY>\n", c->m_max_x, c->m_max_y);
    }
    if ( c->m_numAnchors!=conf_default.curve[0].m_numAnchors ||
	 c->m_anchors[0].x!=conf_default.curve[0].m_anchors[0].x ||
	 c->m_anchors[0].y!=conf_default.curve[0].m_anchors[0].y ||
	 c->m_anchors[1].x!=conf_default.curve[0].m_anchors[1].x ||
	 c->m_anchors[1].y!=conf_default.curve[0].m_anchors[1].y ) {
	for (i=0; i<c->m_numAnchors; i++)
	    buf = uf_markup_buf(buf,
		    "\t<AnchorXY>%lf %lf</AnchorXY>\n",
		    c->m_anchors[i].x, c->m_anchors[i].y);
    }
    return buf;
}

int ptr_array_insert_sorted (
    GPtrArray *array, const void *item, GCompareFunc compare)
{
    int length = array->len;
    g_ptr_array_set_size (array, length + 1);
    const void **root = (const void **)array->pdata;

    int m = 0, l = 0, r = length - 1;

    // Skip trailing NULL, if any
    if (l <= r && !root [r])
        r--;
    
    while (l <= r)
    {
        m = (l + r) / 2;
        int cmp = compare (root [m], item);

        if (cmp == 0)
        {
            ++m;
            goto done;
        }
        else if (cmp < 0)
            l = m + 1;
        else
            r = m - 1;
    }
    if (r == m)
        m++;

done:
    memmove (root + m + 1, root + m, (length - m) * sizeof (void *));
    root [m] = item;
    return m;
}

int ptr_array_find_sorted (
    const GPtrArray *array, const void *item, GCompareFunc compare)
{
    int length = array->len;
    void **root = array->pdata;

    int l = 0, r = length - 1;
    int m = 0, cmp = 0;

    if (!length)
        return -1;

    // Skip trailing NULL, if any
    if (!root [r])
        r--;

    while (l <= r)
    {
        m = (l + r) / 2;
        cmp = compare (root [m], item);

        if (cmp == 0)
            return m;
        else if (cmp < 0)
            l = m + 1;
        else
            r = m - 1;
    }
    
    return -1;
}

void ptr_array_insert_index (
    GPtrArray *array, const void *item, int index)
{
    const void **root;
    int length = array->len;
    g_ptr_array_set_size (array, length + 1);
    root = (const void **)array->pdata;
    memmove (root + index + 1, root + index, (length - index) * sizeof (void *));
    root [index] = item;
}

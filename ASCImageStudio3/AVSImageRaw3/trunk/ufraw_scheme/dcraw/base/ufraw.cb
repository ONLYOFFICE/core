/*
 * UFRaw - Unidentified Flying Raw converter for digital camera images
 *
 * ufraw.c - The standalone interface to UFRaw.
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

#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <errno.h>     /* for errno */
#include <string.h>
#include "uf_glib.h"
#include <glib/gi18n.h>
#include "uf_gtk.h"
#include "ufraw.h"

char *ufraw_binary;

int main (int argc, char **argv)
{
    ufraw_data *uf;
    conf_data rc, cmd, conf;
    int status;
    GtkWidget *dummyWindow=NULL;
    int optInd;
    int plugin = 0;

    char *argFile = uf_win32_locale_to_utf8(argv[0]);
    ufraw_binary = g_path_get_basename(argFile);
    uf_init_locale(argFile);
    uf_win32_locale_free(argFile);
    char *gtkrcfile = g_build_filename(uf_get_home_dir(),
	".ufraw-gtkrc", NULL);
    gtk_rc_add_default_file(gtkrcfile);
    g_free(gtkrcfile);
    gtk_init(&argc, &argv);
    ufraw_icons_init();
#ifdef WIN32
    dummyWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    uf_window_set_icon_name(GTK_WINDOW(dummyWindow), "ufraw");
    ufraw_message(UFRAW_SET_PARENT, (char *)dummyWindow);
#endif
    /* Load $HOME/.ufrawrc */
    conf_load(&rc, NULL);

    /* Half interpolation is an option only for the GIMP plug-in.
     * For the stand-alone tool it is disabled */
    if (rc.interpolation==half_interpolation)
	rc.interpolation = ahd_interpolation;

    if ( !rc.RememberOutputPath )
	g_strlcpy(rc.outputPath, "", max_path);
    g_strlcpy(rc.inputFilename, "", max_path);
    g_strlcpy(rc.outputFilename, "", max_path);

    /* Put the command-line options in cmd */
    optInd = ufraw_process_args(&argc, &argv, &cmd, &rc);
    if ( strlen(cmd.outputFilename)!=0 ) {
	    plugin = 3;
    }
    if ( cmd.silent ) {
	ufraw_message(UFRAW_ERROR,
		_("--silent option is valid only in batch mode"));
	optInd = -1;
    }
    if ( cmd.rotate!=-1 ) {
	ufraw_message(UFRAW_ERROR,
		_("--rotate option is valid only in batch mode"));
	optInd = -1;
    }
    if ( cmd.embeddedImage ) {
	ufraw_message(UFRAW_ERROR, _("Extracting embedded image is not supported in interactive mode"));
	optInd = -1;
    }
    if (optInd<0) exit(1);
    if (optInd==0) exit(0);

    /* Create a dummyWindow for the GUI error messenger */
    if (dummyWindow==NULL) {
	dummyWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	uf_window_set_icon_name(GTK_WINDOW(dummyWindow), "ufraw");
	ufraw_message(UFRAW_SET_PARENT, (char *)dummyWindow);
    }
    /* Load the --conf file. version==0 means ignore conf. */
    conf.version = 0;
    if (strlen(cmd.inputFilename)>0) {
	status = conf_load(&conf, cmd.inputFilename);
	if (status==UFRAW_SUCCESS) {
	    strcpy(conf.inputFilename, "");
	    strcpy(conf.outputFilename, "");
	    strcpy(conf.outputPath, "");
	} else {
	    ufraw_message(UFRAW_REPORT, NULL);
	    conf.version = 0;
	}
    }
    if (optInd==argc) {
	status = ufraw_config(NULL, &rc, &conf, &cmd);
	if (status==UFRAW_ERROR) exit(1);
	ufraw_chooser(&rc, NULL);
//	ufraw_close(cmd.darkframe);
	exit(0);
    }
    /* If there only one argument and it is a directory, use it as the
     * default directory for the file-chooser */
    argFile = uf_win32_locale_to_utf8(argv[optInd]);
    if (optInd==argc-1 && g_file_test(argFile,G_FILE_TEST_IS_DIR)) {
	status = ufraw_config(NULL, &rc, &conf, &cmd);
	if (status==UFRAW_ERROR) exit(1);
	ufraw_chooser(&rc, argFile);
	uf_win32_locale_free(argFile);
//	ufraw_close(cmd.darkframe);
	exit(0);
    }
    uf_win32_locale_free(argFile);

    int exitCode = 0;
    for (; optInd<argc; optInd++) {
	argFile = uf_win32_locale_to_utf8(argv[optInd]);
	uf = ufraw_open(argFile);
	uf_win32_locale_free(argFile);
	if (uf==NULL) {
	    exitCode = 1;
	    ufraw_message(UFRAW_REPORT, NULL);
	    continue;
	}
	status = ufraw_config(uf, &rc, &conf, &cmd);
	if (status==UFRAW_ERROR) {
	    ufraw_close(uf);
	    g_free(uf);
	    exit(1);
	}
	ufraw_preview(uf, plugin, NULL);
	conf_copy_image(&rc, uf->conf);
	conf_copy_save(&rc, uf->conf);
	g_free(uf);
    }
    if (dummyWindow!=NULL) gtk_widget_destroy(dummyWindow);

//    ufraw_close(cmd.darkframe);
    exit(exitCode);
}

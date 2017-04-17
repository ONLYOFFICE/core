NULL =

EXTRA_DIST = \
	README \
	$(NULL)

lib_LTLIBRARIES = \
	libxmlsec1-skeleton.la \
	$(NULL)

libxmlsec1_skeleton_la_CPPFLAGS = \
	-DPACKAGE=\"@PACKAGE@\" \
	-I../../include \
	-I$(top_srcdir)/include \
	$(XMLSEC_DEFINES) \
	$(XMLSEC_SKELETON_DEFINES) \
	$(SKELETON_CFLAGS) \
	$(LIBXSLT_CFLAGS) \
	$(LIBXML_CFLAGS) \
	$(NULL)

libxmlsec1_skeleton_la_SOURCES =\
	app.c \
	crypto.c \
	globals.h \
	$(NULL)

if SHAREDLIB_HACK
libxmlsec1_skeleton_la_SOURCES += ../strings.c
endif

libxmlsec1_skeleton_la_LIBADD = \
	$(SKELETON_LIBS) \
	$(LIBXSLT_LIBS) \
	$(LIBXML_LIBS) \
	../libxmlsec1.la \
	$(NULL)

libxmlsec1_skeleton_la_DEPENDENCIES = \
	$(NULL)

libxmlsec1_skeleton_la_LDFLAGS = \
	@XMLSEC_CRYPTO_EXTRA_LDFLAGS@ \
	-version-info @XMLSEC_VERSION_INFO@ \
	$(NULL)

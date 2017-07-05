NULL =

SUBDIRS = . $(XMLSEC_CRYPTO_LIST)

AM_CFLAGS = \
	-DPACKAGE=\"@PACKAGE@\" \
	-DXMLSEC_DEFAULT_CRYPTO=\"@XMLSEC_DEFAULT_CRYPTO@\" \
	-I../include \
	-I$(top_srcdir)/include \
	$(XMLSEC_DEFINES) \
	$(XMLSEC_DL_INCLUDES) \
	$(LIBXSLT_CFLAGS) \
	$(LIBXML_CFLAGS) \
	$(NULL)

EXTRA_DIST = \
	globals.h \
	kw_aes_des.h \
	skeleton \
	mscrypto \
	$(XMLSEC_CRYPTO_DISABLED_LIST) \
	$(NULL)

lib_LTLIBRARIES = \
	libxmlsec1.la \
	$(NULL)

libxmlsec1_la_SOURCES = \
	$(LTDL_SOURCE_FILES) \
	app.c \
	base64.c \
	bn.c \
	buffer.c \
	c14n.c \
	dl.c \
	enveloped.c \
	errors.c \
	io.c \
	keyinfo.c \
	keys.c \
	keysdata.c \
	keysmngr.c \
	kw_aes_des.c \
	list.c \
	membuf.c \
	nodeset.c \
	parser.c \
	relationship.c \
	soap.c \
	strings.c \
	templates.c \
	transforms.c \
	x509.c \
	xmldsig.c \
	xmlenc.c \
	xmlsec.c \
	xmltree.c \
	xpath.c \
	xslt.c \
	$(NULL)

libxmlsec1_la_LIBADD = \
	$(LIBXSLT_LIBS) \
	$(LIBXML_LIBS) \
	$(XMLSEC_DL_LIBS) \
	$(NULL)

libxmlsec1_la_LDFLAGS = \
	@XMLSEC_EXTRA_LDFLAGS@ \
	-version-info @XMLSEC_VERSION_INFO@ \
	$(NULL)


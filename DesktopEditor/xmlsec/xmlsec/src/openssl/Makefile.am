NULL =

EXTRA_DIST = \
	README \
	$(NULL)

lib_LTLIBRARIES = \
	libxmlsec1-openssl.la \
	$(NULL)

libxmlsec1_openssl_la_CPPFLAGS = \
	-DPACKAGE=\"@PACKAGE@\" \
	-I../../include \
	-I$(top_srcdir)/include \
	$(XMLSEC_DEFINES) \
	$(OPENSSL_CFLAGS) \
	$(LIBXSLT_CFLAGS) \
	$(LIBXML_CFLAGS) \
	$(NULL)

libxmlsec1_openssl_la_SOURCES =\
	app.c \
	bn.c \
	ciphers.c \
	crypto.c \
	digests.c \
	evp.c \
	evp_signatures.c \
	hmac.c \
	kw_aes.c \
	kw_des.c \
	kt_rsa.c \
	signatures.c \
	symkeys.c \
	x509.c \
	x509vfy.c \
	globals.h \
	openssl11_wrapper.h \
	$(NULL)

if SHAREDLIB_HACK
libxmlsec1_openssl_la_SOURCES += ../strings.c
endif

libxmlsec1_openssl_la_LIBADD = \
	$(OPENSSL_LIBS) \
	$(LIBXSLT_LIBS) \
	$(LIBXML_LIBS) \
	../libxmlsec1.la \
	$(NULL)

libxmlsec1_openssl_la_DEPENDENCIES = \
	$(NULL)

libxmlsec1_openssl_la_LDFLAGS = \
	@XMLSEC_CRYPTO_EXTRA_LDFLAGS@ \
	-version-info @XMLSEC_VERSION_INFO@ \
	$(NULL)

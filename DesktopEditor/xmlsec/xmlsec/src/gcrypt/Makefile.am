NULL =

EXTRA_DIST = \
	README \
	$(NULL)

lib_LTLIBRARIES = \
	libxmlsec1-gcrypt.la \
	$(NULL)

libxmlsec1_gcrypt_la_CPPFLAGS = \
	-DPACKAGE=\"@PACKAGE@\" \
	-DGCRYPT_MIN_VERSION=\"$(GCRYPT_MIN_VERSION)\" \
	-I../../include \
	-I$(top_srcdir)/include \
	$(XMLSEC_DEFINES) \
	$(GCRYPT_CFLAGS) \
	$(LIBXSLT_CFLAGS) \
	$(LIBXML_CFLAGS) \
	$(NULL)

libxmlsec1_gcrypt_la_SOURCES =\
	app.c \
	asn1.h \
	asn1.c \
	ciphers.c \
	crypto.c \
	digests.c \
	hmac.c \
	kw_aes.c \
	kw_des.c \
	symkeys.c \
	asymkeys.c \
	signatures.c \
	globals.h \
	$(NULL)

if SHAREDLIB_HACK
libxmlsec1_gcrypt_la_SOURCES += ../strings.c
endif

libxmlsec1_gcrypt_la_LIBADD = \
	$(GCRYPT_LIBS) \
	$(LIBXSLT_LIBS) \
	$(LIBXML_LIBS) \
	../libxmlsec1.la \
	$(NULL)

libxmlsec1_gcrypt_la_DEPENDENCIES = \
	$(NULL)

libxmlsec1_gcrypt_la_LDFLAGS = \
	@XMLSEC_CRYPTO_EXTRA_LDFLAGS@ \
	-version-info @XMLSEC_VERSION_INFO@ \
	$(NULL)

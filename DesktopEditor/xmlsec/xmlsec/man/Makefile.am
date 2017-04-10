NULL =

XMLSEC1_MAN=$(builddir)/xmlsec1.1
XMLSEC1_CONFIG_MAN=$(builddir)/xmlsec1-config.1

XMLSEC1_MAN_SRC=$(srcdir)/xmlsec1.1
XMLSEC1_CONFIG_MAN_SRC=$(srcdir)/xmlsec1-config.1

man_MANS = \
	$(XMLSEC1_MAN) \
	$(XMLSEC1_CONFIG_MAN) \
	$(NULL)

EXTRA_DIST = \
	$(XMLSEC1_MAN) \
	$(XMLSEC1_CONFIG_MAN) \
	$(NULL)

XMLSEC_PROGRAM=$(top_builddir)/apps/xmlsec1
XMLSEC_CONFIG_PROGRAM=$(top_builddir)/xmlsec1-config

all: $(man_MANS) $(XMLSEC_HTML)


if BUILD_MANPAGES
$(XMLSEC1_MAN): $(XMLSEC_PROGRAM)
	$(HELP2MAN) --help-option=--help-all --no-info \
		  --name="sign, verify, encrypt and decrypt XML documents" \
		  --version-option=--version \
		  --output=$(XMLSEC1_MAN) \
		  $(XMLSEC_PROGRAM)
else
$(XMLSEC1_MAN):
	@( \
		echo "Copying xmlsec1 manpage ..."; \
		if [ z"$(XMLSEC1_MAN_SRC)" != z"$(XMLSEC1_MAN)" ]; \
		then \
		   $(CP) -u $(XMLSEC1_MAN_SRC) $(XMLSEC1_MAN) ; \
		fi \
	)
endif

if BUILD_MANPAGES
$(XMLSEC1_CONFIG_MAN): $(XMLSEC_CONFIG_PROGRAM)
	$(HELP2MAN) --help-option=--help --no-info \
		  --name="detail installed version of xmlsec library" \
		  --version-option=--version \
		  --output=$(XMLSEC1_CONFIG_MAN) \
		  $(XMLSEC_CONFIG_PROGRAM)
else
$(XMLSEC1_CONFIG_MAN):
	@( \
		echo "Copying xmlsec1-config manpage ..."; \
		if [ z"$(XMLSEC1_CONFIG_MAN_SRC)" != z"$(XMLSEC1_CONFIG_MAN)" ]; then \
		    $(CP) -u $(XMLSEC1_CONFIG_MAN_SRC) $(XMLSEC1_CONFIG_MAN) ; \
		fi \
	)
endif

$(XMLSEC_PROGRAM): 
	@cd ../apps;make

$(XMLSEC_CONFIG_PROGRAM):

	
clean-local:
	( if [ z"$(XMLSEC1_MAN_SRC)" != z"$(XMLSEC1_MAN)" ]; then \
		rm -rf $(XMLSEC1_MAN) ; \
	fi )
	( if [ z"$(XMLSEC1_CONFIG_MAN_SRC)" != z"$(XMLSEC1_CONFIG_MAN)" ];  then \
		rm -rf $(XMLSEC1_CONFIG_MAN) ; \
	fi )

distclean-local: clean-local


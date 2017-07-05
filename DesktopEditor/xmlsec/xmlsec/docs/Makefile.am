NULL =

SUBDIRS=api

TARGET_DIR=@XMLSEC_DOCDIR@

EXTRA_DIST=\
	$(builddir)/images     \
	$(builddir)/*.html     \
	$(builddir)/*.ico      \
	$(builddir)/xmlsec.xsl \
	$(NULL)

XMLSEC1_MAN=$(top_builddir)/man/xmlsec1.1
XMLSEC_HTML=$(builddir)/xmlsec-man.html

all: docs

# docs is legacy
docs: docs-copy man-docs docs-format

docs-copy:
	@( \
		echo "Copying docs..."; \
		if [ z"$(srcdir)" != z"$(builddir)" ]; \
		then \
		    $(CP) -ru $(srcdir)/*.html $(srcdir)/*.ico $(srcdir)/images $(builddir)/  ; \
		    chmod u+w $(builddir)/*.html ; \
		    chmod u+w $(builddir)/*.ico ; \
		    chmod -R u+w $(builddir)/images ; \
		fi \
	)

# if we build docs then we also have xsltproc
if BUILD_MAN_DOCS
man-docs: $(XMLSEC_HTML) docs-copy

$(XMLSEC_HTML): docs-copy $(XMLSEC1_MAN)
	$(MAN2HTML) $(XMLSEC1_MAN) | \
		  grep -v '^Content-type: text/html' | \
		  tr "[:cntrl:]" " " > \
		  $(XMLSEC_HTML)
else
# do nothing, we aready copied this file
man-docs:

endif

if HAS_XSLTPROC
docs-format:
	@(echo "Formatting html documents"; \
	for i in `find $(builddir) -name "*.html" -print`; \
	do \
	    top_folder=`echo $$i | sed 's#/[^/]*$$#/#' | sed 's#\./##' | \
			sed 's#[^/]*/#../#g'`; \
	    echo "Processing $$i (topfolder='$$top_folder')"; \
	    $(XSLTPROC) --html --stringparam topfolder "$$top_folder" \
		     --output $$i.tmp $(srcdir)/xmlsec.xsl $$i; \
	    if [ $$? != 0 ]; \
	    then \
			echo "ERROR: processing file $$i"; \
			exit 1; \
	    fi; \
	    mv $$i.tmp $$i; \
	done)
	@(echo "Cleaning html documents"; \
	for i in `find $(builddir) -name "*.html" -print`; \
	do \
	    echo Cleaning $$i ; \
	    sed 's/\xA0/ /g' $$i > $$i.tmp ; \
	    mv $$i.tmp $$i ; \
	done)
else
# do nothing
docs-format:

endif

clean-local:
	-rm -f *.stamp
	( if [ z"$(srcdir)" != z"$(builddir)" ] ; then \
		chmod -R u+w $(builddir)/images && rm -rf $(builddir)/images ; \
		chmod -R u+w $(builddir)/src && rm -rf $(builddir)/src ; \
		(for i in `find $(builddir) -name "*.html" -print` ; do \
			echo "Removing files '$$i' ... " ; \
			chmod -R u+w $$i && rm -f $$i ; \
		done ) ; \
		(for i in `find $(builddir) -name "*.ico" -print` ; do \
			echo "Removing files '$$i' ... " ; \
			chmod -R u+w $$i && rm -f $$i ; \
		done ) ; \
	 fi ; )

distclean-local: clean-local

install-data-local: 
	$(mkinstalldirs) $(DESTDIR)$(TARGET_DIR)
	$(mkinstalldirs) $(DESTDIR)$(TARGET_DIR)/images
	$(mkinstalldirs) $(DESTDIR)$(TARGET_DIR)/api
	$(mkinstalldirs) $(DESTDIR)$(TARGET_DIR)/api/images
	-@INSTALL@ -m 0644 $(builddir)/*.html $(DESTDIR)$(TARGET_DIR)
	-@INSTALL@ -m 0644 $(builddir)/*.ico  $(DESTDIR)$(TARGET_DIR)
	-@INSTALL@ -m 0644 $(builddir)/images/*.gif $(builddir)/images/*.png $(DESTDIR)$(TARGET_DIR)/images
	-@INSTALL@ -m 0644 $(builddir)/api/*.html $(builddir)/api/*.png $(builddir)/api/*.sgml $(DESTDIR)$(TARGET_DIR)/api
	-@INSTALL@ -m 0644 $(builddir)/api/images/*.png $(DESTDIR)$(TARGET_DIR)/api/images

uninstall-local:
	@rm -rf $(DESTDIR)$(TARGET_DIR)

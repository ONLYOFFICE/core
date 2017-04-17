#
#
#
NULL=
MODULE=xmlsec
SOURCE_DIR=$(top_srcdir)/src
SOURCE_DIR2=$(top_builddir)/src
INCLUDE_DIR=$(top_srcdir)/include
INCLUDE_DIR2=$(top_builddir)/include


SCAN_DIR=$(builddir)
SGML_DIR=$(builddir)/sgml.tmp
XML_DIR=$(builddir)/xml.tmp
EXAMPLES_DIR=$(SGML_DIR)/examples
SOURCE_CODE_DIR=$(builddir)/code


# We need to copy some files to make gkdoc happy that
# everything is in one folder
TMPL_DIR=$(builddir)/tmpl
TMPL_SRC_DIR=$(srcdir)/tmpl

MAIN_SGML_FILE=$(SGML_DIR)/xmlsec-main.sgml
MAIN_SGML_SRC_FILE=$(srcdir)/src/xmlsec.sgml

SGML_CHAPTERS_DIR=$(SGML_DIR)/chapters
SGML_CHAPTERS_SRC_DIR=$(srcdir)/src/chapters


EXTRA_DIST = \
	$(TMPL_DIR) \
	$(srcdir)/src \
	$(srcdir)/images \
	$(NULL)

SOURCE_FILES_TO_COPY = \
	$(srcdir)/src \
	$(srcdir)/images \
	$(srcdir)/*.html \
	$(srcdir)/*.png \
	$(NULL)	

#
# We need to pre-process original source files
# because gtkdoc does not understand some C features
#
SOURCE_CODE_FILES=\
	$(shell find $(SOURCE_DIR) -name '*.c' -print )  \
	$(shell find $(SOURCE_DIR2) -name '*.c' -print )  \
	$(shell find $(INCLUDE_DIR) -name '*.h' -a ! -name "symbols.h" -print ) \
	$(shell find $(INCLUDE_DIR2) -name '*.h' -a ! -name "symbols.h" -print )

EXAMPLES_SOURCE_DIR=$(top_srcdir)/examples
EXAMPLES_SOURCE_FILES=\
	$(shell find $(EXAMPLES_SOURCE_DIR) -name '*.c' -print) \
	$(shell find $(EXAMPLES_SOURCE_DIR) -name '*.xml' -print) 

#
# This script removes gtkdoc crap from final html
#
REMOVE_GTKDOCLINK=$(top_srcdir)/scripts/remove-gtkdoclink.pl

# docs is legacy
all: docs


# if build API docs, then we also have xsltproc
if BUILD_API_DOCS
docs: html-cleanup

else
docs:
	@( \
		echo "Copying api-docs..."; \
		if [ z"$(srcdir)" != z"$(builddir)" ]; \
		then \
		    $(CP) -ru $(SOURCE_FILES_TO_COPY) $(builddir)/ ; \
		fi \
	)
endif

html-cleanup: html
	( echo "Cleaning up result files"; \
	   $(PERL) $(REMOVE_GTKDOCLINK) `find . -name "*.html" -print` \
	)

# need to cleanup "bad" chars
html: sgml $(MAIN_SGML_FILE) $(SGML_CHAPTERS_DIR) xmlsec-index
	$(GTKDOC_MKHTML) xmlsec $(MAIN_SGML_FILE)

$(SGML_CHAPTERS_DIR): $(SGML_CHAPTERS_SRC_DIR) $(SGML_CHAPTERS_DIR)/.sentinel
	$(CP) -ru $(SGML_CHAPTERS_SRC_DIR)/* $(SGML_CHAPTERS_DIR)

$(MAIN_SGML_FILE): $(MAIN_SGML_SRC_FILE)
	$(CP) -u $(MAIN_SGML_SRC_FILE) $(MAIN_SGML_FILE)
#
# Prepeare sgml files from sources for each library. We are also
# doing some "magic" here by automatically adding links to XML DSig and 
# XML Enc specification, we also remove "Ptr" from the end of the link
# targets to make more references.
#
# We also fix a bunch of stupid errors from gtkdoc 
#
sgml: sgml-base
	(for i in `find $(SGML_DIR) -name "*.sgml" -print` ; do \
			echo "Fixing up '$$i'" ; \
	        cat $$i | \
	          sed 's!\(<dsig:\)\([^/]*\)\(\/>\)!\&lt;dsig:\2\/\&gt;!g' | \
	          sed 's!\(<enc:\)\([^/]*\)\(\/>\)!\&lt;enc:\2\/\&gt;!g' | \
	          sed 's!\(<dsig:\)\(\w*\)\(>\)!\&lt;dsig:\2\/\&gt;!g' | \
	          sed 's!\(<enc:\)\(\w*\)\(>\)!\&lt;enc:\2\/\&gt;!g' | \
	          sed 's!\(&lt;dsig:\)\([^/]*\)\(\/&gt;\)!<ulink URL=\"http://www.w3.org/TR/xmldsig-core/#sec-\2\">\1\2\3</ulink>!g' | \
	          sed 's!\(&lt;enc:\)\([^/]*\)\(\/&gt;\)!<ulink URL=\"http://www.w3.org/TR/xmlenc-core/#sec-\2\">\1\2\3</ulink>!g' | \
	          sed 's!<para \/>!<para></para>!g' | \
	          sed 's!linkend=\"\(.*\)Ptr\"!linkend=\"\1\"!g' | \
	          sed 's!<colspec [^/]*\/>!!g' | \
	          sed 's!<programlisting language="C"!<programlisting!g' | \
	          $(PERL) -pe 's!title>\n!title>!g' | \
	          $(PERL) -pe 's!title>\n!title>!g' | \
	          $(PERL) -pe 's!<\/title><\/refsect1>\n!<\/title><para><\/para><\/refsect1>!g' | \
	          $(PERL) -pe 's!para>\n!para>!g' > \
	          $$i.tmp; \
	        mv -f $$i.tmp $$i; \
	 done);

sgml-base: templates $(SGML_DIR)/.sentinel
	$(GTKDOC_MKDB) --module=xmlsec \
			--main-sgml-file=$(SGML_DIR)/xmlsec-base.sgml \
			--output-dir=$(SGML_DIR)/ \
		    --tmpl-dir=$(TMPL_DIR)/base \
		    --source-dir=$(SOURCE_CODE_DIR)/src/base \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/base
	$(GTKDOC_MKDB) --module=xmlsec-openssl \
		    --main-sgml-file=$(SGML_DIR)/xmlsec-openssl.sgml \
		    --tmpl-dir=$(TMPL_DIR)/openssl \
		    --output-dir=$(SGML_DIR)/openssl \
		    --source-dir=$(SOURCE_CODE_DIR)/src/openssl \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/openssl
	$(GTKDOC_MKDB) --module=xmlsec-gnutls \
		    --main-sgml-file=$(SGML_DIR)/xmlsec-gnutls.sgml \
		    --tmpl-dir=$(TMPL_DIR)/gnutls \
		    --output-dir=$(SGML_DIR)/gnutls \
		    --source-dir=$(SOURCE_CODE_DIR)/src/gnutls \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/gnutls
	$(GTKDOC_MKDB) --module=xmlsec-gcrypt \
		    --main-sgml-file=$(SGML_DIR)/xmlsec-gcrypt.sgml \
		    --tmpl-dir=$(TMPL_DIR)/gcrypt \
		    --output-dir=$(SGML_DIR)/gcrypt \
		    --source-dir=$(SOURCE_CODE_DIR)/src/gcrypt \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/gcrypt
	$(GTKDOC_MKDB) --module=xmlsec-nss \
		    --main-sgml-file=$(SGML_DIR)/xmlsec-nss.sgml \
		    --tmpl-dir=$(TMPL_DIR)/nss \
		    --output-dir=$(SGML_DIR)/nss \
		    --source-dir=$(SOURCE_CODE_DIR)/src/nss \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/nss
	$(GTKDOC_MKDB) --module=xmlsec-mscrypto \
		    --main-sgml-file=$(SGML_DIR)/xmlsec-mscrypto.sgml \
		    --tmpl-dir=$(TMPL_DIR)/mscrypto \
		    --output-dir=$(SGML_DIR)/mscrypto \
		    --source-dir=$(SOURCE_CODE_DIR)/src/mscrypto \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/mscrypto

templates: scan templates-copy
	$(GTKDOC_MKTMPL) --module=xmlsec \
			--output-dir=$(TMPL_DIR)/base
	$(GTKDOC_MKTMPL) --module=xmlsec-openssl \
		    --output-dir=$(TMPL_DIR)/openssl
	$(GTKDOC_MKTMPL) --module=xmlsec-gnutls \
		    --output-dir=$(TMPL_DIR)/gnutls
	$(GTKDOC_MKTMPL) --module=xmlsec-gcrypt \
		    --output-dir=$(TMPL_DIR)/gcrypt
	$(GTKDOC_MKTMPL) --module=xmlsec-nss \
		    --output-dir=$(TMPL_DIR)/nss
	$(GTKDOC_MKTMPL) --module=xmlsec-mscrypto \
		    --output-dir=$(TMPL_DIR)/mscrypto

# make sure to run chmod since we will update templates
templates-copy: $(TMPL_SRC_DIR) $(TMPL_DIR)/.sentinel
	@echo "Copying original template files into '$(TMPL_DIR)' ..."
	( if [ z"$(TMPL_DIR)" != z"$(TMPL_SRC_DIR)" ] ; then \
		$(CP) -ru $(TMPL_SRC_DIR)/* $(TMPL_DIR)/ ; \
	  fi ; )
	chmod -R u+w $(TMPL_DIR)

scan: SOURCE_CODEs example_sources
	$(GTKDOC_SCAN) --module=xmlsec \
		    --rebuild-sections \
		    --output-dir=$(SCAN_DIR) \
		    --source-dir=$(SOURCE_CODE_DIR)/src/base \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/base
	$(GTKDOC_SCAN) --module=xmlsec-openssl \
		    --rebuild-sections \
		    --output-dir=$(SCAN_DIR) \
		    --source-dir=$(SOURCE_CODE_DIR)/src/openssl \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/openssl
	$(GTKDOC_SCAN) --module=xmlsec-gnutls \
		    --rebuild-sections \
		    --output-dir=$(SCAN_DIR) \
		    --source-dir=$(SOURCE_CODE_DIR)/src/gnutls \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/gnutls
	$(GTKDOC_SCAN) --module=xmlsec-gcrypt \
		    --rebuild-sections \
		    --output-dir=$(SCAN_DIR) \
		    --source-dir=$(SOURCE_CODE_DIR)/src/gcrypt \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/gcrypt
	$(GTKDOC_SCAN) --module=xmlsec-nss \
		    --rebuild-sections \
		    --output-dir=$(SCAN_DIR) \
		    --source-dir=$(SOURCE_CODE_DIR)/src/nss \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/nss
	$(GTKDOC_SCAN) --module=xmlsec-mscrypto \
		    --rebuild-sections \
		    --output-dir=$(SCAN_DIR) \
		    --source-dir=$(SOURCE_CODE_DIR)/src/mscrypto \
		    --source-dir=$(SOURCE_CODE_DIR)/include/xmlsec/mscrypto

#
# Prepare source files by coping them to "code" folder and 
# removing XMLSEC_EXPORT_* stuff that makes gtkdoc crazy
#
SOURCE_CODEs: $(SOURCE_CODE_FILES) $(SOURCE_CODE_DIR)/.sentinel SOURCE_CODEs_cleanup
	@echo "Preprocessing source files into '$(SOURCE_CODE_DIR)' ..." 
	@mkdir -p $(SOURCE_CODE_DIR)/src/base $(SOURCE_CODE_DIR)/include/xmlsec/base
	@( \
	for i in $(SOURCE_CODE_FILES) ; do \
		folder_name=`echo "$$i" | \
			sed 's#$(top_srcdir)/##' | \
			sed 's#$(top_builddir)/##' | \
			sed 's#/[^/]*$$##'`; \
	    file_name=`echo "$$i" | \
	    	sed 's#.*/##'`; \
	    mkdir -p "$(SOURCE_CODE_DIR)/$$folder_name"; \
	    cat "$$i" | \
		    sed 's/#if.*//' | \
		    sed 's/#el.*//' | \
		    sed 's/#end.*//' | \
		    sed 's/XMLSEC_CRYPTO_EXPORT//' | \
		    sed 's/XMLSEC_EXPORT_VAR//' | \
		    sed 's/XMLSEC_EXPORT//' | \
		    sed 's/XMLSEC_ERRORS_PRINTF_ATTRIBUTE//' > \
		    $(SOURCE_CODE_DIR)/$$folder_name/$$file_name; \
	done);
	-@mv -f $(SOURCE_CODE_DIR)/src/*.c $(SOURCE_CODE_DIR)/src/base
	-@mv -f $(SOURCE_CODE_DIR)/include/xmlsec/*.h $(SOURCE_CODE_DIR)/include/xmlsec/base
	-@rm -f $(SOURCE_CODE_DIR)/include/xmlsec/*/symbols.h

SOURCE_CODEs_cleanup: $(SOURCE_CODE_DIR)/.sentinel
	@rm -rf $(SOURCE_CODE_DIR)/*

#
# Create index for all functions. For macros and defines need to add -CAPS suffix
#
xmlsec-index: scan $(SGML_DIR)/.sentinel
	@grep -h '<NAME>.*</NAME>' $(SCAN_DIR)/xmlsec-*decl.txt | \
		grep -v '<NAME>extern</NAME>' | \
		sort -u | \
		sed 's#_#-#g' | \
		sed 's#<NAME>\([^-]*\)-\([^<]*\)</NAME>#<listitem><para><link linkend=\"\1-\2-CAPS\">\1-\2</link></para></listitem>#g' | \
		sed 's#<NAME>\([^<]*\)</NAME>#<listitem><para><link linkend=\"\1\">\1</link></para></listitem>#g' > \
		$(SGML_DIR)/xmlsec-index.sgml

#
# The following code converts C example file to sgml RefEntry files.
# We get file title from a string "XML Security Library example: ..."
# which is usually placed at the top of the file. Also all "unsafe" xml 
# characters (<, >, &) are escaped. 
#
example_sources: $(EXAMPLES_DIR)/.sentinel
	@echo "Preprocessing example source files into '$(EXAMPLES_DIR)' ..." 
	@rm -rf $(EXAMPLES_DIR)/*
	@(for i in $(EXAMPLES_SOURCE_FILES) ; do \
	    file_name=`echo $$i | sed 's#.*/##' | sed 's#\..*$$##'`; \
	    file_ext=`echo $$i | sed 's#.*/##' | sed 's#.*\.##'`; \
	    echo Converting $$file_name.$$file_ext to $$file_name.sgml ...; \
	    file_title=`cat $$i | grep 'XML Security Library example: ' | sed 's#^.*: *##'`; \
	    echo "<sect2 id=\"xmlsec-example-$$file_name\" >" > \
	        $(EXAMPLES_DIR)/$$file_name.sgml; \
	    echo "<title>$$file_name.$$file_ext</title><para><informalexample><programlisting>" >> \
		    $(EXAMPLES_DIR)/$$file_name.sgml; \
	    cat $$i | \
		sed "s#&#\&amp;#g" | \
		sed "s#<#\&lt;#g" | \
		sed "s#>#\&gt;#g" >> \
		    $(EXAMPLES_DIR)/$$file_name.sgml; \
	    echo "</programlisting></informalexample></para></sect2>" >> \
		    $(EXAMPLES_DIR)/$$file_name.sgml; \
	done);

# A single pattern rule will create all appropriate folders as required
# otherwise make (annoyingly) deletes it
.PRECIOUS: %/.sentinel
%/.sentinel:
	@echo "Creating folder '${@D}' ..."
	mkdir -p ${@D}
	touch $@

dist-hook:
	@cp -p $(srcdir)/*.html $(srcdir)/*.png $(distdir)/
	(for i in `find $(distdir) -name ".sentinel" -print` ; do \
		echo "Removing some files '$$i' before dist ... " ; \
		rm $$i ; \
	done )
	(for i in `find $(distdir) -name "*.sgml.bak" -print` ; do \
		echo "Removing some files '$$i' before dist ... " ; \
		rm "$$i" ; \
	done ; )
	
clean-local:
	-rm -rf $(SOURCE_CODE_DIR) $(EXAMPLES_DIR) $(SCAN_DIR)/*.txt $(SGML_DIR) $(XML_DIR)
	-rm -f *.stamp *.types *.css index.sgml
	( if [ z"$(TMPL_SRC_DIR)" != z"$(TMPL_DIR)" ] && [ -d "$(TMPL_DIR)" ] ; then \
		chmod -R u+w $(TMPL_DIR) && rm -rf $(TMPL_DIR) ; \
	 fi ; )
	 ( if [ z"$(builddir)" != z"$(srcdir)" ] ; then \
    	chmod -R u+w $(builddir)/src && rm -rf $(builddir)/src ; \
    	chmod -R u+w $(builddir)/images && rm -rf $(builddir)/images ; \
    	chmod -R u+w $(builddir)/*.png && rm -rf $(builddir)/*.png ; \
	 fi ; )

distclean-local: clean-local

maintainer-clean-local: clean-local
	-rm -f *.html


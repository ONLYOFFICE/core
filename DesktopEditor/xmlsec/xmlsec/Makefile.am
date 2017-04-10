NULL =

SAFE_VERSION	= @XMLSEC_VERSION_SAFE@
SUBDIRS 	    = include src apps man docs
TEST_APP 	    = apps/xmlsec1$(EXEEXT)
DEFAULT_CRYPTO	= @XMLSEC_DEFAULT_CRYPTO@

bin_SCRIPTS 	= xmlsec1-config
pkgconfig_DATA 	= xmlsec1.pc @XMLSEC_CRYPTO_PC_FILES_LIST@
pkgconfigdir 	= $(libdir)/pkgconfig
confexecdir	= $(libdir)
confexec_DATA 	= xmlsec1Conf.sh
m4datadir	= $(datadir)/aclocal
m4data_DATA	= xmlsec1.m4

ACLOCAL_AMFLAGS	= -I m4

DISTCLEANFILES = \
	xmlsec1Conf.sh \
	xmlsec1.pc \
	xmlsec1-openssl.pc \
	xmlsec1-nss.pc \
	xmlsec1-gnutls.pc \
	xmlsec1-gcrypt.pc \
	xmlsec1-config \
	xmlsec1.spec \
	stamp-h2 \
	stamp-h3 \
	stamp-h4 \
	stamp-h5 \
	$NULL

EXTRA_DIST = \
	m4 \
	examples \
	scripts \
	tests \
	win32 \
	NEWS \
	ChangeLog \
	Copyright \
	HACKING \
	xmlsec-openssl.pc.in \
	xmlsec-nss.pc.in\
	xmlsec-gnutls.pc.in \
	xmlsec-gcrypt.pc.in \
	xmlsec-config.in \
	xmlsecConf.sh.in \
	xmlsec.spec.in \
	xmlsec1Conf.sh \
	xmlsec1.pc @XMLSEC_CRYPTO_PC_FILES_LIST@ \
	xmlsec1-config \
	xmlsec1.spec \
	xmlsec1.m4 \
	$(NULL)

EXTRA_CLEAN = \
	examples \
	$(NULL)

ABS_SRCDIR=@abs_srcdir@
ABS_BUILDDIR=@abs_builddir@
if XMLSEC_NO_APPS_CRYPTO_DYNAMIC_LOADING
PRECHECK_COMMANDS = \
	cd $(ABS_SRCDIR) \
	$(NULL)
else
PRECHECK_COMMANDS= \
	for i in $(XMLSEC_CRYPTO_LIST) ; do \
	    export LD_LIBRARY_PATH="$(ABS_BUILDDIR)/src/$$i/.libs:$$LD_LIBRARY_PATH" ; \
	done && \
	cd $(ABS_SRCDIR) \
	$(NULL)	
endif
CHECK_CRYPTO_LIST = \
	$(XMLSEC_CRYPTO_LIST) \
	$(NULL)

docs: docs-man
	@(cd docs && $(MAKE) docs)

docs-clean: 
	@(cd docs && $(MAKE) clean)

docs-man: 
	@(cd man && $(MAKE) docs)

check: check-all check-info

check-all: $(TEST_APP) 
	for crypto in $(CHECK_CRYPTO_LIST) ; do \
		make check-crypto-$$crypto ; \
	done

check-crypto-%: $(TEST_APP) 
	@($(PRECHECK_COMMANDS) && \
    echo "=================== Checking xmlsec-$* =================================" && \
    $(SHELL) ./tests/testrun.sh \
        $(ABS_SRCDIR)/tests/testKeys.sh \
        $* \
        $(ABS_SRCDIR)/tests \
        $(ABS_BUILDDIR)/$(TEST_APP) der \
    && \
    $(SHELL) ./tests/testrun.sh \
        $(ABS_SRCDIR)/tests/testDSig.sh \
        $* \
        $(ABS_SRCDIR)/tests \
        $(ABS_BUILDDIR)/$(TEST_APP) \
        der \
    && \
    $(SHELL) ./tests/testrun.sh \
        $(ABS_SRCDIR)/tests/testEnc.sh \
        $* \
        $(ABS_SRCDIR)/tests \
        $(ABS_BUILDDIR)/$(TEST_APP) \
        der \
    ; \
	)
	
check-info:
	@echo "---------------------------- ATTENTION -----------------------------------"
	@echo "--- Some of the tests use resources located on external HTTP servers.  ---"
	@echo "--- If you have no Internet connection or the external resource is not ---"
	@echo "--- responding then the test will fail.                                ---"
	@echo "---------------------------- ATTENTION -----------------------------------"

check-keys: $(TEST_APP)
	@($(PRECHECK_COMMANDS) && \
	$(SHELL) ./tests/testrun.sh \
	    $(ABS_SRCDIR)/tests/testKeys.sh \
	    $(DEFAULT_CRYPTO) \
	    $(ABS_SRCDIR)/tests \
	    $(ABS_BUILDDIR)/$(TEST_APP) \
	    der \
	)

check-dsig: $(TEST_APP) 	
	@($(PRECHECK_COMMANDS) && \
	$(SHELL) ./tests/testrun.sh \
	    $(ABS_SRCDIR)/tests/testDSig.sh \
	    $(DEFAULT_CRYPTO) \
	    $(ABS_SRCDIR)/tests \
	    $(ABS_BUILDDIR)/$(TEST_APP) \
	    der \
	)

check-enc: $(TEST_APP) 
	@($(PRECHECK_COMMANDS) && \
	$(SHELL) ./tests/testrun.sh \
	    $(ABS_SRCDIR)/tests/testEnc.sh  \
	    $(DEFAULT_CRYPTO) \
	    $(ABS_SRCDIR)/tests \
	    $(ABS_BUILDDIR)/$(TEST_APP) \
	    der \
	)

memcheck-res:
	@grep -i 'ERROR SUMMARY' /tmp/*.log | sed 's/.*==.*== *//' | sort -u
	@grep -i 'in use at exit' /tmp/*.log | sed 's/.*==.*== *//' | sort -u
	@grep -i 'definitely lost:' /tmp/*.log | sed 's/.*==.*== *//' | sort -u
	@grep -i 'indirectly lost:' /tmp/*.log | sed 's/.*==.*== *//' | sort -u
	@grep -i 'possibly lost:' /tmp/*.log | sed 's/.*==.*== *//' | sort -u
	@grep -i 'still reachable:' /tmp/*.log | sed 's/.*==.*== *//' | sort -u

memcheck: $(TEST_APP)
	@(export DEBUG_MEMORY=1 && $(MAKE) check && $(MAKE) memcheck-res)

perfcheck: $(TEST_APP)
	@(export PERF_TEST=10 && $(MAKE) check)

dist-hook:

cleantar:
	@($(RM) -f xmlsec*.tar.gz COPYING.LIB)

tar-release: clean cleantar
	@(unset CDPATH && $(MAKE) dist)

rpm: cleantar tar-release
	@(unset CDPATH && rpmbuild -ta $(distdir).tar.gz)

rpm-release: clean cleantar rpm
	

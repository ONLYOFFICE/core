PRODUCT_NAME ?= core
PRODUCT_VERSION ?= 0.0.0
BUILD_NUMBER ?= 0
PACKAGE_NAME := $(PRODUCT_NAME)

PUBLISHER_NAME ?= Ascensio System SIA

UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),x86_64)
	ARCHITECTURE := 64
	ARCH_SUFFIX := x64
endif
ifneq ($(filter %86,$(UNAME_M)),)
	ARCHITECTURE := 32
	ARCH_SUFFIX := x86
endif

ifeq ($(OS),Windows_NT)
	PLATFORM := win
	EXEC_EXT := .exe
	SHELL_EXT := .bat
	SHARED_EXT := .dll
	LIB_EXT := .lib
	ARCH_EXT := .zip
	MAKE := nmake
	AR := 7z a -y
	PACKAGE_VERSION := $(PRODUCT_VERSION).$(BUILD_NUMBER)
	ARCH_REPO_DIR := windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		PLATFORM := linux
		SHARED_EXT := .so*
		SHELL_EXT := .sh
		LIB_EXT := .a
		LIB_PREFIX := lib
		ARCH_EXT := .tar.gz
		MAKE := make
		AR := tar -zcvf
		PACKAGE_VERSION := $(PRODUCT_VERSION)-$(BUILD_NUMBER)
		ARCH_REPO_DIR := linux
	endif
	ifeq ($(UNAME_S),Darwin)
		PLATFORM := mac
		SHARED_EXT := .dylib
		SHELL_EXT := .sh
		LIB_EXT := .a
		LIB_PREFIX := lib
		ARCH_EXT := .tar.gz
		MAKE := make
		AR := tar -zcvf
		PACKAGE_VERSION := $(PRODUCT_VERSION)-$(BUILD_NUMBER)
		ARCH_REPO_DIR := mac
	endif
endif

TARGET := $(PLATFORM)_$(ARCHITECTURE)

LIBDIR := build/lib/$(TARGET)
BINDIR := build/bin/$(TARGET)

ALLFONTSGEN := build/bin/$(TARGET)/allfontsgen$(EXEC_EXT)
ALLTHEMESGEN := build/bin/$(TARGET)/allthemesgen$(EXEC_EXT)
X2T := $(BINDIR)/x2t$(EXEC_EXT)
DOCBUILDER := $(BINDIR)/docbuilder$(EXEC_EXT)
HTMLFILEINTERNAL := $(LIBDIR)/HtmlFileInternal$(EXEC_EXT)
PDFWRITER := $(LIBDIR)/$(LIB_PREFIX)PdfWriter$(SHARED_EXT)
GRAPHICS := $(LIBDIR)/$(LIB_PREFIX)graphics$(SHARED_EXT)
DOCTRENDERER := $(LIBDIR)/$(LIB_PREFIX)doctrenderer$(SHARED_EXT)
HTMLRENDERER := $(LIBDIR)/$(LIB_PREFIX)HtmlRenderer$(SHARED_EXT)
PDFREADER := $(LIBDIR)/$(LIB_PREFIX)PdfReader$(SHARED_EXT)
DJVUFILE := $(LIBDIR)/$(LIB_PREFIX)DjVuFile$(SHARED_EXT)
XPSFILE := $(LIBDIR)/$(LIB_PREFIX)XpsFile$(SHARED_EXT)
HTMLFILE := $(LIBDIR)/$(LIB_PREFIX)HtmlFile$(SHARED_EXT)
UNICODECONVERTER := $(LIBDIR)/$(LIB_PREFIX)UnicodeConverter$(SHARED_EXT)
ASCDOCUMENTSCORE := $(LIBDIR)/$(LIB_PREFIX)ascdocumentscore$(SHARED_EXT)
LICENSEMANAGER := $(LIBDIR)/$(LIB_PREFIX)LicenceManager$(LIB_EXT)
OOXMLSIGNATURE := $(LIBDIR)/$(LIB_PREFIX)ooxmlsignature$(LIB_EXT)
HUNSPELL := $(LIBDIR)/$(LIB_PREFIX)hunspell$(SHARED_EXT)
KERNEL := $(LIBDIR)/$(LIB_PREFIX)kernel$(SHARED_EXT)
CRYPTOPP := $(LIBDIR)/$(LIB_PREFIX)CryptoPPLib$(LIB_EXT)
VIDEOPLAYERLIB := $(LIBDIR)/$(LIB_PREFIX)videoplayer$(SHARED_EXT)

TARGETS += $(ALLFONTSGEN)
TARGETS += $(ALLTHEMESGEN)
TARGETS += $(X2T)
TARGETS += $(DOCBUILDER)
TARGETS += $(HTMLFILEINTERNAL)
TARGETS += $(PDFWRITER)
TARGETS += $(GRAPHICS)
TARGETS += $(DOCTRENDERER)
TARGETS += $(HTMLRENDERER)
TARGETS += $(PDFREADER)
TARGETS += $(DJVUFILE)
TARGETS += $(XPSFILE)
TARGETS += $(HTMLFILE)
TARGETS += $(UNICODECONVERTER)
TARGETS += $(ASCDOCUMENTSCORE)
TARGETS += $(LICENSEMANAGER)
TARGETS += $(OOXMLSIGNATURE)
TARGETS += $(HUNSPELL)
TARGETS += $(KERNEL)
TARGETS += $(CRYPTOPP)
TARGETS += $(VIDEOPLAYERLIB)

X2T_PRO := $(abspath X2tConverter/build/Qt/X2tSLN.pro)
HTMLFILEINTERNAL_PRO := $(abspath ../desktop-sdk/HtmlFile/Internal/Internal.pro)
ALLFONTSGEN_PRO := $(abspath DesktopEditor/AllFontsGen/AllFontsGen.pro)
ALLTHEMESGEN_PRO := $(abspath DesktopEditor/allthemesgen/allthemesgen.pro)
DOCBUILDER_PRO := $(abspath DesktopEditor/doctrenderer/app_builder/docbuilder.pro)
PDFWRITER_PRO := $(abspath PdfWriter/PdfWriter.pro)
GRAPHICS_PRO := $(abspath DesktopEditor/graphics/pro/graphics.pro)
DOCTRENDERER_PRO := $(abspath DesktopEditor/doctrenderer/doctrenderer.pro)
HTMLRENDERER_PRO := $(abspath HtmlRenderer/htmlrenderer.pro)
PDFREADER_PRO := $(abspath PdfReader/PdfReader.pro)
DJVUFILE_PRO := $(abspath DjVuFile/DjVuFile.pro)
XPSFILE_PRO := $(abspath XpsFile/XpsFile.pro)
HTMLFILE_PRO := $(abspath HtmlFile/HtmlFile.pro)
UNICODECONVERTER_PRO := $(abspath UnicodeConverter/UnicodeConverter.pro)
ASCDOCUMENTSCORE_PRO := $(abspath ../desktop-sdk/ChromiumBasedEditors/lib/AscDocumentsCore_$(PLATFORM).pro)
LICENSEMANAGER_PRO := $(abspath LicenceManager/linux/LicenseManager.pro)
OOXMLSIGNATURE_PRO := $(abspath DesktopEditor/xmlsec/src/ooxmlsignature.pro)
HUNSPELL_PRO := $(abspath DesktopEditor/hunspell-1.3.3/src/qt/hunspell.pro)
KERNEL_PRO := $(abspath Common/kernel.pro)
CRYPTOPP_PRO := $(abspath Common/3dParty/cryptopp/project/cryptopp.pro)
#VIDEOPLAYERLIB_PRO := $(abspath ../core-ext/multimedia/videoplayer/lib/VideoPlayerLib.pro)
VIDEOPLAYERLIB_PRO := $(abspath ../desktop-sdk/ChromiumBasedEditors/videoplayerlib/videoplayerlib.pro)

# PROS += $(basename $(X2T_PRO)).build
# PROS += ALLFONTSGEN_PRO
# PROS += PDFWRITER_PRO
# PROS += GRAPHICS_PRO
# PROS += DOCTRENDERER_PRO
# PROS += HTMLRENDERER_PRO
# PROS += PDFREADER_PRO
# PROS += DJVUFILE_PRO
# PROS += XPSFILE_PRO
# PROS += HTMLFILE_PRO
# PROS += UNICODECONVERTER_PRO

QT_PROJ += X2T
QT_PROJ += HTMLFILEINTERNAL
QT_PROJ += ALLFONTSGEN
QT_PROJ += ALLTHEMESGEN
QT_PROJ += DOCBUILDER
QT_PROJ += PDFWRITER
QT_PROJ += GRAPHICS
QT_PROJ += DOCTRENDERER
QT_PROJ += HTMLRENDERER
QT_PROJ += PDFREADER
QT_PROJ += DJVUFILE
QT_PROJ += XPSFILE
QT_PROJ += HTMLFILE
QT_PROJ += UNICODECONVERTER
QT_PROJ += ASCDOCUMENTSCORE
QT_PROJ += LICENSEMANAGER
QT_PROJ += OOXMLSIGNATURE
QT_PROJ += HUNSPELL
QT_PROJ += KERNEL
QT_PROJ += CRYPTOPP
QT_PROJ += VIDEOPLAYERLIB

X2T_DEP += $(PDFWRITER)
X2T_DEP += $(GRAPHICS)
X2T_DEP += $(DOCTRENDERER)
X2T_DEP += $(HTMLRENDERER)
X2T_DEP += $(PDFREADER)
X2T_DEP += $(DJVUFILE)
X2T_DEP += $(XPSFILE)
X2T_DEP += $(HTMLFILE)
X2T_DEP += $(UNICODECONVERTER)
X2T_DEP += $(KERNEL)
X2T_DEP += $(CRYPTOPP)

HTMLFILEINTERNAL_DEP += $(GRAPHICS)
HTMLFILEINTERNAL_DEP += $(UNICODECONVERTER)

XPSFILE_DEP += $(PDFWRITER)
XPSFILE_DEP += $(UNICODECONVERTER)
XPSFILE_DEP += $(KERNEL)
XPSFILE_DEP += $(GRAPHICS)

ALLFONTSGEN_DEP += $(GRAPHICS)
ALLFONTSGEN_DEP += $(UNICODECONVERTER)

ALLTHEMESGEN_DEP += $(DOCTRENDERER)
ALLTHEMESGEN_DEP += $(GRAPHICS)
ALLTHEMESGEN_DEP += $(KERNEL)
ALLTHEMESGEN_DEP += $(UNICODECONVERTER)

DOCTRENDERER_DEP += $(KERNEL)
DOCTRENDERER_DEP += $(GRAPHICS)
DOCTRENDERER_DEP += $(UNICODECONVERTER)

DOCBUILDER_DEP += $(DOCTRENDERER)

GRAPHICS_DEP += $(UNICODECONVERTER)
GRAPHICS_DEP += $(KERNEL)

HTMLFILE_DEP += $(UNICODECONVERTER)
HTMLFILE_DEP += $(KERNEL)

HTMLRENDERER_DEP += $(UNICODECONVERTER)
HTMLRENDERER_DEP += $(KERNEL)
HTMLRENDERER_DEP += $(GRAPHICS)

DJVUFILE_DEP += $(UNICODECONVERTER)
DJVUFILE_DEP += $(KERNEL)
DJVUFILE_DEP += $(GRAPHICS)
DJVUFILE_DEP += $(PDFWRITER)

ASCDOCUMENTSCORE_DEP += $(GRAPHICS)
ASCDOCUMENTSCORE_DEP += $(PDFWRITER)
ASCDOCUMENTSCORE_DEP += $(PDFREADER)
ASCDOCUMENTSCORE_DEP += $(DJVUFILE)
ASCDOCUMENTSCORE_DEP += $(XPSFILE)
#ASCDOCUMENTSCORE_DEP += $(LICENSEMANAGER)
ASCDOCUMENTSCORE_DEP += $(OOXMLSIGNATURE)
ASCDOCUMENTSCORE_DEP += $(HUNSPELL)

PDFREADER_DEP += $(HTMLRENDERER)
PDFREADER_DEP += $(UNICODECONVERTER)
PDFREADER_DEP += $(KERNEL)
PDFREADER_DEP += $(GRAPHICS)
PDFREADER_DEP += $(CRYPTOPP)

PDFWRITER_DEP += $(UNICODECONVERTER)
PDFWRITER_DEP += $(KERNEL)
PDFWRITER_DEP += $(GRAPHICS)
PDFWRITER_DEP += $(CRYPTOPP)

VIDEOPLAYERLIB_DEP += $(KERNEL)

ARCHIVE := ../build_tools/out/$(TARGET)/onlyoffice/core/core.7z

ARTIFACTS += build/*
ARTIFACTS += Common/3dParty/*/$(TARGET)/build/*



ifeq ($(OS),Windows_NT)
ARTIFACTS += Common/3dParty/v8/v8/out.gn/$(TARGET)/release/*.dat
else
ARTIFACTS += Common/3dParty/v8/v8/out.gn/$(TARGET)/*.dat
endif

CORE_TARGET += $(ALLFONTSGEN)
CORE_TARGET += $(ALLTHEMESGEN)
CORE_TARGET += $(DOCTRENDERER)
CORE_TARGET += $(DJVUFILE)
CORE_TARGET += $(HTMLFILE)
CORE_TARGET += $(HTMLRENDERER)
CORE_TARGET += $(PDFREADER)
CORE_TARGET += $(PDFWRITER)
CORE_TARGET += $(UNICODECONVERTER)
CORE_TARGET += $(X2T)
CORE_TARGET += $(XPSFILE)

EXT_TARGET += $(DOCBUILDER)

ifneq ($(PLATFORM),mac)
EXT_TARGET += $(HTMLFILEINTERNAL)
endif

#Template for next statment:
#FOO_MAKE := $(basename $(FOO_PRO)).build/Makefile
#$(FOO): $(FOO_MAKE)
#  cd $(dir $(FOO_MAKE)) && make

define build_proj_tmpl
PROS += $$(basename $$(value $(1)_PRO)).build
$(1)_MAKE := $$(basename $$(value $(1)_PRO)).build/Makefile
$$(value $(1)): $$(value $(1)_MAKE)
	cd $$(dir $$(value $(1)_MAKE)) && $(MAKE);
endef

.PHONY : all clean deploy

.NOTPARALLEL:
all: $(CORE_TARGET)

ext: $(EXT_TARGET)

desktop: $(ASCDOCUMENTSCORE) $(VIDEOPLAYERLIB)

video: $(VIDEOPLAYERLIB) 

$(foreach proj, $(QT_PROJ), $(eval $(call build_proj_tmpl, $(proj))))

$(X2T): $(X2T_DEP)

$(HTMLFILEINTERNAL): $(HTMLFILEINTERNAL_DEP)

$(XPSFILE): $(XPSFILE_DEP)

$(ALLFONTSGEN): $(ALLFONTSGEN_DEP)

$(ALLTHEMESGEN): $(ALLTHEMESGEN_DEP)

$(DOCBUILDER): $(DOCBUILDER_DEP)

$(DOCTRENDERER): $(DOCTRENDERER_DEP)

$(GRAPHICS): $(GRAPHICS_DEP)

$(HTMLFILE): $(HTMLFILE_DEP)

$(HTMLRENDERER): $(HTMLRENDERER_DEP)

$(DJVUFILE): $(DJVUFILE_DEP)

$(ASCDOCUMENTSCORE): $(ASCDOCUMENTSCORE_DEP)

$(PDFREADER): $(PDFREADER_DEP)

$(PDFWRITER): $(PDFWRITER_DEP)

$(VIDEOPLAYERLIB): $(VIDEOPLAYERLIB_DEP)

%.build/Makefile: %.pro
	mkdir -p $(dir $@) && \
	cd $(dir $@) && \
	PUBLISHER_NAME="$(PUBLISHER_NAME)" \
	PRODUCT_VERSION=$(PRODUCT_VERSION) \
	BUILD_NUMBER=$(BUILD_NUMBER) \
	qmake -r $<

$(ARCHIVE) : 
	$(AR) $@ $(ARTIFACTS)

clean:
	rm -rf $(TARGETS) $(ARCHIVE)
	for i in $(PROS); do \
		if [ -d $$i -a -f $$i/Makefile ]; then \
			cd $$i && $(MAKE) distclean; \
		fi \
done

deploy: $(ARCHIVE)
	aws s3 cp \
		$(ARCHIVE) \
		s3://repo-doc-onlyoffice-com/$(ARCH_REPO_DIR)/$(PACKAGE_NAME)/$(GIT_BRANCH)/$(PACKAGE_VERSION)/$(ARCH_SUFFIX)/ \
		--acl public-read

	aws s3 sync \
		s3://repo-doc-onlyoffice-com/$(ARCH_REPO_DIR)/$(PACKAGE_NAME)/$(GIT_BRANCH)/$(PACKAGE_VERSION)/$(ARCH_SUFFIX)/  \
		s3://repo-doc-onlyoffice-com/$(ARCH_REPO_DIR)/$(PACKAGE_NAME)/$(GIT_BRANCH)/latest/$(ARCH_SUFFIX)/ \
		--acl public-read --delete

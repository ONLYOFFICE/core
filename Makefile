PRODUCT_NAME ?= core
PRODUCT_VERSION ?= 0.0.0
BUILD_NUMBER ?= 0
PACKAGE_NAME := $(PRODUCT_NAME)

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
		MAKE := make -j $(shell grep -c ^processor /proc/cpuinfo)
		AR := tar -zcvf
		PACKAGE_VERSION := $(PRODUCT_VERSION)-$(BUILD_NUMBER)
		ARCH_REPO_DIR := linux
	endif
endif

TARGET := $(PLATFORM)_$(ARCHITECTURE)

LIBDIR := build/lib/$(TARGET)
BINDIR := build/bin/$(TARGET)

ALLFONTSGEN := build/bin/AllFontsGen/$(TARGET)$(EXEC_EXT)
X2T := $(BINDIR)/x2t$(EXEC_EXT)
DOCBUILDER := $(BINDIR)/docbuilder$(EXEC_EXT)
HTMLFILEINTERNAL := $(LIBDIR)/HtmlFileInternal$(EXEC_EXT)
XLSFORMATLIB := $(LIBDIR)/$(LIB_PREFIX)XlsFormatLib$(LIB_EXT)
ODFFILEWRITERLIB := $(LIBDIR)/$(LIB_PREFIX)OdfFileWriterLib$(LIB_EXT)
ODFFILEREADERLIB := $(LIBDIR)/$(LIB_PREFIX)OdfFileReaderLib$(LIB_EXT)
DOCFORMATLIB := $(LIBDIR)/$(LIB_PREFIX)DocFormatLib$(LIB_EXT)
PPTFORMATLIB := $(LIBDIR)/$(LIB_PREFIX)PptFormatLib$(LIB_EXT)
RTFFORMATLIB := $(LIBDIR)/$(LIB_PREFIX)RtfFormatLib$(LIB_EXT)
TXTXMLFORMATLIB := $(LIBDIR)/$(LIB_PREFIX)TxtXmlFormatLib$(LIB_EXT)
PDFWRITER := $(LIBDIR)/$(LIB_PREFIX)PdfWriter$(SHARED_EXT)
ASCOFFICEDOCXFILE2LIB := $(LIBDIR)/$(LIB_PREFIX)ASCOfficeDocxFile2Lib$(LIB_EXT)
PPTXFORMATLIB := $(LIBDIR)/$(LIB_PREFIX)PPTXFormatLib$(LIB_EXT)
DOCXFORMATLIB := $(LIBDIR)/$(LIB_PREFIX)DocxFormatLib$(LIB_EXT)
OFFICEUTILS := $(LIBDIR)/$(LIB_PREFIX)OfficeUtils$(LIB_EXT)
GRAPHICS := $(LIBDIR)/$(LIB_PREFIX)graphics$(LIB_EXT)
DOCTRENDERER := $(LIBDIR)/$(LIB_PREFIX)doctrenderer$(SHARED_EXT)
HTMLRENDERER := $(LIBDIR)/$(LIB_PREFIX)HtmlRenderer$(SHARED_EXT)
PDFREADER := $(LIBDIR)/$(LIB_PREFIX)PdfReader$(SHARED_EXT)
DJVUFILE := $(LIBDIR)/$(LIB_PREFIX)DjVuFile$(SHARED_EXT)
XPSFILE := $(LIBDIR)/$(LIB_PREFIX)XpsFile$(SHARED_EXT)
HTMLFILE := $(LIBDIR)/$(LIB_PREFIX)HtmlFile$(SHARED_EXT)
UNICODECONVERTER := $(LIBDIR)/$(LIB_PREFIX)UnicodeConverter$(SHARED_EXT)
ASCDOCUMENTSCORE := $(LIBDIR)/$(LIB_PREFIX)ascdocumentscore$(SHARED_EXT)
LIBXML := $(LIBDIR)/$(LIB_PREFIX)libxml$(LIB_EXT)
LICENSEMANAGER := $(LIBDIR)/$(LIB_PREFIX)LicenceManager$(LIB_EXT)
OOXMLSIGNATURE := $(LIBDIR)/$(LIB_PREFIX)ooxmlsignature$(LIB_EXT)
HUNSPELL := $(LIBDIR)/$(LIB_PREFIX)hunspell$(SHARED_EXT)

TARGETS += $(ALLFONTSGEN)
TARGETS += $(X2T)
TARGETS += $(DOCBUILDER)
TARGETS += $(HTMLFILEINTERNAL)
TARGETS += $(XLSFORMATLIB)
TARGETS += $(ODFFILEWRITERLIB)
TARGETS += $(ODFFILEREADERLIB)
TARGETS += $(DOCFORMATLIB)
TARGETS += $(PPTFORMATLIB)
TARGETS += $(RTFFORMATLIB)
TARGETS += $(TXTXMLFORMATLIB)
TARGETS += $(PDFWRITER)
TARGETS += $(ASCOFFICEDOCXFILE2LIB)
TARGETS += $(PPTXFORMATLIB)
TARGETS += $(DOCXFORMATLIB)
TARGETS += $(OFFICEUTILS)
TARGETS += $(GRAPHICS)
TARGETS += $(DOCTRENDERER)
TARGETS += $(HTMLRENDERER)
TARGETS += $(PDFREADER)
TARGETS += $(DJVUFILE)
TARGETS += $(XPSFILE)
TARGETS += $(HTMLFILE)
TARGETS += $(UNICODECONVERTER)
TARGETS += $(ASCDOCUMENTSCORE)
TARGETS += $(LIBXML)
TARGETS += $(LICENSEMANAGER)
TARGETS += $(OOXMLSIGNATURE)
TARGETS += $(HUNSPELL)

X2T_PRO := $(abspath X2tConverter/build/Qt/X2tSLN.pro)
HTMLFILEINTERNAL_PRO := $(abspath ../desktop-sdk/HtmlFile/Internal/Internal.pro)
ALLFONTSGEN_PRO := $(abspath DesktopEditor/AllFontsGen/AllFontsGen.pro)
DOCBUILDER_PRO := $(abspath ../core-ext/docbuilder/test_builder/docbuilder.pro)
XLSFORMATLIB_PRO := $(abspath ASCOfficeXlsFile2/source/linux/XlsFormatLib.pro)
ODFFILEWRITERLIB_PRO := $(abspath ASCOfficeOdfFileW/linux/OdfFileWriterLib.pro)
ODFFILEREADERLIB_PRO := $(abspath ASCOfficeOdfFile/linux/OdfFileReaderLib.pro)
DOCFORMATLIB_PRO := $(abspath ASCOfficeDocFile/DocFormatLib/Linux/DocFormatLib.pro)
PPTFORMATLIB_PRO := $(abspath ASCOfficePPTFile/PPTFormatLib/Linux/PPTFormatLib.pro)
RTFFORMATLIB_PRO := $(abspath ASCOfficeRtfFile/RtfFormatLib/Linux/RtfFormatLib.pro)
TXTXMLFORMATLIB_PRO := $(abspath ASCOfficeTxtFile/TxtXmlFormatLib/Linux/TxtXmlFormatLib.pro)
PDFWRITER_PRO := $(abspath PdfWriter/PdfWriter.pro)
ASCOFFICEDOCXFILE2LIB_PRO := $(abspath ASCOfficeDocxFile2/Linux/ASCOfficeDocxFile2Lib.pro)
PPTXFORMATLIB_PRO := $(abspath ASCOfficePPTXFile/PPTXLib/Linux/PPTXFormatLib/PPTXFormatLib.pro)
DOCXFORMATLIB_PRO := $(abspath Common/DocxFormat/DocxFormatLib/DocxFormatLib.pro)
OFFICEUTILS_PRO := $(abspath OfficeUtils/OfficeUtils.pro)
GRAPHICS_PRO := $(abspath DesktopEditor/Qt_build/graphics/project/graphics.pro)
DOCTRENDERER_PRO := $(abspath DesktopEditor/doctrenderer/doctrenderer.pro)
HTMLRENDERER_PRO := $(abspath HtmlRenderer/htmlrenderer.pro)
PDFREADER_PRO := $(abspath PdfReader/PdfReader.pro)
DJVUFILE_PRO := $(abspath DjVuFile/DjVuFile.pro)
XPSFILE_PRO := $(abspath XpsFile/XpsFile.pro)
HTMLFILE_PRO := $(abspath HtmlFile/HtmlFile.pro)
UNICODECONVERTER_PRO := $(abspath UnicodeConverter/UnicodeConverter.pro)
ASCDOCUMENTSCORE_PRO := $(abspath ../desktop-sdk/ChromiumBasedEditors/lib/AscDocumentsCore_$(PLATFORM).pro)
LIBXML_PRO := $(abspath DesktopEditor/xml/build/qt/libxml2.pro)
LICENSEMANAGER_PRO := $(abspath LicenceManager/linux/LicenseManager.pro)
OOXMLSIGNATURE_PRO := $(abspath DesktopEditor/xmlsec/src/ooxmlsignature.pro)
HUNSPELL_PRO := $(abspath DesktopEditor/hunspell-1.3.3/src/qt/hunspell.pro)

# PROS += $(basename $(X2T_PRO)).build
# PROS += ALLFONTSGEN_PRO
# PROS += XLSFORMATLIB_PRO
# PROS += ODFFILEWRITERLIB_PRO
# PROS += ODFFILEREADERLIB_PRO
# PROS += DOCFORMATLIB_PRO
# PROS += PPTFORMATLIB_PRO
# PROS += RTFFORMATLIB_PRO
# PROS += TXTXMLFORMATLIB_PRO
# PROS += PDFWRITER_PRO
# PROS += ASCOFFICEDOCXFILE2LIB_PRO
# PROS += PPTXFORMATLIB_PRO
# PROS += DOCXFORMATLIB_PRO
# PROS += OFFICEUTILS_PRO
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
QT_PROJ += DOCBUILDER
QT_PROJ += XLSFORMATLIB
QT_PROJ += ODFFILEWRITERLIB
QT_PROJ += ODFFILEREADERLIB
QT_PROJ += DOCFORMATLIB
QT_PROJ += PPTFORMATLIB
QT_PROJ += RTFFORMATLIB
QT_PROJ += TXTXMLFORMATLIB
QT_PROJ += PDFWRITER
QT_PROJ += ASCOFFICEDOCXFILE2LIB
QT_PROJ += PPTXFORMATLIB
QT_PROJ += DOCXFORMATLIB
QT_PROJ += OFFICEUTILS
QT_PROJ += GRAPHICS
QT_PROJ += DOCTRENDERER
QT_PROJ += HTMLRENDERER
QT_PROJ += PDFREADER
QT_PROJ += DJVUFILE
QT_PROJ += XPSFILE
QT_PROJ += HTMLFILE
QT_PROJ += UNICODECONVERTER
QT_PROJ += ASCDOCUMENTSCORE
QT_PROJ += LIBXML
QT_PROJ += LICENSEMANAGER
QT_PROJ += OOXMLSIGNATURE
QT_PROJ += HUNSPELL

# X2T_DEP += $(XLSFORMATLIB)
# X2T_DEP += $(ODFFILEWRITERLIB)
# X2T_DEP += $(ODFFILEREADERLIB)
# X2T_DEP += $(DOCFORMATLIB)
# X2T_DEP += $(PPTFORMATLIB)
# X2T_DEP += $(RTFFORMATLIB)
# X2T_DEP += $(TXTXMLFORMATLIB)
# X2T_DEP += $(PDFWRITER)
# X2T_DEP += $(ASCOFFICEDOCXFILE2LIB)
# X2T_DEP += $(PPTXFORMATLIB)
# X2T_DEP += $(DOCXFORMATLIB)
# X2T_DEP += $(OFFICEUTILS)
# X2T_DEP += $(GRAPHICS)
# X2T_DEP += $(DOCTRENDERER)
# X2T_DEP += $(HTMLRENDERER)
# X2T_DEP += $(PDFREADER)
# X2T_DEP += $(DJVUFILE)
# X2T_DEP += $(XPSFILE)
# X2T_DEP += $(HTMLFILE)
# X2T_DEP += $(UNICODECONVERTER)

HTMLFILEINTERNAL_DEP += $(GRAPHICS)
HTMLFILEINTERNAL_DEP += $(LIBXML)
HTMLFILEINTERNAL_DEP += $(UNICODECONVERTER)

XPSFILE_DEP += $(PDFWRITER)

ALLFONTSGEN_DEP += $(GRAPHICS)
ALLFONTSGEN_DEP += $(OFFICEUTILS)
ALLFONTSGEN_DEP += $(UNICODECONVERTER)

DOCBUILDER_DEP += $(DOCTRENDERER)

HTMLFILE_DEP += $(UNICODECONVERTER)

RTFFORMATLIB_DEP += $(UNICODECONVERTER)

XLSFORMATLIB_DEP += $(UNICODECONVERTER)

DJVUFILE_DEP += $(PDFWRITER)

ASCDOCUMENTSCORE_DEP += $(GRAPHICS)
ASCDOCUMENTSCORE_DEP += $(OFFICEUTILS)
ASCDOCUMENTSCORE_DEP += $(PDFWRITER)
ASCDOCUMENTSCORE_DEP += $(PDFREADER)
ASCDOCUMENTSCORE_DEP += $(DJVUFILE)
ASCDOCUMENTSCORE_DEP += $(XPSFILE)
#ASCDOCUMENTSCORE_DEP += $(LICENSEMANAGER)
ASCDOCUMENTSCORE_DEP += $(LIBXML)
ASCDOCUMENTSCORE_DEP += $(OOXMLSIGNATURE)
ASCDOCUMENTSCORE_DEP += $(HUNSPELL)

OOXMLSIGNATURE_DEP += $(LIBXML)

PDFREADER_DEP += $(HTMLRENDERER)

PDFWRITER_DEP += $(UNICODECONVERTER)

ARCHIVE := ./$(PACKAGE_NAME)$(ARCH_EXT)

ARTIFACTS += build/*
ARTIFACTS += Common/3dParty/*/$(TARGET)/build/*


ifeq ($(OS),Windows_NT)
ARTIFACTS += Common/3dParty/v8/$(TARGET)/*/*.dll
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

.PHONY : all bin lib clean deploy

all: lib bin 

bin: $(X2T) $(ALLFONTSGEN)

lib: $(PDFWRITER) $(DOCTRENDERER) $(HTMLRENDERER) $(PDFREADER) $(DJVUFILE) $(XPSFILE) $(HTMLFILE) $(UNICODECONVERTER)

ext: $(HTMLFILEINTERNAL) $(DOCBUILDER)

desktop: $(ASCDOCUMENTSCORE)

$(foreach proj, $(QT_PROJ), $(eval $(call build_proj_tmpl, $(proj))))

#$(X2T): $(X2T_DEP)

$(HTMLFILEINTERNAL): $(HTMLFILEINTERNAL_DEP)

$(XPSFILE): $(XPSFILE_DEP)

$(ALLFONTSGEN): $(ALLFONTSGEN_DEP)

$(DOCBUILDER): $(DOCBUILDER_DEP)

$(HTMLFILE): $(HTMLFILE_DEP)

$(RTFFORMATLIB): $(RTFFORMATLIB_DEP)

$(XLSFORMATLIB): $(XLSFORMATLIB_DEP)

$(DJVUFILE): $(DJVUFILE_DEP)

$(ASCDOCUMENTSCORE): $(ASCDOCUMENTSCORE_DEP)

$(PDFREADER): $(PDFREADER_DEP)

$(PDFWRITER): $(PDFWRITER_DEP)

$(OOXMLSIGNATURE): $(OOXMLSIGNATURE_DEP)

%.build/Makefile: %.pro
	mkdir -p $(dir $@) && cd $(dir $@) && qmake -r $<

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

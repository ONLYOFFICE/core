QT       -= core gui

VERSION = 1.0.0.0
TARGET = EpubFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

DEFINES += EPUBFILE_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel)

SOURCES += src/CEpubFile.cpp \
    src/Docx/hyperlink/CHyperlink.cpp \
    src/Docx/docRels/CDocRelationships.cpp \
    src/Docx/core/CCoreProperties.cpp \
    src/Docx/app/CAppXml.cpp \
    src/Docx/app/CProperties.cpp \
    src/Docx/core/CCoreXml.cpp \
    src/Docx/docRels/CDocRelationshipsXml.cpp \
    src/Docx/rels/CRelationships.cpp \
    src/Docx/rels/CRelationshipsXml.cpp \
    src/Docx/sdt/CSdt.cpp \
    src/Docx/sdt/CSdtContent.cpp \
    src/Docx/sdt/CSdtEndPr.cpp \
    src/Docx/sdt/CSdtPr.cpp \
    src/Docx/style/CStyle.cpp \
    src/Docx/style/CStyles.cpp \
    src/Docx/style/CStylesXml.cpp \
    src/Docx/webSettings/CWebSettings.cpp \
    src/Docx/settings/CSettings.cpp \
    src/Docx/fontTable/CFont.cpp \
    src/Docx/fontTable/CFontTable.cpp \
    src/Docx/CXmlFile.cpp \
    src/Docx/contentType/CContentType.cpp \
    src/Docx/contentType/CContentTypeXml.cpp \
    src/Docx/document/CDocument.cpp \
    src/Docx/document/CDocumentXml.cpp \
    src/Docx/document/CParagraphProperties.cpp \
    src/Docx/document/CRun.cpp \
    src/Docx/document/CRunProperties.cpp \
    src/Docx/CElement.cpp \
    src/Docx/document/CParagraph.cpp \
    src/Docx/document/CBody.cpp \
    src/Docx/CFileDocument.cpp \
    src/Docx/CDocxFile.cpp \
    src/CBookToc.cpp \
    src/CBookContentItem.cpp \
    src/CBookItem.cpp \
    src/CBookInfo.cpp \
    src/Docx/document/CSectionProperties.cpp \
    src/Docx/fontTable/CFontTableXml.cpp \
    src/Docx/settings/CSettingsXml.cpp \
    src/Docx/webSettings/CWebSettingsXml.cpp

HEADERS += CEpubFile.h \
    src/Docx/hyperlink/CHyperlink.h \
    src/Docx/docRels/CDocRelationships.h \
    src/Docx/core/CCoreProperties.h \
    src/Docx/app/CAppXml.h \
    src/Docx/app/CProperties.h \
    src/Docx/core/CCoreXml.h \
    src/Docx/docRels/CDocRelationshipsXml.h \
    src/Docx/rels/CRelationships.h \
    src/Docx/rels/CRelationshipsXml.h \
    src/Docx/sdt/CSdt.h \
    src/Docx/sdt/CSdtContent.h \
    src/Docx/sdt/CSdtEndPr.h \
    src/Docx/sdt/CSdtPr.h \
    src/Docx/style/CStyle.h \
    src/Docx/style/CStyles.h \
    src/Docx/style/CStylesXml.h \
    src/Docx/webSettings/CWebSettings.h \
    src/Docx/settings/CSettings.h \
    src/Docx/fontTable/CFont.h \
    src/Docx/fontTable/CFontTable.h \
    src/Docx/CXmlFile.h \
    src/Docx/contentType/CContentType.h \
    src/Docx/contentType/CContentTypeXml.h \
    src/Docx/document/CDocument.h \
    src/Docx/document/CDocumentXml.h \
    src/Docx/document/CParagraphProperties.h \
    src/Docx/document/CRun.h \
    src/Docx/document/CRunProperties.h \
    src/Docx/CElement.h \
    src/Docx/document/CParagraph.h \
    src/Docx/document/CBody.h \
    src/Docx/CFileDocument.h \
    src/Docx/CDocxFile.h \
    src/CBookToc.h \
    src/CBookContentItem.h \
    src/CBookItem.h \
    src/CBookInfo.h \
    src/Docx/document/CSectionProperties.h \
    src/Docx/fontTable/CFontTableXml.h \
    src/Docx/settings/CSettingsXml.h \
    src/Docx/webSettings/CWebSettingsXml.h \
    src/EpubFile_global.h


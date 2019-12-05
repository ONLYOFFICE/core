QT       -= core gui

VERSION = 1.0.0.4
TARGET = PdfReader
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

DEFINES += PDFREADER_USE_DYNAMIC_LIBRARY
DEFINES += HTMLRENDERER_USE_DYNAMIC_LIBRARY

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, HtmlRenderer)

DEFINES += CRYPTOPP_DISABLE_ASM
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

core_windows {

LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

SOURCES += \
    Src/Annot.cpp \
    Src/Array.cpp \
    Src/Catalog.cpp \
    Src/CharCodeToUnicode.cpp \
    Src/CMap.cpp \
    Src/Decrypt.cpp \
    Src/Dict.cpp \
    Src/ExtractImageOutputDev.cpp \
    Src/FontFileBase.cpp \
    Src/FontFileTrueType.cpp \
    Src/FontFileType1.cpp \
    Src/FontFileType1C.cpp \
    Src/Function.cpp \
    Src/GFont.cpp \
    Src/GlobalParams.cpp \
    Src/Graphics.cpp \
    Src/GState.cpp \
    Src/Hash.cpp \
    Src/JArithmeticDecoder.cpp \
    Src/JBIG2Stream.cpp \
    Src/JPXStream.cpp \
    Src/Lexer.cpp \
    Src/Link.cpp \
    Src/List.cpp \
    Src/NameToCharCode.cpp \
    Src/Object.cpp \
    Src/Outline.cpp \
    Src/OutputDevice.cpp \
    Src/Page.cpp \
    Src/Parser.cpp \
    Src/PDFDoc.cpp \
    Src/PSLexer.cpp \
    Src/RendererOutputDev.cpp \
    Src/SecurityHandler.cpp \
    Src/Stream.cpp \
    Src/StringExt.cpp \
    Src/UnicodeMap.cpp \
    Src/XRef.cpp \
    PdfReader.cpp

HEADERS +=\
    Resources/Fontd050000l.h \
    Resources/Fontn019003l.h \
    Resources/Fontn019004l.h \
    Resources/Fontn019023l.h \
    Resources/Fontn019024l.h \
    Resources/Fontn021003l.h \
    Resources/Fontn021004l.h \
    Resources/Fontn021023l.h \
    Resources/Fontn021024l.h \
    Resources/Fontn022003l.h \
    Resources/Fontn022004l.h \
    Resources/Fontn022023l.h \
    Resources/Fontn022024l.h \
    Resources/Fonts050000l.h \
    Src/Annot.h \
    Src/Array.h \
    Src/BuiltinFont.h \
    Src/BuiltinFontTables.h \
    Src/Catalog.h \
    Src/CCITT-Tables.h \
    Src/CharCodeToUnicode.h \
    Src/CharTypes.h \
    Src/CMap.h \
    Src/Constants.h \
    Src/Decrypt.h \
    Src/Dict.h \
    Src/EncodingTables.h \
    Src/ErrorConstants.h \
    Src/ExtractImageOutputDev.h \
    Src/File.h \
    Src/FontFileBase.h \
    Src/FontFileEncodings.h \
    Src/FontFileTrueType.h \
    Src/FontFileType1.h \
    Src/FontFileType1C.h \
    Src/Function.h \
    Src/GFont.h \
    Src/GlobalParams.h \
    Src/Graphics.h \
    Src/GState.h \
    Src/Hash.h \
    Src/JArithmeticDecoder.h \
    Src/JBIG2Stream.h \
    Src/JPXStream.h \
    Src/Lexer.h \
    Src/Link.h \
    Src/List.h \
    Src/MemoryUtils.h \
    Src/NameToCharCode.h \
    Src/NameToUnicodeTable.h \
    Src/Object.h \
    Src/Outline.h \
    Src/OutputDevice.h \
    Src/Page.h \
    Src/Parser.h \
    Src/PDFDoc.h \
    Src/PDFDocEncoding.h \
    Src/PSLexer.h \
    Src/RendererOutputDev.h \
    Src/SecurityHandler.h \
    Src/Stream.h \
    Src/StringExt.h \
    Src/UnicodeMap.h \
    Src/UnicodeMapTables.h \
    Src/UTF8.h \
    Src/XmlUtils.h \
    Src/XRef.h \
    PdfReader.h

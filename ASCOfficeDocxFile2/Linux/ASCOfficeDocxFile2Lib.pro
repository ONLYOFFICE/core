#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = ASCOfficeDocxFile2Lib
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++0x -Wall -Wno-ignored-qualifiers -g
DEFINES += UNICODE \
	_UNICODE \
	LINUX \
	_LINUX_QT \
	NODOCX \
	_USE_XMLLITE_READER_ \
	USE_LITE_READER \
	USE_ATL_CSTRING \
	USE_AVSOFFICESTUDIO_XMLUTILS \
	SOLUTION_ASCOFFICEDOCXFILE2 \
	DISABLE_FILE_DOWNLOADER \
	_USE_LIBXML2_READER_ \
	LIBXML_READER_ENABLED

INCLUDEPATH += \
    ../../DesktopEditor/freetype-2.5.2/include \
    ../../Common/DocxFormat/Source/XML/libxml2/XML/include

INCLUDEPATH += \
    /usr/include/libxml2

LIBS += -lxml2
	
SOURCES += ../DocWrapper/DocxSerializer.cpp \
    ../DocWrapper/FontProcessor.cpp \
    ../DocWrapper/XlsxSerializer.cpp \
    ../../XlsxSerializerCom/Common/Common.cpp \
    ../../XlsxSerializerCom/Reader/ChartFromToBinary.cpp \
    ../../XlsxSerializerCom/Reader/CommonWriter.cpp \
    ../../XlsxSerializerCom/Reader/CSVReader.cpp \
    ../../XlsxSerializerCom/Writer/CSVWriter.cpp

HEADERS += ../DocWrapper/DocxSerializer.h \
    ../DocWrapper/FontProcessor.h \
    ../DocWrapper/XlsxSerializer.h \
    ../BinReader/ChartWriter.h \
    ../BinReader/CommentsWriter.h \
    ../BinReader/ContentTypesWriter.h \
    ../BinReader/DocumentRelsWriter.h \
    ../BinReader/DocumentWriter.h \
    ../BinReader/FileWriter.h \
    ../BinReader/fontTableWriter.h \
    ../BinReader/HeaderFooterWriter.h \
    ../BinReader/MediaWriter.h \
    ../BinReader/NumberingWriter.h \
    ../BinReader/ReaderClasses.h \
    ../BinReader/Readers.h \
    ../BinReader/SettingWriter.h \
    ../BinReader/StylesWriter.h \
    ../BinWriter/BinEquationWriter.h \
    ../BinWriter/BinReaderWriterDefines.h \
    ../BinWriter/BinWriters.h \
    ../../XlsxSerializerCom/Common/BinReaderWriterDefines.h \
    ../../XlsxSerializerCom/Common/Common.h \
    ../../XlsxSerializerCom/Reader/BinaryWriter.h \
    ../../XlsxSerializerCom/Reader/ChartFromToBinary.h \
    ../../XlsxSerializerCom/Reader/CommonWriter.h \
    ../../XlsxSerializerCom/Reader/CSVReader.h \
    ../../XlsxSerializerCom/Writer/BinaryCommonReader.h \
    ../../XlsxSerializerCom/Writer/BinaryReader.h \
    ../../XlsxSerializerCom/Writer/CSVWriter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

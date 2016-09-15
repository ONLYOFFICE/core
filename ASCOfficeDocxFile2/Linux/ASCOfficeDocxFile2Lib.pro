QT       -= core gui

TARGET = ASCOfficeDocxFile2Lib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t

include(../../Common/base.pri)

#BOOST
include($$PWD/../../Common/3dParty/boost/boost.pri)

DEFINES += UNICODE \
	_UNICODE \
        NODOCX \
	_USE_XMLLITE_READER_ \
	USE_LITE_READER \
	USE_ATL_CSTRING \
	USE_AVSOFFICESTUDIO_XMLUTILS \
	SOLUTION_ASCOFFICEDOCXFILE2 \
        #DISABLE_FILE_DOWNLOADER \
	_USE_LIBXML2_READER_ \
	LIBXML_READER_ENABLED

INCLUDEPATH += \
    ../../DesktopEditor/freetype-2.5.2/include \
    ../../DesktopEditor/xml/libxml2/include \


SOURCES += ../DocWrapper/DocxSerializer.cpp \
    ../DocWrapper/FontProcessor.cpp \
    ../DocWrapper/XlsxSerializer.cpp \
    ../../XlsxSerializerCom/Common/Common.cpp \
    ../../XlsxSerializerCom/Reader/ChartFromToBinary.cpp \
    ../../XlsxSerializerCom/Reader/CommonWriter.cpp \
    ../../XlsxSerializerCom/Reader/CSVReader.cpp \
    ../../XlsxSerializerCom/Writer/CSVWriter.cpp \
    ../DocWrapper/ChartWriter.cpp \
    ../../OfficeCryptReader/source/ECMACryptReader.cpp \
    ../../OfficeCryptReader/source/CryptTransform.cpp

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
    ../../XlsxSerializerCom/Writer/CSVWriter.h \
    ../BinReader/webSettingsWriter.h \
    ../../Common/FileDownloader/FileDownloader.h \
    ../BinReader/DefaultThemeWriter.h \
    ../DocWrapper/ChartWriter.h \
    ../../OfficeCryptReader/source/ECMACryptReader.h \
    ../../OfficeCryptReader/source/CryptTransform.h

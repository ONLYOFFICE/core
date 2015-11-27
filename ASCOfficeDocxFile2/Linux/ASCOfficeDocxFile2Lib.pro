#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = ASCOfficeDocxFile2Lib
TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

win32 {
    QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
} else {
    QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers
}

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../SDK/lib

# WINDOWS
win32:contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
CONFIG(debug, debug|release) {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTDIR = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++ | linux-g++-64 | linux-g++-32:contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++ | linux-g++-64 | linux-g++-32:!contains(QMAKE_HOST.arch, x86_64):{
    DESTDIR = $$DESTINATION_SDK_PATH/linux_32
}

mac {
    DESTDIR = $$DESTINATION_SDK_PATH/mac_64
}
############### destination path ###############

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
    ../../DesktopEditor/freetype-2.5.2/include

#################### WINDOWS #####################
win32 {
INCLUDEPATH += ../../Common/DocxFormat/Source/XML/libxml2/XML/include
}
#################### WINDOWS #####################

#################### LINUX ########################
linux-g++ | linux-g++-64 | linux-g++-32 {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT

INCLUDEPATH += /usr/include/libxml2
}

mac {
    DEFINES += \
        LINUX \
        _LINUX \
        _LINUX_QT \
        _MAC \
        MAC

INCLUDEPATH += ../../DesktopEditor/xml/libxml2/include
}
#################### LINUX ########################

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
    ../../XlsxSerializerCom/Writer/CSVWriter.h \
    ../BinReader/webSettingsWriter.h \
    ../../Common/FileDownloader.h \
    ../BinReader/DefaultThemeWriter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


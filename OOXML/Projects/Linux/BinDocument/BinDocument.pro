QT       -= core gui

TARGET = BinDocument
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t

include(../../../../Common/base.pri)

#BOOST
include($$PWD/../../../../Common/3dParty/boost/boost.pri)

INCLUDEPATH += ../../../../MsBinaryFile/XlsFile/Format
INCLUDEPATH += ../../../../MsBinaryFile/Common/common_xls
INCLUDEPATH += ../../../XlsbFormat

DEFINES += UNICODE \
	_UNICODE \
	SOLUTION_ASCOFFICEDOCXFILE2 \
	#DISABLE_FILE_DOWNLOADER \
	DONT_WRITE_EMBEDDED_FONTS \
	AVS_USE_CONVERT_PPTX_TOCUSTOM_VML

!disable_precompiled_header:CONFIG += precompile_header
precompile_header {
    PRECOMPILED_HEADER = precompiled.h
    HEADERS += precompiled.h
}

SOURCES += \
	../../../Binary/Document/DocWrapper/DocxSerializer.cpp \
	../../../Binary/Document/DocWrapper/FontProcessor.cpp \
	../../../Binary/Document/DocWrapper/XlsxSerializer.cpp \
	../../../Binary/Document/DocWrapper/ChartSerializer.cpp \
	../../../Binary/Document/BinWriter/BinWriters.cpp \
	../../../Binary/Sheets/Common/Common.cpp \
	../../../Binary/Sheets/Reader/ChartFromToBinary.cpp \
	../../../Binary/Sheets/Reader/CommonWriter.cpp \
	../../../Binary/Sheets/Reader/CSVReader.cpp \
	../../../Binary/Sheets/Reader/BinaryWriter.cpp \
	../../../Binary/Sheets/Writer/BinaryReader.cpp \
	../../../Binary/Sheets/Writer/CSVWriter.cpp \
	../../../../OfficeCryptReader/source/ECMACryptFile.cpp \
	../../../../OfficeCryptReader/source/CryptTransform.cpp \
	../../../Binary/Document/BinReader/ReaderClasses.cpp \
	../../../Binary/Document/BinReader/Readers.cpp \
	../../../Binary/Document/BinReader/CustomXmlWriter.cpp \
	../../../Binary/Document/BinReader/FileWriter.cpp \
	../../../Binary/Document/BinReader/ChartWriter.cpp \
	../../../Binary/Document/BinReader/CommentsWriter.cpp \
	../../../Binary/Document/BinReader/DefaultThemeWriter.cpp \
	../../../Binary/Document/BinReader/DocumentRelsWriter.cpp \
	../../../Binary/Document/BinReader/DocumentWriter.cpp \
	../../../Binary/Document/BinReader/fontTableWriter.cpp \
	../../../Binary/Document/BinReader/HeaderFooterWriter.cpp \
	../../../Binary/Document/BinReader/MediaWriter.cpp \
	../../../Binary/Document/BinReader/NumberingWriter.cpp \
	../../../Binary/Document/BinReader/SettingWriter.cpp \
	../../../Binary/Document/BinReader/StylesWriter.cpp \
	../../../Binary/Document/BinReader/webSettingsWriter.cpp

HEADERS += \
	../../../Binary/Document/DocWrapper/DocxSerializer.h \
	../../../Binary/Document/DocWrapper/FontProcessor.h \
	../../../Binary/Document/DocWrapper/XlsxSerializer.h \
	../../../Binary/Document/BinReader/ChartWriter.h \
	../../../Binary/Document/BinReader/CommentsWriter.h \
	../../../Binary/Document/BinReader/DocumentRelsWriter.h \
	../../../Binary/Document/BinReader/DocumentWriter.h \
	../../../Binary/Document/BinReader/FileWriter.h \
	../../../Binary/Document/BinReader/fontTableWriter.h \
	../../../Binary/Document/BinReader/HeaderFooterWriter.h \
	../../../Binary/Document/BinReader/MediaWriter.h \
	../../../Binary/Document/BinReader/NumberingWriter.h \
	../../../Binary/Document/BinReader/ReaderClasses.h \
	../../../Binary/Document/BinReader/Readers.h \
	../../../Binary/Document/BinReader/SettingWriter.h \
	../../../Binary/Document/BinReader/StylesWriter.h \
	../../../Binary/Document/BinWriter/BinEquationWriter.h \
	../../../Binary/Document/BinWriter/BinReaderWriterDefines.h \
	../../../Binary/Document/BinWriter/BinWriters.h \
	../../../Binary/Sheets/Common/BinReaderWriterDefines.h \
	../../../Binary/Sheets/Common/Common.h \
	../../../Binary/Sheets/Reader/BinaryWriter.h \
	../../../Binary/Sheets/Reader/ChartFromToBinary.h \
	../../../Binary/Sheets/Reader/CommonWriter.h \
	../../../Binary/Sheets/Reader/CSVReader.h \
	../../../Binary/Sheets/Writer/BinaryCommonReader.h \
	../../../Binary/Sheets/Writer/BinaryReader.h \
	../../../Binary/Sheets/Writer/CSVWriter.h \
	../../../Binary/Document/BinReader/webSettingsWriter.h \
	../../../../Common/FileDownloader/FileDownloader.h \
	../../../Binary/Document/BinReader/DefaultThemeWriter.h \
	../../../Binary/Document/DocWrapper/ChartWriter.h \
	../../../../OfficeCryptReader/source/ECMACryptFile.h \
	../../../../OfficeCryptReader/source/CryptTransform.h \
	../../../Binary/Document/BinReader/CustomXmlWriter.h

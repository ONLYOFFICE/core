cmake_minimum_required(VERSION 3.4.1)

# Examples
# Exclude from list by regex
#list(FILTER XML_EDITOR_CPP EXCLUDE REGEX "^${XML_EDITOR_DIR}libxml2/test.*\.c$")

set(CMAKE_VERBOSE_MAKEFILE on)

# Set global definition
add_definitions(
    -D__ANDROID__
)

# Add checks arguments here
# Core path arg
if (NOT DEFINED ARG_SRC_CORE)
    message(FATAL_ERROR "You must set argument \"ARG_SRC_CORE\" with path to core...")
elseif (NOT EXISTS ${ARG_SRC_CORE})
    message(FATAL_ERROR "Core source path doesn't exist!")
endif()

# Result lib name
if (NOT DEFINED ARG_LIB_CORE)
    message(FATAL_ERROR "You must set argument \"ARG_LIB_CORE\" with result lib's name...")
endif()

# 3Party libs path arg
if (NOT DEFINED ARG_LIBS)
    message(FATAL_ERROR "You must set argument \"ARG_LIBS\" with path to 3d-party library...")
elseif (NOT EXISTS ${ARG_LIBS})
    message(FATAL_ERROR "3d party libraries path doesn't exist!")
endif()

# ---------- Libs names ----------
# Main lib
set(LIB_NAME_CORE ${ARG_LIB_CORE})

# Converter lib name
set(LIB_NAME_X2T x2t-converter)

# 3d party libraries
set(LIB_NAME_OPENSSL openssl)
set(LIB_NAME_SSL ssl)
set(LIB_NAME_CRYPTO crypto)
set(LIB_NAME_CURL curl)
set(LIB_NAME_ICONV iconv)
set(LIB_NAME_BOOST boost)
set(LIB_NAME_BOOST_REGEX boost_regex)
set(LIB_NAME_ICU icu)
set(LIB_NAME_ICU_DATA icudata)
set(LIB_NAME_ICU_i18n icui18n)
set(LIB_NAME_ICU_UC icuuc)

# Common
set(LIB_NAME_CPUFEATURE cpufeature)
set(LIB_NAME_UNICODE_CONVERTER unicodeconv)
set(LIB_NAME_OFFICE_UTILS officeutils)
set(LIB_NAME_CRYPTO_READER crypto-reader)
set(LIB_NAME_DOCX_RENDER docx-render)
set(LIB_NAME_CRYPTOPP cryptopp)
set(LIB_NAME_XLSX_SERIALIZE xlsx-serialize)
set(LIB_NAME_POLE pole)
set(LIB_NAME_DOWNLOADER downloader)

# Desktop editor
set(LIB_NAME_AGG agg)
set(LIB_NAME_FREETYPES freetype)
set(LIB_NAME_EDITOR_COMMON editor-common)
set(LIB_NAME_EDITOR_XML editor-xml)
set(LIB_NAME_FONT_ENGINE font-engine)
set(LIB_NAME_DOCT_RENDER doct-render)

# CxImage libraries
set(LIB_NAME_CXIMAGE cximage)
set(LIB_NAME_JASPER jasper)
set(LIB_NAME_JPEG jpeg)
set(LIB_NAME_PNG png)
set(LIB_NAME_JBIG jbig)
set(LIB_NAME_TIFF tiff)
set(LIB_NAME_RAW raw)
set(LIB_NAME_MNG mng)
set(LIB_NAME_PSD psd)
set(LIB_NAME_ZLIB zlib)

# Formats
set(LIB_NAME_DOCXFORMAT docxformat)
set(LIB_NAME_PDF_WRITER pdf-writer)
set(LIB_NAME_PDF_READER pdf-reader)
set(LIB_NAME_DJVU djvu)
set(LIB_NAME_HTML_FILE html-file)
set(LIB_NAME_HTML_RENDER html-render)
set(LIB_NAME_TXT txt)
set(LIB_NAME_DOC doc)
set(LIB_NAME_DOCX docx)
set(LIB_NAME_PPT ppt)
set(LIB_NAME_PPTX pptx)
set(LIB_NAME_RTF rtf)
set(LIB_NAME_ODF_READER odf-reader)
set(LIB_NAME_ODF_WRITER odf-writer)
set(LIB_NAME_XLS xls)
set(LIB_NAME_XPS xps)

# ---------- Paths sources  ----------
# Core src dir path
set(CORE_DIR ${ARG_SRC_CORE})
message(STATUS "Core source path: ${CORE_DIR}")

# Prebuild libraries path
set(LIBS_DIR ${ARG_LIBS})
message(STATUS "Prebuild libraries path: ${LIBS_DIR}")

# OpenSsl libs path
set(SSL_INCLUDE_DIR ${LIBS_DIR}/${LIB_NAME_OPENSSL}/${ANDROID_ABI}/include)
message(STATUS "OpenSsl include path: ${SSL_INCLUDE_DIR}")
set(SSL_LIB_DIR ${LIBS_DIR}/${LIB_NAME_OPENSSL}/${ANDROID_ABI}/lib)
message(STATUS "OpenSsl lib path: ${SSL_LIB_DIR}")

# Curl libs path
set(CURL_INCLUDE_DIR ${LIBS_DIR}/${LIB_NAME_CURL}/${ANDROID_ABI}/include)
message(STATUS "Curl include path: ${CURL_INCLUDE_DIR}")
set(CURL_LIB_DIR ${LIBS_DIR}/${LIB_NAME_CURL}/${ANDROID_ABI}/lib)
message(STATUS "Curl lib path: ${CURL_LIB_DIR}")

# Iconv libs path
set(ICONV_INCLUDE_DIR ${LIBS_DIR}/${LIB_NAME_ICONV}/${ANDROID_ABI}/include)
message(STATUS "Iconv include path: ${ICONV_INCLUDE_DIR}")
set(ICONV_LIB_DIR ${LIBS_DIR}/${LIB_NAME_ICONV}/${ANDROID_ABI}/lib)
message(STATUS "Iconv lib path: ${ICONV_LIB_DIR}")

# Boost src dir path
set(BOOST_INCLUDE_DIR ${CORE_DIR}/Common/3dParty/${LIB_NAME_BOOST}/boost_1_58_0)
message(STATUS "Boost include path: ${BOOST_INCLUDE_DIR}")
set(BOOST_LIB_DIR ${LIBS_DIR}/${LIB_NAME_BOOST}/${ANDROID_ABI}/lib)
message(STATUS "Boost lib path: ${BOOST_LIB_DIR}")

# Icu src dir path
set(ICU_INCLUDE_DIR ${LIBS_DIR}/${LIB_NAME_ICU}/${ANDROID_ABI}/include)
message(STATUS "Icu include path: ${ICU_INCLUDE_DIR}")
set(ICU_LIB_DIR ${LIBS_DIR}/${LIB_NAME_ICU}/${ANDROID_ABI}/lib)
message(STATUS "Icu lib path: ${ICU_LIB_DIR}")

# Add global path for lib resources here
# Workaround src dir path
set(WORKAROUND_DIR ${CMAKE_CURRENT_SOURCE_DIR}/workaround)
message(STATUS "Workaround source path: ${WORKAROUND_DIR}")
# Check path
if (NOT EXISTS ${WORKAROUND_DIR})
    message(FATAL_ERROR "Workaround source path doesn't exist!")
endif()

# Core project dir path
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "Core project path: ${SRC_DIR}")
# Core temp build dir path
set(BUILD_DIR ${CMAKE_BINARY_DIR}/tmp)
message(STATUS "Core build path: ${BUILD_DIR}")
file(MAKE_DIRECTORY ${BUILD_DIR})

# CpuFeature src path
set(CPU_FEATURE_DIR ${ANDROID_NDK}/sources/android/cpufeatures)
message(STATUS "CpuFeature source path: ${CPU_FEATURE_DIR}")

# Core modules path
# X2T
set(X2T_DIR ${CORE_DIR}/X2tConverter/)
message(STATUS "X2t converter source path: ${X2T_DIR}")

# Common
# CryptoPP src path
set(CRYPTOPP_DIR ${CORE_DIR}/Common/3dParty/cryptopp/)
message(STATUS "CryptoPP source path: ${CRYPTOPP_DIR}")
# Pole src path
set(POLE_DIR ${CORE_DIR}/Common/3dParty/pole/)
message(STATUS "Pole source path: ${POLE_DIR}")
# File downloader src path
set(DOWNLOADER_DIR ${CORE_DIR}/Common/FileDownloader/)
message(STATUS "File downloader source path: ${DOWNLOADER_DIR}")
# Unicode converter
set(UNICODE_CONVERTER_DIR ${CORE_DIR}/UnicodeConverter/)
message(STATUS "Unicode converter source path: ${UNICODE_CONVERTER_DIR}")
# Office utils
set(OFFICE_UTILS_DIR ${CORE_DIR}/OfficeUtils/)
message(STATUS "Office utils converter source path: ${OFFICE_UTILS_DIR}")
# Xlsx serialize src path
set(XLSX_SERIALIZE_DIR ${CORE_DIR}/XlsxSerializerCom/)
message(STATUS "Xlsx serialize source path: ${XLSX_SERIALIZE_DIR}")
# Crypto reader src path
set(CRYPTO_READER_DIR ${CORE_DIR}/OfficeCryptReader/)
message(STATUS "Crypto reader source path: ${CRYPTO_READER_DIR}")
# Docx renderer src path
set(DOCX_RENDER_DIR ${CORE_DIR}/DocxRenderer/)
message(STATUS "Docx render source path: ${DOCX_RENDER_DIR}")

# DesktopEditor
# FreeTypes src path
set(FREETYPES_DIR ${CORE_DIR}/DesktopEditor/freetype-2.5.2/)
message(STATUS "FreeTypes source path: ${FREETYPES_DIR}")
# Agg src path
set(AGG_DIR ${CORE_DIR}/DesktopEditor/agg-2.4/)
message(STATUS "Agg source path: ${AGG_DIR}")
# Common editor
set(COMMON_EDITOR_DIR ${CORE_DIR}/DesktopEditor/common/)
message(STATUS "Common editor source path: ${COMMON_EDITOR_DIR}")
# Graphics editor
set(GRAPHICS_EDITOR_DIR ${CORE_DIR}/DesktopEditor/graphics/)
message(STATUS "Graphics editor source path: ${GRAPHICS_EDITOR_DIR}")
# Raster editor
set(RASTER_EDITOR_DIR ${CORE_DIR}/DesktopEditor/raster/)
message(STATUS "Raster editor source path: ${RASTER_EDITOR_DIR}")
# Font engine
set(FONT_ENGINE_DIR ${CORE_DIR}/DesktopEditor/fontengine/)
message(STATUS "Font engine source path: ${FONT_ENGINE_DIR}")
# Xml
set(XML_EDITOR_DIR ${CORE_DIR}/DesktopEditor/xml/)
message(STATUS "Xml editor source path: ${XML_EDITOR_DIR}")
# Doct render
set(DOCT_RENDER_DIR ${CORE_DIR}/DesktopEditor/doctrenderer/)
message(STATUS "Doct render source path: ${DOCT_RENDER_DIR}")

# CxImage engine
set(CXIMAGE_DIR ${CORE_DIR}/DesktopEditor/cximage/CxImage/)
message(STATUS "CxImage source path: ${CXIMAGE_DIR}")
# Jasper
set(JASPER_DIR ${CORE_DIR}/DesktopEditor/cximage/jasper/)
message(STATUS "Jasper source path: ${JASPER_DIR}")
# Jpeg
set(JPEG_DIR ${CORE_DIR}/DesktopEditor/cximage/jpeg/)
message(STATUS "Jpeg source path: ${JPEG_DIR}")
# Png
set(PNG_DIR ${CORE_DIR}/DesktopEditor/cximage/png/)
message(STATUS "Png source path: ${PNG_DIR}")
# JBig
set(JBIG_DIR ${CORE_DIR}/DesktopEditor/cximage/jbig/)
message(STATUS "JBig source path: ${JBIG_DIR}")
# Tiff
set(TIFF_DIR ${CORE_DIR}/DesktopEditor/cximage/tiff/)
message(STATUS "Tiff source path: ${TIFF_DIR}")
# Raw
set(RAW_DIR ${CORE_DIR}/DesktopEditor/cximage/raw/)
message(STATUS "Raw source path: ${RAW_DIR}")
# Mng
set(MNG_DIR ${CORE_DIR}/DesktopEditor/cximage/mng/)
message(STATUS "Mng source path: ${MNG_DIR}")
# Psd
set(PSD_DIR ${CORE_DIR}/DesktopEditor/cximage/libpsd/)
message(STATUS "Psd source path: ${PSD_DIR}")
# Zlib
set(ZLIB_DIR ${CORE_DIR}/DesktopEditor/cximage/zlib/)
message(STATUS "Zlib source path: ${ZLIB_DIR}")

# Formats
# DocxFormat src path
set(DOCXFORMAT_DIR ${CORE_DIR}/Common/DocxFormat/)
message(STATUS "Docx format source path: ${DOCXFORMAT_DIR}")
# Pdf writer
set(PDF_WRITER_DIR ${CORE_DIR}/PdfWriter/)
message(STATUS "Pdf writer source path: ${PDF_WRITER_DIR}")
# Pdf reader
set(PDF_READER_DIR ${CORE_DIR}/PdfReader/)
message(STATUS "Pdf reader source path: ${PDF_READER_DIR}")
# DjVu
set(DJVU_DIR ${CORE_DIR}/DjVuFile/)
message(STATUS "DjVu source path: ${DJVU_DIR}")
# Html file
set(HTML_FILE_DIR ${CORE_DIR}/HtmlFile/)
message(STATUS "Html file source path: ${HTML_FILE_DIR}")
# Html render
set(HTML_RENDER_DIR ${CORE_DIR}/HtmlRenderer/)
message(STATUS "Html render source path: ${HTML_RENDER_DIR}")
# Txt
set(TXT_DIR ${CORE_DIR}/ASCOfficeTxtFile/)
message(STATUS "Txt file source path: ${TXT_DIR}")
# Doc
set(DOC_FILE_DIR ${CORE_DIR}/ASCOfficeDocFile/)
message(STATUS "Doc file source path: ${DOC_FILE_DIR}")
# Docx
set(DOCX_FILE_DIR ${CORE_DIR}/ASCOfficeDocxFile2/)
message(STATUS "Docx file source path: ${DOCX_FILE_DIR}")
# Pptx
set(PPTX_FILE_DIR ${CORE_DIR}/ASCOfficePPTXFile/)
message(STATUS "Pptx file source path: ${PPTX_FILE_DIR}")
# Ppt
set(PPT_FILE_DIR ${CORE_DIR}/ASCOfficePPTFile/)
message(STATUS "Ppt file source path: ${PPT_FILE_DIR}")
# Rtf
set(RTF_FILE_DIR ${CORE_DIR}/ASCOfficeRtfFile/)
message(STATUS "Rtf file source path: ${RTF_FILE_DIR}")
# Odf reader
set(ODF_READER_FILE_DIR ${CORE_DIR}/ASCOfficeOdfFile/)
message(STATUS "Odf reader file source path: ${ODF_READER_FILE_DIR}")
# Odf writer
set(ODF_WRITER_FILE_DIR ${CORE_DIR}/ASCOfficeOdfFileW/)
message(STATUS "Odf writer file source path: ${ODF_WRITER_FILE_DIR}")
# Xls
set(XLS_FILE_DIR ${CORE_DIR}/ASCOfficeXlsFile2/)
message(STATUS "Xls file source path: ${XLS_FILE_DIR}")
# Xps
set(XPS_FILE_DIR ${CORE_DIR}/XpsFile/)
message(STATUS "Xps file source path: ${XPS_FILE_DIR}")

# 3d party libraries
# Headers
include_directories(
    ${SSL_INCLUDE_DIR}
    ${CURL_INCLUDE_DIR}
    ${ICONV_INCLUDE_DIR}
    ${BOOST_INCLUDE_DIR}
    ${ICU_INCLUDE_DIR}
)

# Libraries
link_directories(
    ${SSL_LIB_DIR}
    ${CURL_LIB_DIR}
    ${ICONV_LIB_DIR}
    ${BOOST_LIB_DIR}
    ${ICU_LIB_DIR}
)

# Target and dependencies
# Add dependencies sub projects
add_subdirectory(${SRC_DIR}/core/cpufeature ${BUILD_DIR}/core/cpufeature)
add_subdirectory(${SRC_DIR}/core/common/officeutils ${BUILD_DIR}/core/common/officeutils)
add_subdirectory(${SRC_DIR}/core/common/unicodeconverter ${BUILD_DIR}/core/common/unicodeconverter)
add_subdirectory(${SRC_DIR}/core/common/cryptopp ${BUILD_DIR}/core/common/cryptopp)
add_subdirectory(${SRC_DIR}/core/common/pole ${BUILD_DIR}/core/common/pole)
add_subdirectory(${SRC_DIR}/core/common/downloader ${BUILD_DIR}/core/common/downloader)
add_subdirectory(${SRC_DIR}/core/common/cryptoreader ${BUILD_DIR}/core/common/cryptoreader)
add_subdirectory(${SRC_DIR}/core/common/xlsxserialize ${BUILD_DIR}/core/common/xlsxserialize)

add_subdirectory(${SRC_DIR}/core/desktopeditor/freetypes ${BUILD_DIR}/core/desktopeditor/freetypes)
add_subdirectory(${SRC_DIR}/core/desktopeditor/agg ${BUILD_DIR}/core/desktopeditor/agg)
add_subdirectory(${SRC_DIR}/core/desktopeditor/common ${BUILD_DIR}/core/desktopeditor/common)
add_subdirectory(${SRC_DIR}/core/desktopeditor/cximage/cximage ${BUILD_DIR}/core/desktopeditor/cximage/cximage)
add_subdirectory(${SRC_DIR}/core/desktopeditor/fontengine ${BUILD_DIR}/core/desktopeditor/fontengine)
add_subdirectory(${SRC_DIR}/core/desktopeditor/xml ${BUILD_DIR}/core/desktopeditor/xml)
add_subdirectory(${SRC_DIR}/core/desktopeditor/doctrender ${BUILD_DIR}/core/desktopeditor/doctrender)

add_subdirectory(${SRC_DIR}/core/formats/pdfwriter ${BUILD_DIR}/core/formats/pdfwriter)
add_subdirectory(${SRC_DIR}/core/formats/pdfreader ${BUILD_DIR}/core/formats/pdfreader)
add_subdirectory(${SRC_DIR}/core/formats/djvu ${BUILD_DIR}/core/formats/djvu)
add_subdirectory(${SRC_DIR}/core/formats/docxformat ${BUILD_DIR}/core/formats/docxformat)
add_subdirectory(${SRC_DIR}/core/formats/htmlfile ${BUILD_DIR}/core/formats/htmlfile)
add_subdirectory(${SRC_DIR}/core/formats/htmlrender ${BUILD_DIR}/core/formats/htmlrender)
add_subdirectory(${SRC_DIR}/core/formats/xls ${BUILD_DIR}/core/formats/xls)
add_subdirectory(${SRC_DIR}/core/formats/ppt ${BUILD_DIR}/core/formats/ppt)
add_subdirectory(${SRC_DIR}/core/formats/pptx ${BUILD_DIR}/core/formats/pptx)
add_subdirectory(${SRC_DIR}/core/formats/doc ${BUILD_DIR}/core/formats/doc)
add_subdirectory(${SRC_DIR}/core/formats/docx ${BUILD_DIR}/core/formats/docx)
add_subdirectory(${SRC_DIR}/core/formats/rtf ${BUILD_DIR}/core/formats/rtf)
add_subdirectory(${SRC_DIR}/core/formats/odfreader ${BUILD_DIR}/core/formats/odfreader)
add_subdirectory(${SRC_DIR}/core/formats/odfwriter ${BUILD_DIR}/core/formats/odfwriter)
add_subdirectory(${SRC_DIR}/core/formats/txt ${BUILD_DIR}/core/formats/txt)
add_subdirectory(${SRC_DIR}/core/formats/xps ${BUILD_DIR}/core/formats/xps)

add_subdirectory(${SRC_DIR}/core/x2t ${BUILD_DIR}/core/x2t)

# Add target library
add_library(${LIB_NAME_CORE} SHARED wrappers/main.cpp)

# Add dependency library
target_link_libraries(${LIB_NAME_CORE}
    PUBLIC
        ${LIB_NAME_X2T}
    PRIVATE
        # Common
        ${LIB_NAME_CPUFEATURE}
        ${LIB_NAME_OFFICE_UTILS}
        ${LIB_NAME_UNICODE_CONVERTER}
        ${LIB_NAME_CRYPTOPP}
        ${LIB_NAME_POLE}
        ${LIB_NAME_DOWNLOADER}
        ${LIB_NAME_CRYPTO_READER}
        ${LIB_NAME_XLSX_SERIALIZE}
        # Editors
        ${LIB_NAME_FREETYPES}
        ${LIB_NAME_AGG}
        ${LIB_NAME_EDITOR_COMMON}
        ${LIB_NAME_CXIMAGE}
        ${LIB_NAME_FONT_ENGINE}
        ${LIB_NAME_EDITOR_XML}
        ${LIB_NAME_DOCT_RENDER}
        # Formats
        ${LIB_NAME_PDF_WRITER}
        ${LIB_NAME_PDF_READER}
        ${LIB_NAME_DJVU}
        ${LIB_NAME_DOCXFORMAT}
        ${LIB_NAME_HTML_FILE}
        ${LIB_NAME_HTML_RENDER}
        ${LIB_NAME_XLS}
        ${LIB_NAME_DOC}
        ${LIB_NAME_DOCX}
        ${LIB_NAME_PPT}
        ${LIB_NAME_PPTX}
        ${LIB_NAME_RTF}
        ${LIB_NAME_ODF_READER}
        ${LIB_NAME_ODF_WRITER}
        ${LIB_NAME_TXT}
        ${LIB_NAME_XPS}
        # Third party
        ${LIB_NAME_ICU_UC}
        ${LIB_NAME_ICU_DATA}
        ${LIB_NAME_ICU_i18n}
        ${LIB_NAME_BOOST_REGEX}
)

# Set target definition
target_compile_options(${LIB_NAME_CORE}
    PRIVATE
        -Wno-c++11-narrowing
        -Wno-format-security
        -Wno-register
)
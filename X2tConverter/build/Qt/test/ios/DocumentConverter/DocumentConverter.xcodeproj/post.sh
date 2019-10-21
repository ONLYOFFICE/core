#!/bin/sh

dst="${CONFIGURATION_BUILD_DIR}"

# app
install_name_tool \
-change \
libkernel.dylib \
@executable_path/libkernel.dylib \
"${dst}"/${TARGET_NAME}.app/${TARGET_NAME}

install_name_tool \
-change \
libgraphics.dylib \
@executable_path/libgraphics.dylib \
"${dst}"/${TARGET_NAME}.app/${TARGET_NAME}

install_name_tool \
-change \
libUnicodeConverter.dylib \
@executable_path/libUnicodeConverter.dylib \
"${dst}"/${TARGET_NAME}.app/${TARGET_NAME}

install_name_tool \
-change \
libDjVuFile.dylib \
@executable_path/libDjVuFile.dylib \
"${dst}"/${TARGET_NAME}.app/${TARGET_NAME}

install_name_tool \
-change \
libdoctrenderer.dylib \
@executable_path/libdoctrenderer.dylib \
"${dst}"/${TARGET_NAME}.app/${TARGET_NAME}

install_name_tool \
-change \
libHtmlFile.dylib \
@executable_path/libHtmlFile.dylib \
"${dst}"/${TARGET_NAME}.app/${TARGET_NAME}

install_name_tool \
-change \
libHtmlRenderer.dylib \
@executable_path/libHtmlRenderer.dylib \
"${dst}"/${TARGET_NAME}.app/${TARGET_NAME}

install_name_tool \
-change \
libPdfReader.dylib \
@executable_path/libPdfReader.dylib \
"${dst}"/${TARGET_NAME}.app/${TARGET_NAME}

install_name_tool \
-change \
libPdfWriter.dylib \
@executable_path/libPdfWriter.dylib \
"${dst}"/${TARGET_NAME}.app/${TARGET_NAME}

install_name_tool \
-change \
libXpsFile.dylib \
@executable_path/libXpsFile.dylib \
"${dst}"/${TARGET_NAME}.app/${TARGET_NAME}

install_name_tool \
-change \
libx2t.dylib \
@executable_path/libx2t.dylib \
"${dst}"/${TARGET_NAME}.app/${TARGET_NAME}

# graphics
install_name_tool \
-change \
libkernel.dylib \
@executable_path/libkernel.dylib \
"${dst}"/${TARGET_NAME}.app/libgraphics.dylib

install_name_tool \
-change \
libUnicodeConverter.dylib \
@executable_path/libUnicodeConverter.dylib \
"${dst}"/${TARGET_NAME}.app/libgraphics.dylib

# html
install_name_tool \
-change \
libkernel.dylib \
@executable_path/libkernel.dylib \
"${dst}"/${TARGET_NAME}.app/libHtmlFile.dylib

install_name_tool \
-change \
libUnicodeConverter.dylib \
@executable_path/libUnicodeConverter.dylib \
"${dst}"/${TARGET_NAME}.app/libHtmlFile.dylib

# doctrenderer
install_name_tool \
-change \
libkernel.dylib \
@executable_path/libkernel.dylib \
"${dst}"/${TARGET_NAME}.app/libdoctrenderer.dylib

install_name_tool \
-change \
libUnicodeConverter.dylib \
@executable_path/libUnicodeConverter.dylib \
"${dst}"/${TARGET_NAME}.app/libdoctrenderer.dylib

install_name_tool \
-change \
libgraphics.dylib \
@executable_path/libgraphics.dylib \
"${dst}"/${TARGET_NAME}.app/libdoctrenderer.dylib

# htmlrenderer
install_name_tool \
-change \
libkernel.dylib \
@executable_path/libkernel.dylib \
"${dst}"/${TARGET_NAME}.app/libHtmlRenderer.dylib

install_name_tool \
-change \
libUnicodeConverter.dylib \
@executable_path/libUnicodeConverter.dylib \
"${dst}"/${TARGET_NAME}.app/libHtmlRenderer.dylib

install_name_tool \
-change \
libgraphics.dylib \
@executable_path/libgraphics.dylib \
"${dst}"/${TARGET_NAME}.app/libHtmlRenderer.dylib

# pdfwriter
install_name_tool \
-change \
libkernel.dylib \
@executable_path/libkernel.dylib \
"${dst}"/${TARGET_NAME}.app/libPdfWriter.dylib

install_name_tool \
-change \
libUnicodeConverter.dylib \
@executable_path/libUnicodeConverter.dylib \
"${dst}"/${TARGET_NAME}.app/libPdfWriter.dylib

install_name_tool \
-change \
libgraphics.dylib \
@executable_path/libgraphics.dylib \
"${dst}"/${TARGET_NAME}.app/libPdfWriter.dylib

# djvu
install_name_tool \
-change \
libkernel.dylib \
@executable_path/libkernel.dylib \
"${dst}"/${TARGET_NAME}.app/libDjVuFile.dylib

install_name_tool \
-change \
libUnicodeConverter.dylib \
@executable_path/libUnicodeConverter.dylib \
"${dst}"/${TARGET_NAME}.app/libDjVuFile.dylib

install_name_tool \
-change \
libgraphics.dylib \
@executable_path/libgraphics.dylib \
"${dst}"/${TARGET_NAME}.app/libDjVuFile.dylib

install_name_tool \
-change \
libPdfWriter.dylib \
@executable_path/libPdfWriter.dylib \
"${dst}"/${TARGET_NAME}.app/libDjVuFile.dylib

# xps
install_name_tool \
-change \
libkernel.dylib \
@executable_path/libkernel.dylib \
"${dst}"/${TARGET_NAME}.app/libXpsFile.dylib

install_name_tool \
-change \
libUnicodeConverter.dylib \
@executable_path/libUnicodeConverter.dylib \
"${dst}"/${TARGET_NAME}.app/libXpsFile.dylib

install_name_tool \
-change \
libgraphics.dylib \
@executable_path/libgraphics.dylib \
"${dst}"/${TARGET_NAME}.app/libXpsFile.dylib

install_name_tool \
-change \
libPdfWriter.dylib \
@executable_path/libPdfWriter.dylib \
"${dst}"/${TARGET_NAME}.app/libXpsFile.dylib

# pdfreader
install_name_tool \
-change \
libkernel.dylib \
@executable_path/libkernel.dylib \
"${dst}"/${TARGET_NAME}.app/libPdfReader.dylib

install_name_tool \
-change \
libUnicodeConverter.dylib \
@executable_path/libUnicodeConverter.dylib \
"${dst}"/${TARGET_NAME}.app/libPdfReader.dylib

install_name_tool \
-change \
libgraphics.dylib \
@executable_path/libgraphics.dylib \
"${dst}"/${TARGET_NAME}.app/libPdfReader.dylib

install_name_tool \
-change \
libHtmlRenderer.dylib \
@executable_path/libHtmlRenderer.dylib \
"${dst}"/${TARGET_NAME}.app/libPdfReader.dylib

# x2t
install_name_tool \
-change \
libkernel.dylib \
@executable_path/libkernel.dylib \
"${dst}"/${TARGET_NAME}.app/libx2t.dylib

install_name_tool \
-change \
libgraphics.dylib \
@executable_path/libgraphics.dylib \
"${dst}"/${TARGET_NAME}.app/libx2t.dylib

install_name_tool \
-change \
libUnicodeConverter.dylib \
@executable_path/libUnicodeConverter.dylib \
"${dst}"/${TARGET_NAME}.app/libx2t.dylib

install_name_tool \
-change \
libDjVuFile.dylib \
@executable_path/libDjVuFile.dylib \
"${dst}"/${TARGET_NAME}.app/libx2t.dylib

install_name_tool \
-change \
libdoctrenderer.dylib \
@executable_path/libdoctrenderer.dylib \
"${dst}"/${TARGET_NAME}.app/libx2t.dylib

install_name_tool \
-change \
libHtmlFile.dylib \
@executable_path/libHtmlFile.dylib \
"${dst}"/${TARGET_NAME}.app/libx2t.dylib

install_name_tool \
-change \
libHtmlRenderer.dylib \
@executable_path/libHtmlRenderer.dylib \
"${dst}"/${TARGET_NAME}.app/libx2t.dylib

install_name_tool \
-change \
libPdfReader.dylib \
@executable_path/libPdfReader.dylib \
"${dst}"/${TARGET_NAME}.app/libx2t.dylib

install_name_tool \
-change \
libPdfWriter.dylib \
@executable_path/libPdfWriter.dylib \
"${dst}"/${TARGET_NAME}.app/libx2t.dylib

install_name_tool \
-change \
libXpsFile.dylib \
@executable_path/libXpsFile.dylib \
"${dst}"/${TARGET_NAME}.app/libx2t.dylib


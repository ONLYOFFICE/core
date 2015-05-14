#-------------------------------------------------
#
# Project created by QtCreator 2015-02-17T12:38:00
#
#-------------------------------------------------

QT       -= core gui

TARGET =  PdfWriterLib
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers

DEFINES += UNICODE \
           _UNICODE \
            LINUX \
           _LINUX_QT \
           _USE_LIBXML2_READER_ \
           _USE_XMLLITE_READER_ \
            USE_LITE_READER \
            BUILD_CONFIG_FULL_VERSION \
            FILTER_FLATE_DECODE_ENABLED \
            DONT_WRITE_EMBEDDED_FONTS

INCLUDEPATH += \
    ../../../DesktopEditor/agg-2.4/include \
    ../../../DesktopEditor/freetype-2.5.2/include \
    ../../../DesktopEditor/cximage/zlib

LIBS += -lxml2

INCLUDEPATH += \
    /usr/include/libxml2

SOURCES += \
    ../PdfWriterRenderer.cpp \
    ../ToUnicode.cpp \
    ../Encoder.cpp \
    ../FontTrueType.cpp \
    ../Objects.cpp \
    ../Encrypt.cpp \
    ../Image.cpp \
    ../GraphicState.cpp \
    ../Annotation.cpp \
    ../Font.cpp \
    ../PdfWriterLib.cpp \
    ../Common.cpp \
    ../PageOperations.cpp \
    ../Outline.cpp \
    ../List.cpp \
    ../EncryptDictionary.cpp \
    ../Pages.cpp \
    ../FontTypes.cpp \
    ../Destination.cpp \
    ../Document.cpp \
    ../MemoryManager.cpp \
    ../FontBase.cpp \
    ../FontCID.cpp \
    ../MetaData.cpp \
    ../Streams.cpp \
    ../Catalog.cpp \
    ../PageLabel.cpp \
    ../Utils.cpp \
    ../OnlineOfficeBinToPdf.cpp \
    ../../../Common/DocxFormat/Source/XML/stringcommon.cpp \
    ../../../Common/DocxFormat/Source/SystemUtility/FileSystem/DirectoryPosix.cpp \
    ../../../Common/3dParty/pole/pole.cpp \
    ../../../Common/DocxFormat/Source/XML/libxml2/libxml2.cpp \
    ../../../Common/DocxFormat/Source/Base/unicode_util.cpp \
    ../../../ASCOfficePPTXFile/Editor/FontPicker.cpp \
    ../../../DesktopEditor/fontengine/ApplicationFonts.cpp \
    ../../../DesktopEditor/fontengine/FontManager.cpp \
    ../../../DesktopEditor/raster/JBig2/source/JBig2File.cpp \
    ../../../DesktopEditor/raster/JBig2/source/Encoder/jbig2arith.cpp \
    ../../../DesktopEditor/raster/JBig2/source/Encoder/jbig2enc.cpp \
    ../../../DesktopEditor/raster/JBig2/source/Encoder/jbig2sym.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/adaptmap.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/affine.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/affinecompose.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/arithlow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/arrayaccess.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/bardecode.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/baseline.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/bbuffer.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/bilinear.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/binarize.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/binexpand.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/binexpandlow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/binreduce.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/binreducelow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/blend.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/bmf.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/bmpio.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/bmpiostub.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/boxbasic.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/boxfunc1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/boxfunc2.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/boxfunc3.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/ccbord.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/ccthin.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/classapp.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/colorcontent.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/colormap.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/colormorph.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/colorquant1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/colorquant2.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/colorseg.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/compare.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/conncomp.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/convertfiles.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/convolve.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/convolvelow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/correlscore.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/dewarp.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/dwacomb.2.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/dwacomblow.2.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/edge.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/enhance.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/fhmtauto.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/fhmtgen.1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/fhmtgenlow.1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/finditalic.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/flipdetect.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/fliphmtgen.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/fmorphauto.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/fmorphgen.1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/fmorphgenlow.1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/fpix1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/fpix2.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/freetype.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/gifio.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/gifiostub.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/gplot.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/graphics.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/graymorph.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/graymorphlow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/grayquant.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/grayquantlow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/heap.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/jbclass.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/jpegio.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/jpegiostub.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/kernel.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/list.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/maze.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/morph.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/morphapp.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/morphdwa.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/morphseq.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/numabasic.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/numafunc1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/numafunc2.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pageseg.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/paintcmap.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/parseprotos.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/partition.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pix1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pix2.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pix3.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pix4.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pix5.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pixabasic.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pixacc.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pixafunc1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pixafunc2.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pixalloc.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pixarith.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pixcomp.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pixconv.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pixtiling.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pngio.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pngiostub.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pnmio.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pnmiostub.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/projective.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/psio1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/psio1stub.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/psio2.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/psio2stub.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/ptabasic.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/ptafunc1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/ptra.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/queue.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/rank.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/readbarcode.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/readfile.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/regutils.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/rop.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/ropiplow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/roplow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/rotate.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/rotateam.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/rotateamlow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/rotateorth.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/rotateorthlow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/rotateshear.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/runlength.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/sarray.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/scale.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/scalelow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/seedfill.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/seedfilllow.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/sel1.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/sel2.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/selgen.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/shear.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/skew.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/spixio.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/stack.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/sudoku.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/textops.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/tiffio.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/tiffiostub.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/utils.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/viewfiles.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/warper.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/watershed.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/webpio.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/webpiostub.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/writefile.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/zlibmem.cpp \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/zlibmemstub.cpp

HEADERS += \
    ../Catalog.h \
    ../FontTrueTypeWriter.h \
    ../Encoder.h \
    ../Document.h \
    ../Pattern.h \
    ../Encrypt.h \
    ../Image.h \
    ../GraphicState.h \
    ../MetaData.h \
    ../Streams.h \
    ../Annotation.h \
    ../Errors.h \
    ../Font.h \
    ../Utils.h \
    ../ToUnicode.h \
    ../Common.h \
    ../FontType1.h \
    ../PageOperations.h \
    ../EncryptDictionary.h \
    ../FontBase14.h \
    ../ImageExt.h \
    ../FontCID.h \
    ../PageLabel.h \
    ../PDF.h \
    ../PdfWriterLib.h \
    ../FontTypes.h \
    ../FontTrueType.h \
    ../Pages.h \
    ../Objects.h \
    ../LZWEncoder.h \
    ../Destination.h \
    ../Outline.h \
    ../List.h \
    ../MemoryManager.h \
    ../FontBase.h \
    ../OnlineOfficeBinToPdf.h \
    ../EncoderConstants.h \
    ../Consts.h \
    ../Types.h \
    ../../../DesktopEditor/raster/JBig2/source/JBig2File.h \
    ../../../DesktopEditor/raster/JBig2/source/Utils.h \
    ../../../DesktopEditor/raster/JBig2/source/Encoder/jbig2arith.h \
    ../../../DesktopEditor/raster/JBig2/source/Encoder/jbig2enc.h \
    ../../../DesktopEditor/raster/JBig2/source/Encoder/jbig2encoder.h \
    ../../../DesktopEditor/raster/JBig2/source/Encoder/jbig2segments.h \
    ../../../DesktopEditor/raster/JBig2/source/Encoder/jbig2structs.h \
    ../../../DesktopEditor/raster/JBig2/source/Encoder/jbig2sym.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/additionaltypes.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/allheaders.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/alltypes.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/array.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/arrayaccess.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/bbuffer.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/bmf.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/bmp.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/ccbord.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/dewarp.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/endianness.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/environ.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/freetype.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/funcprotos.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/gplot.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/heap.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/imageio.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/jbclass.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/list.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/morph.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/pix.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/ptra.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/queue.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/readbarcode.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/regutils.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/stack.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/sudoku.h \
    ../../../DesktopEditor/raster/JBig2/source/LeptonLib/watershed.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

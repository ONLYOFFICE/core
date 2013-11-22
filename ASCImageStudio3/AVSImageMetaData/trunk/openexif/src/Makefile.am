
AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = libopenexif.la

openexif_jpeg_srcs = \
	OpenExifJpeg/OpenExif_jcapimin.cpp  OpenExifJpeg/OpenExif_jdapimin.cpp  OpenExifJpeg/OpenExif_jdsample.cpp \
	OpenExifJpeg/OpenExif_jcapistd.cpp  OpenExifJpeg/OpenExif_jdapistd.cpp  OpenExifJpeg/OpenExif_jerror.cpp \
	OpenExifJpeg/OpenExif_jccoefct.cpp  OpenExifJpeg/OpenExif_jdatadst.cpp  OpenExifJpeg/OpenExif_jfdctflt.cpp \
	OpenExifJpeg/OpenExif_jccolor.cpp   OpenExifJpeg/OpenExif_jdatasrc.cpp  OpenExifJpeg/OpenExif_jfdctfst.cpp \
	OpenExifJpeg/OpenExif_jcdctmgr.cpp  OpenExifJpeg/OpenExif_jdcoefct.cpp  OpenExifJpeg/OpenExif_jfdctint.cpp \
	OpenExifJpeg/OpenExif_jchuff.cpp    OpenExifJpeg/OpenExif_jdcolor.cpp   OpenExifJpeg/OpenExif_jidctflt.cpp \
	OpenExifJpeg/OpenExif_jcinit.cpp    OpenExifJpeg/OpenExif_jddctmgr.cpp  OpenExifJpeg/OpenExif_jidctfst.cpp \
	OpenExifJpeg/OpenExif_jcmainct.cpp  OpenExifJpeg/OpenExif_jdhuff.cpp    OpenExifJpeg/OpenExif_jidctint.cpp \
	OpenExifJpeg/OpenExif_jcmarker.cpp  OpenExifJpeg/OpenExif_jdinput.cpp   OpenExifJpeg/OpenExif_jidctred.cpp \
	OpenExifJpeg/OpenExif_jcmaster.cpp  OpenExifJpeg/OpenExif_jdmainct.cpp  OpenExifJpeg/OpenExif_jmemansi.cpp \
	OpenExifJpeg/OpenExif_jcomapi.cpp   OpenExifJpeg/OpenExif_jdmarker.cpp  OpenExifJpeg/OpenExif_jmemmgr.cpp \
	OpenExifJpeg/OpenExif_jcparam.cpp   OpenExifJpeg/OpenExif_jdmaster.cpp  OpenExifJpeg/OpenExif_jquant1.cpp \
	OpenExifJpeg/OpenExif_jcphuff.cpp   OpenExifJpeg/OpenExif_jdmerge.cpp   OpenExifJpeg/OpenExif_jquant2.cpp \
	OpenExifJpeg/OpenExif_jcprepct.cpp  OpenExifJpeg/OpenExif_jdphuff.cpp   OpenExifJpeg/OpenExif_jutils.cpp \
	OpenExifJpeg/OpenExif_jcsample.cpp  OpenExifJpeg/OpenExif_jdpostct.cpp

libopenexif_la_SOURCES = \
	ExifApp0Seg.cpp \
	ExifAppSegManager.cpp \
	ExifAppSegment.cpp \
	ExifFileIO.cpp \
	ExifHuffmanTable.cpp \
	ExifIFD.cpp \
	ExifIJGWrapper.cpp \
	ExifIJGWriteFrame.c \
	ExifImageDescUtils.cpp  \
	ExifImageFile.cpp \
	ExifImageFileRead.cpp \
	ExifImageFileWrite.cpp \
	ExifInternetIO.cpp  \
	ExifIO.cpp \
	ExifJpegDataDst.cpp \
	ExifJpegDataSrc.cpp \
	ExifJpegImage.cpp \
	ExifMemoryIO.cpp \
	ExifMisc.cpp \
	ExifOpenFile.cpp \
	ExifRawAppSeg.cpp \
	ExifStripImage.cpp \
	ExifTiffAppSeg.cpp \
	$(openexif_jpeg_srcs)

include_HEADERS = \
	ExifApp0Seg.h \
	ExifAppSegManager.h \
	ExifAppSegment.h \
	ExifComMarker.h \
	ExifComp.h \
	ExifConf.h \
	ExifDefs.h \
	ExifErrorCodes.h \
	ExifFileIO.h \
	ExifHuffmanTable.h \
	ExifIFD.h \
	ExifIJGWrapper.h \
	ExifIJGWriteFrame.h \
	ExifImageDescUtils.h \
	ExifImageFile.h \
	ExifImageFileIO.h \
	ExifInternetIO.h \
	ExifIO.h \
	ExifJpegExtn.h \
	ExifJpegImage.h \
	ExifJpegTables.h \
	ExifMemoryIO.h \
	ExifMisc.h \
	ExifOpenFile.h \
	ExifRawAppSeg.h \
	ExifStripImage.h \
	ExifTagEntry.h \
	ExifTags.h \
	ExifTiffAppSeg.h \
	ExifTypeDefs.h 

INCLUDES = -I. -IOpenExifJpeg 

clean-compile:
	-rm -f $(OBJECTS)

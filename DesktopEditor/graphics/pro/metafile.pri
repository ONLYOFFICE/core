METAFILE_PATH = $$PWD/../../raster/Metafile

!metafile_disable_wmf_emf {
    DEFINES += METAFILE_SUPPORT_WMF_EMF

    HEADERS += \
	    $$METAFILE_PATH/Common/CPathConverter.h \
		\
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretator.h \
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorArray.h \
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorBase.h \
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorRender.h \
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorSvg.h \
		$$METAFILE_PATH/Emf/EmfParser/CEmfParser.h \
		$$METAFILE_PATH/Emf/EmfParser/CEmfParserBase.h \
		$$METAFILE_PATH/Emf/EmfParser/CEmfPlusParser.h \
		\
		$$METAFILE_PATH/Wmf/WmfInterpretator/CWmfInterpretator.h \
		$$METAFILE_PATH/Wmf/WmfInterpretator/CWmfInterpretatorBase.h \
		$$METAFILE_PATH/Wmf/WmfInterpretator/CInterpretatorSvgBase.h \
		$$METAFILE_PATH/Wmf/WmfInterpretator/CWmfInterpretatorSvg.h \
		$$METAFILE_PATH/Wmf/WmfInterpretator/CWmfInterpretatorRender.h \
		$$METAFILE_PATH/Wmf/WmfParser/CWmfParser.h \
		$$METAFILE_PATH/Wmf/WmfParser/CWmfParserBase.h

    SOURCES += \
	    $$METAFILE_PATH/Common/CPathConverter.cpp \
		\
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretator.cpp \
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorArray.cpp \
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorRender.cpp \
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorSvg.cpp \
		$$METAFILE_PATH/Emf/EmfParser/CEmfParser.cpp \
		$$METAFILE_PATH/Emf/EmfParser/CEmfParserBase.cpp \
		$$METAFILE_PATH/Emf/EmfParser/CEmfPlusParser.cpp \
		\
		$$METAFILE_PATH/Emf/EmfClip.cpp \
		$$METAFILE_PATH/Emf/EmfObjects.cpp \
		$$METAFILE_PATH/Emf/EmfPath.cpp \
		$$METAFILE_PATH/Emf/EmfPlayer.cpp \
		$$METAFILE_PATH/Emf/EmfFile.cpp \
		$$METAFILE_PATH/Wmf/WmfClip.cpp \
		$$METAFILE_PATH/Wmf/WmfObjects.cpp \
		$$METAFILE_PATH/Wmf/WmfPlayer.cpp \
		\
		$$METAFILE_PATH/Wmf/WmfInterpretator/CInterpretatorSvgBase.cpp \
		$$METAFILE_PATH/Wmf/WmfInterpretator/CWmfInterpretatorSvg.cpp \
		$$METAFILE_PATH/Wmf/WmfInterpretator/CWmfInterpretatorRender.cpp \
		$$METAFILE_PATH/Wmf/WmfInterpretator/CWmfInterpretator.cpp \
		$$METAFILE_PATH/Wmf/WmfParser/CWmfParser.cpp \
		$$METAFILE_PATH/Wmf/WmfParser/CWmfParserBase.cpp

    !metafile_disable_wmf_emf_xml {
	    DEFINES += METAFILE_SUPPORT_WMF_EMF_XML
		HEADERS += \
		    $$METAFILE_PATH/CXmlOutput.h \
			\
			$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorXml.h \
			$$METAFILE_PATH/Emf/EmfParser/CEmfxParser.h

        SOURCES += \
		    $$METAFILE_PATH/CXmlOutput.cpp \
			\
			$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorXml.cpp \
			$$METAFILE_PATH/Emf/EmfParser/CEmfxParser.cpp
	}
}

!metafile_disable_svg {
    DEFINES += METAFILE_SUPPORT_SVG

    HEADERS += \
	    $$METAFILE_PATH/svg/SVGFramework.h \
		$$METAFILE_PATH/svg/SVGTransformer.h

    SOURCES += 	\
	    $$METAFILE_PATH/svg/SVGFramework.cpp \
		$$METAFILE_PATH/svg/SVGTransformer.cpp
}

!metafile_disable_svm {
    DEFINES += METAFILE_SUPPORT_SVM

    SOURCES += \
	    $$METAFILE_PATH/StarView/SvmClip.cpp \
		$$METAFILE_PATH/StarView/SvmFile.cpp \
		$$METAFILE_PATH/StarView/SvmObjects.cpp \
		$$METAFILE_PATH/StarView/SvmPlayer.cpp
}

HEADERS += \
    $$METAFILE_PATH/MetaFile.h \
	$$METAFILE_PATH/Common/MetaFile.h \
	$$METAFILE_PATH/Common/IOutputDevice.h \
	$$METAFILE_PATH/Common/MetaFileTypes.h \
	$$METAFILE_PATH/Common/MetaFileClip.h \
	$$METAFILE_PATH/Common/MetaFileObjects.h \
	$$METAFILE_PATH/Common/MetaFileRenderer.h \
	$$METAFILE_PATH/Common/MetaFileUtils.h

SOURCES += \
    $$METAFILE_PATH/MetaFile.cpp \
	$$METAFILE_PATH/Common/MetaFileTypes.cpp \
	$$METAFILE_PATH/Common/MetaFileUtils.cpp

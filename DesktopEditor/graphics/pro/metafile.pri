METAFILE_PATH = $$PWD/../../raster/Metafile

!metafile_disable_wmf_emf {
	DEFINES += METAFILE_SUPPORT_WMF_EMF

	HEADERS += \
		$$METAFILE_PATH/Common/CPathConverter.h \
		$$METAFILE_PATH/Common/CPath.h \
		$$METAFILE_PATH/Common/CClip.h \
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
		$$METAFILE_PATH/Emf/EmfTypes.h \
		$$METAFILE_PATH/Emf/EmfObjects.h \
		$$METAFILE_PATH/Emf/EmfPlayer.h \
		$$METAFILE_PATH/Emf/EmfFile.h \
		$$METAFILE_PATH/Wmf/WmfObjects.h \
		$$METAFILE_PATH/Wmf/WmfPlayer.h \
		$$METAFILE_PATH/Wmf/WmfTypes.h \
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
		$$METAFILE_PATH/Common/CPath.cpp \
		$$METAFILE_PATH/Common/CClip.cpp \
		\
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretator.cpp \
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorArray.cpp \
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorRender.cpp \
		$$METAFILE_PATH/Emf/EmfInterpretator/CEmfInterpretatorSvg.cpp \
		$$METAFILE_PATH/Emf/EmfParser/CEmfParser.cpp \
		$$METAFILE_PATH/Emf/EmfParser/CEmfParserBase.cpp \
		$$METAFILE_PATH/Emf/EmfParser/CEmfPlusParser.cpp \
		\
		$$METAFILE_PATH/Emf/EmfObjects.cpp \
		$$METAFILE_PATH/Emf/EmfPlusObjects.cpp \
		$$METAFILE_PATH/Emf/EmfPlayer.cpp \
		$$METAFILE_PATH/Emf/EmfFile.cpp \
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

	# DEPRECATED ENGINE. REMOVE IN 7.6+ VERSIONS
	#CONFIG += svg_old_version

	svg_old_version {

		DEFINES += SVG_OLD_ENGINE

		HEADERS += \
			$$METAFILE_PATH/svg/SVGFramework.h \
			$$METAFILE_PATH/svg/SVGTransformer.h

		SOURCES += \
			$$METAFILE_PATH/svg/SVGFramework.cpp \
			$$METAFILE_PATH/svg/SVGTransformer.cpp

	} else {

		HEADERS += \
			$$METAFILE_PATH/svg/SvgTypes.h \
			$$METAFILE_PATH/svg/CSvgFile.h \
			$$METAFILE_PATH/svg/CSvgParser.h \
			$$METAFILE_PATH/svg/SvgObjects/CContainer.h \
			$$METAFILE_PATH/svg/SvgObjects/CGradient.h \
			$$METAFILE_PATH/svg/SvgObjects/CClipPath.h \
			$$METAFILE_PATH/svg/SvgObjects/CMask.h \
			$$METAFILE_PATH/svg/SvgObjects/CPattern.h \
			$$METAFILE_PATH/svg/SvgObjects/CSymbol.h \
			$$METAFILE_PATH/svg/SvgObjects/CSwitch.h \
			$$METAFILE_PATH/svg/SvgObjects/CMarker.h \
			$$METAFILE_PATH/svg/SvgObjects/CImage.h \
			$$METAFILE_PATH/svg/SvgObjects/CLine.h \
			$$METAFILE_PATH/svg/SvgObjects/CRect.h \
			$$METAFILE_PATH/svg/SvgObjects/CCircle.h \
			$$METAFILE_PATH/svg/SvgObjects/CEllipse.h \
			$$METAFILE_PATH/svg/SvgObjects/CPath.h \
			$$METAFILE_PATH/svg/SvgObjects/CText.h \
			$$METAFILE_PATH/svg/SvgObjects/CUse.h \
			$$METAFILE_PATH/svg/SvgObjects/CPolyline.h \
			$$METAFILE_PATH/svg/SvgObjects/CStyle.h \
			$$METAFILE_PATH/svg/SvgObjects/CObjectBase.h \
			$$METAFILE_PATH/svg/SvgUtils.h

		SOURCES += 	\
			$$METAFILE_PATH/svg/CSvgFile.cpp \
			$$METAFILE_PATH/svg/CSvgParser.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CContainer.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CGradient.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CClipPath.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CMask.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CMarker.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CPattern.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CSymbol.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CSwitch.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CImage.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CLine.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CRect.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CCircle.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CEllipse.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CPath.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CText.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CUse.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CPolyline.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CObjectBase.cpp \
			$$METAFILE_PATH/svg/SvgObjects/CStyle.cpp

		CONFIG += css_calculator_without_xhtml

		include($$METAFILE_PATH/../../../Common/3dParty/html/css/CssCalculator.pri)
	}
}

!metafile_disable_svm {
	DEFINES += METAFILE_SUPPORT_SVM

	HEADERS += \
		$$METAFILE_PATH/StarView/SvmFile.h \
		$$METAFILE_PATH/StarView/SvmObjects.h \
		$$METAFILE_PATH/StarView/SvmPlayer.h

	SOURCES += \
		$$METAFILE_PATH/StarView/SvmFile.cpp \
		$$METAFILE_PATH/StarView/SvmObjects.cpp \
		$$METAFILE_PATH/StarView/SvmPlayer.cpp
}

HEADERS += \
	$$METAFILE_PATH/MetaFile.h \
	$$METAFILE_PATH/Common/MetaFile.h \
	$$METAFILE_PATH/Common/IOutputDevice.h \
	$$METAFILE_PATH/Common/MetaFileTypes.h \
	$$METAFILE_PATH/Common/MetaFileObjects.h \
	$$METAFILE_PATH/Common/MetaFileRenderer.h \
	$$METAFILE_PATH/Common/MetaFileUtils.h

SOURCES += \
	$$METAFILE_PATH/MetaFile.cpp \
	$$METAFILE_PATH/Common/MetaFileTypes.cpp \
	$$METAFILE_PATH/Common/MetaFileUtils.cpp

// CASCPresentationEditor.h : Declaration of the CASCOfficeViewer
#pragma once

#ifdef PPT_DEF
    #include "../../ASCPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTShape/Formula.cpp"
    #include "../../ASCPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTShape/PPTShape.cpp"
	#ifndef BASE_SHAPE
		#define BASE_SHAPE
	#endif
#endif
#ifdef PPTX_DEF
    #include "../../ASCPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTXShape/Formula.cpp"
    #include "../../ASCPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTXShape/PPTXShape.cpp"
	#ifndef BASE_SHAPE
		#define BASE_SHAPE
	#endif

    #include "../../ASCPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTShape/ppt2pptxshapeconverter.h"

#endif

#ifdef BASE_SHAPE
    #include "./OfficeDrawing/Shapes/BaseShape/BaseShape.cpp"
#endif

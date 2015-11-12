// CASCPresentationEditor.h : Declaration of the CASCOfficeViewer
#pragma once

#ifdef PPT_DEF
    #include "./Drawing/Shapes/BaseShape/PPTShape/Formula.cpp"
    #include "./Drawing/Shapes/BaseShape/PPTShape/PPTShape.cpp"
	#ifndef BASE_SHAPE
		#define BASE_SHAPE
	#endif
#endif
#ifdef PPTX_DEF
    #include "./Drawing/Shapes/BaseShape/PPTXShape/Formula.cpp"
    #include "./Drawing/Shapes/BaseShape/PPTXShape/PPTXShape.cpp"
	#ifndef BASE_SHAPE
		#define BASE_SHAPE
	#endif

    #include "./Drawing/Shapes/BaseShape/PPTShape/ppt2pptxshapeconverter.h"

#endif

#ifdef BASE_SHAPE
    #include "./Drawing/Shapes/BaseShape/BaseShape.cpp"
#endif

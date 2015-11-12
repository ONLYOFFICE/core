// CASCPresentationEditor.h : Declaration of the CASCOfficeViewer
#pragma once

#ifdef PPT_DEF
    #include "./Shapes/BaseShape/PPTShape/Formula.cpp"
    #include "./Shapes/BaseShape/PPTShape/PPTShape.cpp"
	#ifndef BASE_SHAPE
		#define BASE_SHAPE
	#endif
#endif
#ifdef PPTX_DEF
    #include "./Shapes/BaseShape/PPTXShape/Formula.cpp"
    #include "./Shapes/BaseShape/PPTXShape/PPTXShape.cpp"
	#ifndef BASE_SHAPE
		#define BASE_SHAPE
	#endif

    #include "./Shapes/BaseShape/PPTShape/ppt2pptxshapeconverter.h"

#endif

#ifdef BASE_SHAPE
    #include "./Shapes/BaseShape/BaseShape.cpp"
#endif

#pragma once
#ifndef XPS_DRAWPAGEPROPERTIES_INCLUDE_H_
#define XPS_DRAWPAGEPROPERTIES_INCLUDE_H_

#import "..\..\..\..\..\..\..\Redist\AVSGraphics.dll"									named_guids raw_interfaces_only rename_namespace("AVSGraphics")
#include "..\..\..\..\..\..\AVSImageStudio3\AVSGraphics\agg\AVSWrapper\Matrix.h"
#include <list>

namespace XPS
{
	class DrawPageProperties
	{
	public:
		AVSGraphics::IAVSFontManager* m_FontManager;

		Aggplus::CMatrix m_CurrentTransform;
		std::list<Aggplus::CMatrix> m_TransformStack;

		CAtlMap<CString, CString> m_mapFigures;

	public:
		DrawPageProperties();
		~DrawPageProperties();
	};
} // namespace XPS

#endif //XPS_DRAWPAGEPROPERTIES_INCLUDE_H_
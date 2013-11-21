#pragma once
#ifndef XPS_PAGE_INCLUDE_H_
#define XPS_PAGE_INCLUDE_H_

#include "..\..\..\..\..\..\AVSImageStudio3\AVSGraphics\Interfaces\XmlUtils.h"
#include <set>
#include <boost/shared_ptr.hpp>
#include "Utils.h"
#include "DrawPageProperties.h"

namespace XPS
{
	class Page
	{
	private:
		CString m_strFilename;
		CString m_strPath;
		boost::shared_ptr<std::set<CString> > m_fontMap;

		void DrawCanvas(XmlUtils::CXmlNode& canvas, AVSGraphics::IAVSRenderer* pRenderer, DrawPageProperties* pProps, BOOL* pBreak)const;
		void DrawGlyph(XmlUtils::CXmlNode& node, AVSGraphics::IAVSRenderer* pRenderer, DrawPageProperties* pProps)const;
		void DrawPath(XmlUtils::CXmlNode& node, AVSGraphics::IAVSRenderer* pRenderer, DrawPageProperties* pProps)const;
		void CanvasTransform(XmlUtils::CXmlNode& node, AVSGraphics::IAVSRenderer* pRenderer, DrawPageProperties* pProps)const;
		void FillToRenderer(XmlUtils::CXmlNode& node, AVSGraphics::IAVSRenderer* pRenderer)const;

		static bool VmlToRenderer(CString& str, AVSGraphics::IAVSRenderer* pRenderer);
		static void GetDataFromNode(CString& str, XmlUtils::CXmlNode& node);
		void TransformToRenderer(CString& str, AVSGraphics::IAVSRenderer* pRenderer, DrawPageProperties* pProps)const;
		void ResetTransform(AVSGraphics::IAVSRenderer* pRenderer, DrawPageProperties* pProps)const;

		CRITICAL_SECTION* m_pFontCS;

	public:
		Page(const CString& File, const CString& Path, boost::shared_ptr<std::set<CString> > fontMap, CRITICAL_SECTION* fontCS);
		~Page();

		void GetSize(int& w, int& h)const;
		void Draw(AVSGraphics::IAVSRenderer* pRenderer, BOOL* pBreak)const;
	};
} // namespace XPS

#endif //XPS_PAGE_INCLUDE_H_
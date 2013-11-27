#pragma once
#ifndef XPS_PAGE_INCLUDE_H_
#define XPS_PAGE_INCLUDE_H_

#include "../../../Common/XmlUtils.h"
#include "../../../Common/DocxFormat/Source/Base/SmartPtr.h";
#include "Utils.h"
#include "DrawPageProperties.h"

namespace XPS
{
	class Page
	{
	private:
		CString m_strFilename;
		CString m_strPath;
		mutable NSCommon::smart_ptr<CAtlMap<CString, bool>> m_fontMap;

		void DrawCanvas(XmlUtils::CXmlNode& canvas, ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps, BOOL* pBreak)const;
		void DrawGlyph(XmlUtils::CXmlNode& node, ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps)const;
		void DrawPath(XmlUtils::CXmlNode& node, ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps)const;
		void CanvasTransform(XmlUtils::CXmlNode& node, ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps)const;
		void FillToRenderer(XmlUtils::CXmlNode& node, ASCGraphics::IASCRenderer* pRenderer)const;

		static bool VmlToRenderer(CString& str, ASCGraphics::IASCRenderer* pRenderer);
		static void GetDataFromNode(CString& str, XmlUtils::CXmlNode& node);
		void TransformToRenderer(CString& str, ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps)const;
		void ResetTransform(ASCGraphics::IASCRenderer* pRenderer, DrawPageProperties* pProps)const;

		CRITICAL_SECTION* m_pFontCS;

	public:
		Page(const CString& File, const CString& Path, NSCommon::smart_ptr<CAtlMap<CString, bool>> fontMap, CRITICAL_SECTION* fontCS);
		~Page();

		void GetSize(int& w, int& h)const;
		void Draw(ASCGraphics::IASCRenderer* pRenderer, BOOL* pBreak)const;
	};
} // namespace XPS

#endif //XPS_PAGE_INCLUDE_H_
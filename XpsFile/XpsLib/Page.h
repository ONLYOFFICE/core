#ifndef _XPS_XPSLIB_PAGE_H
#define _XPS_XPSLIB_PAGE_H

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../DesktopEditor/fontengine/FontManager.h"

#include "FontList.h"
#include "Utils.h"
#include "ContextState.h"

namespace XPS
{
	class CDocument;
	class CStaticResource;

	class Page
	{
	public:
		Page(const std::wstring& wsFile, const std::wstring& Path, CFontList* pFontList, CFontManager* pFontManager, CDocument* pDocument);
		~Page();

		void GetSize(int& nW, int& nH) const;
		void Draw(IRenderer* pRenderer, bool* pbBreak);

	private:

		void DrawCanvas      (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState, bool* pbBreak);
		bool ReadResource    (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void DrawGlyph       (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void DrawPath        (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		bool StrokeToRenderer(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void ReadPathData    (XmlUtils::CXmlLiteReader& oReader, CWString& wsData, CWString& wsTranform);
		
		bool ClipToRenderer     (const wchar_t* wsString, CContextState* pState);
		bool TransformToRenderer(const wchar_t* wsString, CContextState* pState);

	private:

		std::wstring     m_wsPagePath;
		std::wstring     m_wsRootPath;
		CFontList*       m_pFontList;
		CFontManager*    m_pFontManager;
		CDocument*       m_pDocument;
	};
}

#endif // _XPS_XPSLIB_PAGE_H

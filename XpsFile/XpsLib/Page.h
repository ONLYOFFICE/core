#ifndef _XPS_XPSLIB_PAGE_H
#define _XPS_XPSLIB_PAGE_H

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../Common/DocxFormat/Source/XML/xmlutils.h"
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

		void DrawCanvas       (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState, bool* pbBreak);
		void ReadPageResources(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void DrawGlyph        (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		bool ReadTransform    (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void DrawPath         (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		bool FillToRenderer   (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer);
		void ReadPathData     (XmlUtils::CXmlLiteReader& oReader, std::wstring& wsData);
		void ReadPathGeometry (XmlUtils::CXmlLiteReader& oReader, std::wstring& wsData);
		void ReadPathFigure   (XmlUtils::CXmlLiteReader& oReader, std::wstring& wsData);
		
		bool ClipToRenderer     (const wchar_t* wsString, CContextState* pState);
		bool TransformToRenderer(const wchar_t* wsString, CContextState* pState);

		CWString ReadMatrixTransform(XmlUtils::CXmlLiteReader& oReader);
		CWString ReadClip           (XmlUtils::CXmlLiteReader& oReader);

	private:

		std::wstring     m_wsPagePath;
		std::wstring     m_wsRootPath;
		CFontList*       m_pFontList;
		CFontManager*    m_pFontManager;
		CDocument*       m_pDocument;
		CStaticResource* m_pStaticResource;
		bool             m_bDeleteStaticResource;

		int m_nCounter;
	};
}

#endif // _XPS_XPSLIB_PAGE_H
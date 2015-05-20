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
	class Page
	{
	public:
		Page(const std::wstring& wsFile, const std::wstring& Path, CFontList* pFontList, CFontManager* pFontManager);
		~Page();

		void GetSize(int& nW, int& nH) const;
		void Draw(IRenderer* pRenderer, bool* pbBreak);

	private:

		void DrawCanvas(XmlUtils::CXmlNode& oNode, IRenderer* pRenderer, CContextState* pState, bool* pbBreak);
		void DrawGlyph(XmlUtils::CXmlNode& oNode, IRenderer* pRenderer, CContextState* pState);
		void DrawPath(XmlUtils::CXmlNode& oNode, IRenderer* pRenderer, CContextState* pState);
		void CanvasTransform(XmlUtils::CXmlNode& oNode, IRenderer* pRenderer, CContextState* pState);
		void FillToRenderer(XmlUtils::CXmlNode& oNode, IRenderer* pRenderer);

		bool VmlToRenderer(std::wstring& wsValue, IRenderer* pRenderer);
		void GetDataFromNode(std::wstring& wsString, XmlUtils::CXmlNode& oNode);
		void TransformToRenderer(const std::wstring& wsString, IRenderer* pRenderer, CContextState* pState);
		void ResetTransform(IRenderer* pRenderer, CContextState* pState);

		void PrepareVmlString(std::wstring& wsString);

	private:

		std::wstring  m_wsPagePath;
		std::wstring  m_wsRootPath;
		CFontList*    m_pFontList;
		CFontManager* m_pFontManager;
	};
}

#endif // _XPS_XPSLIB_PAGE_H
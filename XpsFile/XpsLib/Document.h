#ifndef _XPS_XPSLIB_DOCUMENT_H
#define _XPS_XPSLIB_DOCUMENT_H

#include "FontList.h"
#include "Page.h"
#include <map>
#include <vector>

#define UNICODE
#define _UNICODE
#define _USE_LIBXML2_READER_
#define LIBXML_READER_ENABLED

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

namespace XPS
{
	class CPath;
	class CStaticResource;
	class CDocument
	{
	public:
		CDocument(CFontManager* pFontManager);
		~CDocument();

		bool ReadFromPath(const std::wstring& wsPath);
		int  GetPageCount() const;
		void GetPageSize(int nPageIndex, int& nW, int& nH);
		void DrawPage(int nPageIndex, IRenderer* pRenderer, bool* pbBreak);
		void Close();
		CStaticResource* GetStaticResource(const wchar_t* wsPath);

	private:
									        
		std::wstring                             m_wsPath;					      
		std::map<int, XPS::Page*>                m_mPages;
		CFontList                                m_oFontList;
		CFontManager*                            m_pFontManager;
		std::map<std::wstring, CStaticResource*> m_mStaticResources;
	};	
}

#endif //_XPS_XPSLIB_DOCUMENT_H
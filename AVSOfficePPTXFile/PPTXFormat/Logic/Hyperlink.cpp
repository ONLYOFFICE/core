#include "stdafx.h"

#include "Hyperlink.h"
#include "./../Slide.h"
#include "./../SlideMaster.h"
#include "./../SlideLayout.h"
#include "./../Theme.h"

namespace PPTX
{
	namespace Logic
	{
		CString Hyperlink::GetFullHyperlinkName()const
		{
			if(id.IsInit() && *id != _T(""))
			{
				OOX::RId rid(*id);

				CString sLink = _T("");
				if(parentFileIs<Slide>())
					sLink = parentFileAs<Slide>().GetFullHyperlinkNameFromRId(rid);
				else if(parentFileIs<SlideLayout>())
					sLink = parentFileAs<SlideLayout>().GetFullHyperlinkNameFromRId(rid);
				else if(parentFileIs<SlideMaster>())
					sLink = parentFileAs<SlideMaster>().GetFullHyperlinkNameFromRId(rid);
				else if(parentFileIs<Theme>())
					sLink = parentFileAs<Theme>().GetFullHyperlinkNameFromRId(rid);
				
				sLink.Replace(TCHAR('\\'), TCHAR('/'));
				sLink.Replace(_T("//"), _T("/"));
				sLink.Replace(_T("http:/"), _T("http://"));
				sLink.Replace(_T("https:/"), _T("https://"));
				sLink.Replace(_T("ftp:/"), _T("ftp://"));
				sLink.Replace(_T("file:/"), _T("file://"));

				return sLink;
			}
			return _T("");
		}		
	} // namespace Logic
} // namespace PPTX


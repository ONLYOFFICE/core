#include "Controls.h"
#include "../../ASCOfficeDrawingConverter.h"

#include "../Slide.h"

namespace PPTX
{
	namespace Logic
	{
		CString Control::GetVmlXmlBySpid(CString spid, CString & rels)  const
		{
			CString xml;
			rels = _T("");
			if(parentFileIs<PPTX::Slide>() && parentFileAs<PPTX::Slide>().Vml.IsInit())
			{
				xml		= parentFileAs<PPTX::Slide>().GetVmlXmlBySpid(spid);
				rels	= parentFileAs<PPTX::Slide>().Vml->m_sFilename;
			}
			else if(parentFileIs<PPTX::SlideLayout>() && parentFileAs<PPTX::SlideLayout>().Vml.IsInit())
			{
				xml= parentFileAs<PPTX::SlideLayout>().GetVmlXmlBySpid(spid);
				rels	= parentFileAs<PPTX::SlideLayout>().Vml->m_sFilename;
			}
			else if(parentFileIs<PPTX::SlideMaster>() && parentFileAs<PPTX::SlideMaster>().Vml.IsInit())
			{
				xml = parentFileAs<PPTX::SlideMaster>().GetVmlXmlBySpid(spid);
				rels	= parentFileAs<PPTX::SlideMaster>().Vml->m_sFilename;
			}

			return xml;
		}
		void Control::AddObjectTo (const std::vector<SpTreeElem> *spTreeElements, NSShapeImageGen::CImageManager* pImageManager) const
		{//проблема переноса картинок !!!
			if (spid.IsInit() == false) return;

			CString s = *spid;
			if (s.GetLength() < 8) s = _T("_x0000_s") + s; 

			CString rels;
			CString xml = GetVmlXmlBySpid(s, rels);

			if (xml.GetLength() > 0)
			{
				CString temp = _T("<v:object>");
				temp.Append(xml);
				temp.Append(_T("</v:object>"));
				
				NSBinPptxRW::CDrawingConverter oDrawingConverter;
				oDrawingConverter.SetAdditionalParam(_T("parent_spTree"), (BYTE*)spTreeElements, 0);
	
				RELEASEOBJECT(oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pImageManager);
				oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pImageManager = pImageManager;

				oDrawingConverter.SetRelsPath(rels);

				CString *main_props = NULL;
				HRESULT hRes = oDrawingConverter.AddObject(temp, &main_props);

				oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pImageManager =  NULL;
			}
		}
		void Control::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter)  const
		{
			if (spid.IsInit() == false) return;

			CString s = *spid;
			if (s.GetLength() < 8) s = _T("_x0000_s") + s; 

			CString rels;
			CString xml = GetVmlXmlBySpid(s, rels);

			if (xml.GetLength() > 0)
			{
				CString temp = _T("<v:object>");
				temp.Append(xml);
				temp.Append(_T("</v:object>"));
				NSBinPptxRW::CDrawingConverter oDrawingConverter;
				//oDrawingConverter.SetFontManager(pFontManager);

				RELEASEOBJECT(oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pImageManager);
				oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pImageManager = pWriter->m_pCommon->m_pImageManager;
	
				CString *main_props = NULL;

				oDrawingConverter.SetRelsPath(rels);
				//oDrawingConverter.SetAdditionalParam(_T("xfrm_override"), (BYTE*)&xfrm, sizeof(xfrm));

				HRESULT hRes = oDrawingConverter.AddObject(temp, &main_props);
				if (hRes == S_OK && oDrawingConverter.m_pBinaryWriter->GetPosition() > 10)
				{
					pWriter->WriteBYTEArray(oDrawingConverter.m_pBinaryWriter->GetBuffer(),oDrawingConverter.m_pBinaryWriter->GetPosition());
				}
				oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pImageManager =  NULL;
			}
		}

		void Controls::AddObjectsTo (const std::vector<SpTreeElem> *spTreeElements, NSShapeImageGen::CImageManager*	pImageManager) const
		{
			for (long i=0; i < arrControls.size(); ++i)
			{
				if (arrControls[i].spid.IsInit() == false) continue;

				arrControls[i].AddObjectTo (spTreeElements, pImageManager );
			}
		}
	} // namespace Logic
} // namespace PPTX

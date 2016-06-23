/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
				rels	= parentFileAs<PPTX::Slide>().Vml->filename().GetPath();
			}
			else if(parentFileIs<PPTX::SlideLayout>() && parentFileAs<PPTX::SlideLayout>().Vml.IsInit())
			{
				xml= parentFileAs<PPTX::SlideLayout>().GetVmlXmlBySpid(spid);
				rels	= parentFileAs<PPTX::SlideLayout>().Vml->filename().GetPath();
			}
			else if(parentFileIs<PPTX::SlideMaster>() && parentFileAs<PPTX::SlideMaster>().Vml.IsInit())
			{
				xml = parentFileAs<PPTX::SlideMaster>().GetVmlXmlBySpid(spid);
				rels	= parentFileAs<PPTX::SlideMaster>().Vml->filename().GetPath();
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

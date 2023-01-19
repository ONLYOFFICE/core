/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "WavAudioFile.h"
#include "../../Slide.h"
#include "../../SlideMaster.h"
#include "../../SlideLayout.h"
#include "../../Theme.h"
#include "../../../DocxFormat/Media/Media.h"

namespace PPTX
{
	namespace Logic
	{
		WavAudioFile::WavAudioFile(const std::wstring & name)
		{
			m_name = name;
		}
		WavAudioFile& WavAudioFile::operator=(const WavAudioFile& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			name  = oSrc.name;
			embed = oSrc.embed;
			m_name = oSrc.m_name;
			return *this;
		}
		OOX::EElementType WavAudioFile::getType() const
		{
			return OOX::et_a_snd; //todooo расширить ...
		}
		void WavAudioFile::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name	= XmlUtils::GetNameNoNS(oReader.GetName());
			ReadAttributes( oReader );
		}
		void WavAudioFile::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"embed", embed )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"name", name )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void WavAudioFile::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name	= XmlUtils::GetNameNoNS(node.GetName());

			embed	= node.GetAttribute((L"r:embed"));
			XmlMacroReadAttributeBase(node, L"name", name);
		}
		std::wstring WavAudioFile::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write((L"r:embed"), embed.ToString());
			oAttr.Write((L"name"), name);

			return XmlUtils::CreateNode((L"a:") + m_name, oAttr);
		}
		void WavAudioFile::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode((L"a:") + m_name);

			pWriter->StartAttributes();
			pWriter->WriteAttribute((L"r:embed"), embed.ToString());
			pWriter->WriteAttribute2((L"name"), name);
			pWriter->EndAttributes();

			pWriter->EndNode((L"a:") + m_name);
		}
		void WavAudioFile::FillParentPointersForChilds(){}
		std::wstring WavAudioFile::GetPathFromId(OOX::IFileContainer* pRels, const std::wstring & rId)const
		{
			if (rId.empty()) return L"";

			OOX::RId rid(rId);

			std::wstring sLink = L"";
			if (pRels != NULL)
			{
				smart_ptr<OOX::Media> p = pRels->Get<OOX::Media>(rid);
				if (p.is_init())
					sLink = p->filename().GetPath();
			}
			if(sLink.empty())
			{
				 if(parentFileIs<FileContainer>())
					 sLink = parentFileAs<FileContainer>().GetLinkFromRId(rid);
			}

			return sLink;
		}
		void WavAudioFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			OOX::IFileContainer* pRels = pWriter->GetRels().GetPointer();

			std::wstring audioPath = GetPathFromId(pRels, embed.get());

			NSShapeImageGen::CMediaInfo oId = pWriter->m_pCommon->m_pMediaManager->WriteMedia(audioPath);
			std::wstring s = oId.GetPath2();

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, s);
			if (name.IsInit())
			{
				pWriter->WriteString1(1, *name);
			}
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void WavAudioFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			std::wstring strSoundPath;

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						strSoundPath = pReader->GetString2(true);
						break;
					}
					case 1:
					{
						name = pReader->GetString2();
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);

			if (!strSoundPath.empty())
			{
				std::wstring strPath = pReader->m_strFolder + FILE_SEPARATOR_STR + _T("media")  + FILE_SEPARATOR_STR + strSoundPath;
				NSBinPptxRW::_relsGeneratorInfo oRelsGeneratorInfo = pReader->m_pRels->WriteMedia(strPath, 1);

				if (oRelsGeneratorInfo.nImageRId > 0)
				{
					embed = OOX::RId(oRelsGeneratorInfo.nImageRId);
				}
			}

		}
	} // namespace Logic
} // namespace PPTX


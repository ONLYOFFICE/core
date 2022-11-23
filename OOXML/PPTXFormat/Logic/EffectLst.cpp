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
#include "EffectLst.h"

namespace PPTX
{
	namespace Logic
	{
		void EffectLst::fromXML(XmlUtils::CXmlNode& node)
		{
			blur		= node.ReadNode(_T("a:blur"));
			fillOverlay = node.ReadNode(_T("a:fillOverlay"));
			glow		= node.ReadNode(_T("a:glow"));
			innerShdw	= node.ReadNode(_T("a:innerShdw"));
			outerShdw	= node.ReadNode(_T("a:outerShdw"));
			prstShdw	= node.ReadNode(_T("a:prstShdw"));
			reflection	= node.ReadNode(_T("a:reflection"));
			softEdge	= node.ReadNode(_T("a:softEdge"));

			FillParentPointersForChilds();
		}
		void EffectLst::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
                std::wstring strName = oReader.GetName();
			
				if (strName == L"a:blur")
					blur = oReader;
				else if (strName == L"a:fillOverlay")
					fillOverlay = oReader;
				else if (strName == L"a:glow")
					glow = oReader;
				else if (strName == L"a:innerShdw")
					innerShdw = oReader;
				else if (strName == L"a:outerShdw")
					outerShdw	= oReader;
				else if (strName == L"a:prstShdw")
					prstShdw = oReader;
				else if (strName == L"a:reflection")
					reflection	= oReader;
				else if (strName == L"a:softEdge")
					softEdge = oReader;

			}
		}
		std::wstring EffectLst::toXML() const
		{
			std::wstring str;
			if (blur.IsInit())			str += blur->toXML();
			if (fillOverlay.IsInit())	str += fillOverlay->toXML();
			if (glow.IsInit())			str += glow->toXML();
			if (innerShdw.IsInit())		str += innerShdw->toXML();
			if (outerShdw.IsInit())		str += outerShdw->toXML();
			if (prstShdw.IsInit())		str += prstShdw->toXML();
			if (reflection.IsInit())	str += reflection->toXML();
			if (softEdge.IsInit())		str += softEdge->toXML();

			if (false == str.empty())
			{
				str = L"<a:effectLst>" + str + L"</a:effectLst>";
			}
			else
			{
				str = L"<a:effectLst/>";
			}
			return str;
		}
		void EffectLst::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (!blur.IsInit() && !fillOverlay.IsInit() && !glow.IsInit() && !innerShdw.IsInit() && 
				!outerShdw.IsInit() && !prstShdw.IsInit() && !reflection.IsInit() && !softEdge.IsInit())
			{
				pWriter->WriteString(L"<a:effectLst/>");
				return;
			}
			
			pWriter->StartNode(L"a:effectLst");
			pWriter->EndAttributes();
			
			pWriter->Write(blur);
			pWriter->Write(fillOverlay);
			pWriter->Write(glow);
			pWriter->Write(innerShdw);
			pWriter->Write(outerShdw);
			pWriter->Write(prstShdw);
			pWriter->Write(reflection);
			pWriter->Write(softEdge);

			pWriter->EndNode(L"a:effectLst");
		}
		void EffectLst::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECTPROPERTIES_TYPE_LIST);

			pWriter->WriteRecord2(0, blur);
			pWriter->WriteRecord2(1, fillOverlay);
			pWriter->WriteRecord2(2, glow);
			pWriter->WriteRecord2(3, innerShdw);
			pWriter->WriteRecord2(4, outerShdw);
			pWriter->WriteRecord2(5, prstShdw);
			pWriter->WriteRecord2(6, reflection);
			pWriter->WriteRecord2(7, softEdge);

			pWriter->EndRecord();
		}
		void EffectLst::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(4); // len
			BYTE _type = pReader->GetUChar(); 
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						blur = new Logic::Blur();
						blur->fromPPTY(pReader);							
					}break;
					case 1:
					{
						fillOverlay = new Logic::FillOverlay();
						fillOverlay->fromPPTY(pReader);							
					}break;
					case 2:
					{
						glow = new Logic::Glow();
						glow->fromPPTY(pReader);							
					}break;
					case 3:
					{
						innerShdw = new Logic::InnerShdw();
						innerShdw->fromPPTY(pReader);							
					}break;
					case 4:
					{
						outerShdw = new Logic::OuterShdw();
						outerShdw->fromPPTY(pReader);							
					}break;
					case 5:
					{
						prstShdw = new Logic::PrstShdw();
						prstShdw->fromPPTY(pReader);							
					}break;
					case 6:
					{
						reflection = new Logic::Reflection();
						reflection->fromPPTY(pReader);							
					}break;
					case 7:
					{
						softEdge = new Logic::SoftEdge();
						softEdge->fromPPTY(pReader);							
					}break;
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}	

		void EffectLst::FillParentPointersForChilds()
		{
			if(blur.IsInit())
				blur->SetParentPointer(this);
			if(fillOverlay.IsInit())
				fillOverlay->SetParentPointer(this);
			if(glow.IsInit())
				glow->SetParentPointer(this);
			if(innerShdw.IsInit())
				innerShdw->SetParentPointer(this);
			if(outerShdw.IsInit())
				outerShdw->SetParentPointer(this);
			if(prstShdw.IsInit())
				prstShdw->SetParentPointer(this);
			if(reflection.IsInit())
				reflection->SetParentPointer(this);
			if(softEdge.IsInit())
				softEdge->SetParentPointer(this);
		}

	} // namespace Logic
} // namespace PPTX

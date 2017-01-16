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
#pragma once
#ifndef PPTX_LOGIC_HYPERLINK_INCLUDE_H_
#define PPTX_LOGIC_HYPERLINK_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "../DocxFormat/RId.h"
#include "Media/WavAudioFile.h"

namespace PPTX
{
	namespace Logic
	{

		class Hyperlink : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Hyperlink)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = XmlUtils::GetNameNoNS(node.GetName());

                std::wstring sSndNodeName = _T("snd");
                snd	= node.ReadNodeNoNS(sSndNodeName);

				node.ReadAttributeBase(L"r:id", id);
				node.ReadAttributeBase(L"invalidUrl", invalidUrl);
				node.ReadAttributeBase(L"action", action);
				node.ReadAttributeBase(L"tgtFrame", tgtFrame);
				node.ReadAttributeBase(L"tooltip", tooltip);
				node.ReadAttributeBase(L"history", history);
				node.ReadAttributeBase(L"highlightClick", highlightClick);
				node.ReadAttributeBase(L"endSnd", endSnd);
			}
			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("r:id"), id);
				oAttr.Write(_T("invalidUrl"), invalidUrl);
				oAttr.Write(_T("action"), action);
				oAttr.Write(_T("tgtFrame"), tgtFrame);
				oAttr.Write(_T("tooltip"), tooltip);
				oAttr.Write(_T("history"), history);
				oAttr.Write(_T("highlightClick"), highlightClick);
				oAttr.Write(_T("endSnd"), endSnd);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(snd);

				return XmlUtils::CreateNode(_T("a:") + m_name, oAttr, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

				if (id.is_init())
				{
					FileContainer* pRels = NULL;
					if (pWriter->m_pCommonRels->is_init())
						pRels = pWriter->m_pCommonRels->operator ->();

					std::wstring str = GetFullHyperlinkName(pRels);
					pWriter->WriteString1(0, str);
				}

				pWriter->WriteString2(1, invalidUrl);
				pWriter->WriteString2(2, action);
				pWriter->WriteString2(3, tgtFrame);
				pWriter->WriteString2(4, tooltip);
				pWriter->WriteBool2(5, history);
				pWriter->WriteBool2(6, highlightClick);
				pWriter->WriteBool2(7, endSnd);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, snd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				bool bIsPresentUrl = false;
				std::wstring strUrl = _T("");

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							strUrl = pReader->GetString2();
							bIsPresentUrl = true;
							break;
						}
						case 1:
						{
							invalidUrl = pReader->GetString2();
							break;
						}
						case 2:
						{
							action = pReader->GetString2();
							break;
						}
						case 3:
						{
							tgtFrame = pReader->GetString2();
							break;
						}
						case 4:
						{
							tooltip = pReader->GetString2();
							break;
						}
						case 5:
						{
							history = pReader->GetBool();
							break;
						}
						case 6:
						{
							highlightClick = pReader->GetBool();
							break;
						}
						case 7:
						{
							endSnd = pReader->GetBool();
							break;
						}
						default:
							break;
					}
				}

				if (bIsPresentUrl)
				{
					if (strUrl == _T(""))
						id = _T("");
					else
					{
						LONG lId = pReader->m_pRels->WriteHyperlink(strUrl, action.is_init());

						id = L"rId" + std::to_wstring(lId);
					}
				}

				pReader->Seek(_end_rec);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:") + m_name);

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("r:id"), id);
                pWriter->WriteAttribute2(_T("invalidUrl"), invalidUrl);
                pWriter->WriteAttribute2(_T("action"), action);
                pWriter->WriteAttribute2(_T("tgtFrame"), tgtFrame);
                pWriter->WriteAttribute2(_T("tooltip"), tooltip);
				pWriter->WriteAttribute(_T("history"), history);
				pWriter->WriteAttribute(_T("highlightClick"), highlightClick);
				pWriter->WriteAttribute(_T("endSnd"), endSnd);
				pWriter->EndAttributes();

				pWriter->Write(snd);
				
				pWriter->EndNode(_T("a:") + m_name);
			}
			
		public:
			nullable<WavAudioFile>	snd;

			nullable_string			id;//<PPTX::RId> id;//  <xsd:attribute ref="r:id" use="optional"/>
			nullable_string			invalidUrl;//default=""
			nullable_string			action;//default=""
			nullable_string			tgtFrame;//default=""
			nullable_string			tooltip;//default=""
			nullable_bool			history;//default="true"
			nullable_bool			highlightClick;//default="false"
			nullable_bool			endSnd;//default="false"
		//private:
		public:
			std::wstring m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(snd.IsInit())
					snd->SetParentPointer(this);
			}

			virtual std::wstring GetFullHyperlinkName(FileContainer* pRels)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_HYPERLINK_INCLUDE_H_

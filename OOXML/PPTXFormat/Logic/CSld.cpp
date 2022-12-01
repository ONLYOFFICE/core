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

#include "CSld.h"

namespace PPTX
{
	namespace Logic
	{
		CSld::CSld(OOX::Document *pMain) : WrapperWritingElement(pMain), spTree(L"p")
		{
		}
		void CSld::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
		}
		void CSld::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"name", attrName);

			bg			= node.ReadNode(L"p:bg");
			spTree		= node.ReadNodeNoNS(L"spTree");
			controls	= node.ReadNode(L"p:controls");

			FillParentPointersForChilds();
		}
		std::wstring CSld::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"name", attrName);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(bg);
			oValue.Write(spTree);
			oValue.WriteNullable(controls);

			return XmlUtils::CreateNode(L"p:cSld", oAttr, oValue);
		}
		void CSld::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:cSld");

			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"name", attrName);
			pWriter->EndAttributes();

			pWriter->Write(bg);
			spTree.toXmlWriter(pWriter);
			pWriter->Write(controls);

			pWriter->EndNode(L"p:cSld");
		}
		void CSld::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, attrName);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, bg);

			pWriter->StartRecord(1);
				pWriter->StartRecord(SPTREE_TYPE_SPTREE);
					pWriter->WriteRecord1(0, spTree.nvGrpSpPr);
					pWriter->WriteRecord1(1, spTree.grpSpPr);

					pWriter->StartRecord(2);
						pWriter->WriteULONG((_UINT32)spTree.SpTreeElems.size());
						for (size_t i = 0; i < spTree.SpTreeElems.size(); i++)
						{
							pWriter->WriteRecord1(0, spTree.SpTreeElems[i]);
						}
					pWriter->EndRecord();
				pWriter->EndRecord();
			pWriter->EndRecord();

			pWriter->WriteRecord2(2, controls);
		}
		void CSld::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					attrName = pReader->GetString2();
				else
					break;
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						bg = new Bg();
						bg->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						spTree.fromPPTY(pReader);
						break;
					}
					case 2:
					{
						controls = new Controls(m_pMainDocument);
						controls->fromPPTY(pReader);
						break;
					}
					default:
					{
						pReader->Seek(_end_rec);
						return;
					}
				}
			}

			pReader->Seek(_end_rec);
		}
		void CSld::FillParentPointersForChilds()
		{
			spTree.SetParentPointer(this);
			if(bg.IsInit())			bg->SetParentPointer(this);
			if(controls.IsInit())	controls->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX

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

#include "GraphicEl.h"

namespace PPTX
{
	namespace Logic
	{		
		void GraphicEl::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNode;

			if (node.GetNode(_T("p:chart"), oNode))
			{
				XmlMacroReadAttributeBase(oNode, L"bldStep", chartBuildStep);
				XmlMacroReadAttributeBase(oNode, L"seriesIdx", seriesIdx);
				XmlMacroReadAttributeBase(oNode, L"categoryIdx", categoryIdx);
			}
			else if (node.GetNode(_T("a:dgm"), oNode))
			{
				XmlMacroReadAttributeBase(oNode, L"bldStep", dgmBuildStep);
				XmlMacroReadAttributeBase(oNode, L"id", dgmId);
			}
		}
		std::wstring GraphicEl::toXML() const
		{
			if (chartBuildStep.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("bldStep"), chartBuildStep);
				oAttr.Write(_T("seriesIdx"), seriesIdx);
				oAttr.Write(_T("categoryIdx"), categoryIdx);

				return XmlUtils::CreateNode(_T("p:graphicEl"), XmlUtils::CreateNode(_T("p:chart"), oAttr));
			}

			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("id"), dgmId);
			oAttr.WriteLimitNullable(_T("bldStep"), dgmBuildStep);

			return XmlUtils::CreateNode(_T("p:graphicEl"), XmlUtils::CreateNode(_T("a:dgm"), oAttr));
		}
		void GraphicEl::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteString(toXML());
		}
		void GraphicEl::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, dgmId);
				pWriter->WriteLimit2(1, dgmBuildStep);
				pWriter->WriteLimit2(2, chartBuildStep);
				pWriter->WriteInt2(3, seriesIdx);
				pWriter->WriteInt2(4, categoryIdx);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void GraphicEl::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	dgmId = pReader->GetString2();
				else if (1 == _at)	dgmBuildStep = pReader->GetUChar();
				else if (2 == _at)	chartBuildStep = pReader->GetUChar();
				else if (3 == _at)	seriesIdx = pReader->GetLong();
				else if (4 == _at)	categoryIdx = pReader->GetLong();
			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);
		}
		void GraphicEl::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX

/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "EffectStyle.h"

namespace PPTX
{
	namespace Logic
	{
		EffectStyle& EffectStyle::operator=(const EffectStyle& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			Effects = oSrc.Effects;
			scene3d = oSrc.scene3d;
			sp3d = oSrc.sp3d;
			return *this;
		}
		void EffectStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				size_t nCount = oNodes.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					 if (L"scene3d" == strName)
						scene3d = oNode;
					else if (L"sp3d" == strName)
						sp3d = oNode;
					else if (L"effectDag" == strName ||
							L"effectLst" == strName)
					{
						Effects.fromXML(oNode);
					}
				}
			}
			FillParentPointersForChilds();
		}
		void EffectStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:effectStyle");
			pWriter->EndAttributes();

			Effects.toXmlWriter(pWriter);
			pWriter->Write(scene3d);
			pWriter->Write(sp3d);

			pWriter->EndNode(L"a:effectStyle");
		}
		void EffectStyle::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();
				if (strName == L"a:scene3d")
					scene3d = oReader;
				else if (strName == L"a:sp3d")
					sp3d = oReader;
				else
					Effects.fromXML(oReader);
			}
			FillParentPointersForChilds();
		}
		void EffectStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, Effects);
			pWriter->WriteRecord2(1, scene3d);
			pWriter->WriteRecord2(2, sp3d);
		}
		void EffectStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					Effects.fromPPTY(pReader);					
				}break;
				case 1:
				{
					scene3d = new Logic::Scene3d();
					scene3d->fromPPTY(pReader);					
				}break;
				case 2:
				{
					sp3d = new Logic::Sp3d();
					sp3d->fromPPTY(pReader);					
				}break;
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void EffectStyle::FillParentPointersForChilds()
		{
			Effects.SetParentPointer(this);
			if(scene3d.IsInit())
				scene3d->SetParentPointer(this);
			if(sp3d.IsInit())
				sp3d->SetParentPointer(this);
		}

	} // namespace Logic
} // namespace PPTX

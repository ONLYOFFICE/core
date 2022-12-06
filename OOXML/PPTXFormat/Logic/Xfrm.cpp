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

#include "Xfrm.h"

namespace PPTX
{
	namespace Logic
	{		
		Xfrm::Xfrm()
		{
			node_name = L"a:xfrm";
		}
		Xfrm::~Xfrm() {}
		Xfrm::Xfrm(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		Xfrm::Xfrm(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
		}
		const Xfrm& Xfrm::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		Xfrm::Xfrm(const Xfrm& oSrc)
		{
			*this = oSrc;
		}
		OOX::EElementType Xfrm::getType () const
		{
			return OOX::et_a_xfrm;
		}
		void Xfrm::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			node_name = oReader.GetName();

			ReadAttributes(oReader);

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if (sName == L"a:off")
					ReadAttributes1(oReader, offX, offY);
				else if (sName == L"a:ext")
					ReadAttributes2(oReader, extX, extY);
				else if (sName == L"a:chOff")
					ReadAttributes1(oReader, chOffX, chOffY);
				else if (sName == L"a:chExt")
					ReadAttributes2(oReader, chExtX, chExtY);
			}
		}
		void Xfrm::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("flipH"), flipH)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("flipV"), flipV )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rot"), rot )
			WritingElement_ReadAttributes_End( oReader )
		}
		void Xfrm::ReadAttributes1(XmlUtils::CXmlLiteReader& oReader, nullable_int & x, nullable_int & y)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("x"), x )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("y"), y )
			WritingElement_ReadAttributes_End( oReader )
		}
		void Xfrm::ReadAttributes2(XmlUtils::CXmlLiteReader& oReader, nullable_int & cx, nullable_int & cy)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("cx"), cx )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("cy"), cy )
			WritingElement_ReadAttributes_End( oReader )
		}
		void Xfrm::fromXML(XmlUtils::CXmlNode& node)
		{
			node_name = node.GetName();

			XmlMacroReadAttributeBase(node, L"flipH", flipH);
			XmlMacroReadAttributeBase(node, L"flipV", flipV);
			XmlMacroReadAttributeBase(node, L"rot", rot);

			XmlUtils::CXmlNode oNodeOff;
			if(node.GetNode(_T("a:off"), oNodeOff))
			{
				XmlMacroReadAttributeBase(oNodeOff, L"x", offX);
				XmlMacroReadAttributeBase(oNodeOff, L"y", offY);
			}
			XmlUtils::CXmlNode oNodeExt;
			if(node.GetNode(_T("a:ext"), oNodeExt))
			{
				XmlMacroReadAttributeBase(oNodeExt, L"cx", extX);
				XmlMacroReadAttributeBase(oNodeExt, L"cy", extY);
			}
			XmlUtils::CXmlNode oNodeChOff;
			if(node.GetNode(_T("a:chOff"), oNodeChOff))
			{
				XmlMacroReadAttributeBase(oNodeChOff, L"x", chOffX);
				XmlMacroReadAttributeBase(oNodeChOff, L"y", chOffY);
			}
			XmlUtils::CXmlNode oNodeChExt;
			if(node.GetNode(_T("a:chExt"), oNodeChExt))
			{
				XmlMacroReadAttributeBase(oNodeChExt, L"cx", chExtX);
				XmlMacroReadAttributeBase(oNodeChExt, L"cy", chExtY);
			}
			Normalize();
		}
		std::wstring Xfrm::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"rot", rot);
			oAttr.Write(L"flipH", flipH);
			oAttr.Write(L"flipV", flipV);

			XmlUtils::CNodeValue oValue;

			XmlUtils::CAttribute oAttr1;
			oAttr1.Write(L"x", offX);
			oAttr1.Write(L"y", offY);

			XmlUtils::CAttribute oAttr2;
			oAttr2.Write(_T("cx"), extX);
			oAttr2.Write(_T("cy"), extY);

			XmlUtils::CAttribute oAttr3;
			oAttr3.Write(_T("x"), chOffX);
			oAttr3.Write(_T("y"), chOffY);

			XmlUtils::CAttribute oAttr4;
			oAttr4.Write(_T("cx"), chExtX);
			oAttr4.Write(_T("cy"), chExtY);

			std::wstring strValue = _T("");

			if (_T("") != oAttr1.m_strValue)
				strValue += XmlUtils::CreateNode(_T("a:off"), oAttr1);
			if (_T("") != oAttr2.m_strValue)
				strValue += XmlUtils::CreateNode(_T("a:ext"), oAttr2);
			if (_T("") != oAttr3.m_strValue)
				strValue += XmlUtils::CreateNode(_T("a:chOff"), oAttr3);
			if (_T("") != oAttr4.m_strValue)
				strValue += XmlUtils::CreateNode(_T("a:chExt"), oAttr4);

			return XmlUtils::CreateNode(node_name, oAttr, strValue);
		}
		void Xfrm::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(node_name);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("rot"), rot);
			pWriter->WriteAttribute(_T("flipH"), flipH);
			pWriter->WriteAttribute(_T("flipV"), flipV);
			pWriter->EndAttributes();

			if (offX.is_init() || offY.is_init())
			{
				pWriter->StartNode(_T("a:off"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), offX);
				pWriter->WriteAttribute(_T("y"), offY);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:off"));
			}

			if (extX.is_init() || extY.is_init())
			{
				pWriter->StartNode(_T("a:ext"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("cx"), extX);
				pWriter->WriteAttribute(_T("cy"), extY);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:ext"));
			}

			if (chOffX.is_init() || chOffY.is_init())
			{
				pWriter->StartNode(_T("a:chOff"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), chOffX);
				pWriter->WriteAttribute(_T("y"), chOffY);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:chOff"));
			}

			if (chExtX.is_init() || chExtY.is_init())
			{
				pWriter->StartNode(_T("a:chExt"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("cx"), chExtX);
				pWriter->WriteAttribute(_T("cy"), chExtY);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:chExt"));
			}

			pWriter->EndNode(node_name);
		}
		void Xfrm::toXmlWriter2(const std::wstring& strNS, NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(strNS + L":xfrm");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("rot"), rot);
			pWriter->WriteAttribute(_T("flipH"), flipH);
			pWriter->WriteAttribute(_T("flipV"), flipV);
			pWriter->EndAttributes();

			if (offX.is_init() || offY.is_init())
			{
				pWriter->StartNode(_T("a:off"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), offX);
				pWriter->WriteAttribute(_T("y"), offY);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:off"));
			}

			if (extX.is_init() || extY.is_init())
			{
				pWriter->StartNode(_T("a:ext"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("cx"), extX);
				pWriter->WriteAttribute(_T("cy"), extY);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:ext"));
			}

			if (chOffX.is_init() || chOffY.is_init())
			{
				pWriter->StartNode(_T("a:chOff"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), chOffX);
				pWriter->WriteAttribute(_T("y"), chOffY);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:chOff"));
			}

			if (chExtX.is_init() || chExtY.is_init())
			{
				pWriter->StartNode(_T("a:chExt"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("cx"), chExtX);
				pWriter->WriteAttribute(_T("cy"), chExtY);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:chExt"));
			}

			pWriter->EndNode(strNS + _T(":xfrm"));
		}
		void Xfrm::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, offX);
			pWriter->WriteInt2(1, offY);
			pWriter->WriteInt2(2, extX);
			pWriter->WriteInt2(3, extY);
			pWriter->WriteInt2(4, chOffX);
			pWriter->WriteInt2(5, chOffY);
			pWriter->WriteInt2(6, chExtX);
			pWriter->WriteInt2(7, chExtY);
			pWriter->WriteBool2(8, flipH);
			pWriter->WriteBool2(9, flipV);
			pWriter->WriteInt2(10, rot);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Xfrm::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						offX = pReader->GetLong();
						break;
					}
					case 1:
					{
						offY = pReader->GetLong();
						break;
					}
					case 2:
					{
						extX = pReader->GetLong();
						break;
					}
					case 3:
					{
						extY = pReader->GetLong();
						break;
					}
					case 4:
					{
						chOffX = pReader->GetLong();
						break;
					}
					case 5:
					{
						chOffY = pReader->GetLong();
						break;
					}
					case 6:
					{
						chExtX = pReader->GetLong();
						break;
					}
					case 7:
					{
						chExtY = pReader->GetLong();
						break;
					}
					case 8:
					{
						flipH = pReader->GetBool();
						break;
					}
					case 9:
					{
						flipV = pReader->GetBool();
						break;
					}
					case 10:
					{
						rot = pReader->GetLong();
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void Xfrm::Merge(nullable<Xfrm>& xfrm)const
		{
			if(!xfrm.IsInit())
				xfrm = new Xfrm();
			if(offX.IsInit())
				xfrm->offX = *offX;
			if(offY.IsInit())
				xfrm->offY = *offY;
			if(extX.IsInit())
				xfrm->extX = *extX;
			if(extY.IsInit())
				xfrm->extY = *extY;
			if(chOffX.IsInit())
				xfrm->chOffX = *chOffX;
			if(chOffY.IsInit())
				xfrm->chOffY = *chOffY;
			if(chExtX.IsInit())
				xfrm->chExtX = *chExtX;
			if(chExtY.IsInit())
				xfrm->chExtY = *chExtY;
			if(flipH.IsInit())
				xfrm->flipH = *flipH;
			if(flipV.IsInit())
				xfrm->flipV = *flipV;
			if(rot.IsInit())
				xfrm->rot = *rot;
		}
		void Xfrm::FillParentPointersForChilds(){}
		void Xfrm::Normalize()
		{
			extX.normalize_positive();
			extY.normalize_positive();
			chExtX.normalize_positive();
			chExtY.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX

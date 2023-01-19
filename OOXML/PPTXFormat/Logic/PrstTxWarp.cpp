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

#include "PrstTxWarp.h"

namespace PPTX
{
	namespace Logic
	{
		PrstTxWarp& PrstTxWarp::operator=(const PrstTxWarp& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			avLst = oSrc.avLst;
			prst = oSrc.prst;

			return *this;
		}
		OOX::EElementType PrstTxWarp::getType () const
		{
			return OOX::et_a_prstTxWarp;
		}
		void PrstTxWarp::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (_T("a:avLst") == strName)
				{
					if ( oReader.IsEmptyNode() )
						continue;

					int nCurDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nCurDepth1 ) )
					{
						std::wstring strName1 = oReader.GetName();

						if (_T("a:gd") == strName1)
						{
							Gd gd;
							avLst.push_back(gd);
							avLst.back().fromXML(oReader);
						}
					}
				}
			}
		}
		void PrstTxWarp::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, _T("prst"), prst )
			WritingElement_ReadAttributes_End	( oReader )
		}
		void PrstTxWarp::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"prst", prst);

			XmlUtils::CXmlNode nodeList = node.ReadNode(_T("a:avLst"));
			XmlMacroLoadArray(nodeList, _T("a:gd"), avLst, Gd);

			FillParentPointersForChilds();
		}
		std::wstring PrstTxWarp::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("prst"), prst.get());

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(_T("a:avLst"), avLst);

			return XmlUtils::CreateNode(_T("a:prstTxWarp"), oAttr, oValue);
		}
		void PrstTxWarp::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:prstTxWarp"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("prst"), prst.get());
			pWriter->EndAttributes();

			pWriter->WriteArray(_T("a:avLst"), avLst);

			pWriter->EndNode(_T("a:prstTxWarp"));
		}
		void PrstTxWarp::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit1(0, prst);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecordArray(0, 1, avLst);
		}
		void PrstTxWarp::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					prst.SetBYTECode(pReader->GetUChar());
					break;
				}
				default:
					break;
				}
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					pReader->Skip(4);
					LONG len = pReader->GetLong();
					for (LONG i = 0; i < len; ++i)
					{
						pReader->Skip(1);
						avLst.push_back(Gd());
						avLst[i].fromPPTY(pReader);
					}
					break;
				}
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void PrstTxWarp::FillParentPointersForChilds()
		{
			size_t count = avLst.size();
			for (size_t i = 0; i < count; ++i)
				avLst[i].SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX

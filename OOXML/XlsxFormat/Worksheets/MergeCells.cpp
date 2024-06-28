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

#include "MergeCells.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../XlsbFormat/Biff12_records/MergeCell.h"
#include "../../XlsbFormat/Biff12_records/BeginMergeCells.h"
#include "../../XlsbFormat/Biff12_unions/MERGECELLS.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CMergeCell::CMergeCell(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CMergeCell::~CMergeCell()
		{
		}
		void CMergeCell::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMergeCell::toXML() const
		{
			return _T("");
		}
		void CMergeCell::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<mergeCell"));
			WritingStringNullableAttrEncodeXmlString(L"ref", m_oRef, m_oRef.get());
			writer.WriteString(_T("/>"));
		}
		void CMergeCell::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CMergeCell::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CMergeCell::toBin()
		{
			auto castedPtr(new XLSB::MergeCell);
			XLS::BaseObjectPtr ptr(castedPtr);
			castedPtr->rfx = m_oRef.get();
			return ptr;
		}
		EElementType CMergeCell::getType () const
		{
			return et_x_MergeCell;
		}
		void CMergeCell::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"), m_oRef )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CMergeCell::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::MergeCell*>(obj.get());
			m_oRef  = ptr->rfx.toString(true, true);
		}

		CMergeCells::CMergeCells(OOX::Document *pMain) : WritingElementWithChilds<CMergeCell>(pMain)
		{
		}
		CMergeCells::~CMergeCells()
		{
		}
		void CMergeCells::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMergeCells::toXML() const
		{
			return _T("");
		}
		void CMergeCells::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_arrItems.empty()) return;

			writer.WriteString(_T("<mergeCells"));
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			writer.WriteString(_T(">"));

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString(_T("</mergeCells>"));
		}
		void CMergeCells::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("mergeCell") == sName )
				{
					CMergeCell *pMergeCell = new CMergeCell(m_pMainDocument);
					m_arrItems.push_back(pMergeCell);

					pMergeCell->fromXML(oReader);
				}
			}
		}
		void CMergeCells::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			ReadAttributes(obj);

			if (obj.empty())
				return;

			for(auto &mergeCell : obj)
			{
				CMergeCell *pMergeCell = new CMergeCell(m_pMainDocument);
				m_arrItems.push_back(pMergeCell);

				pMergeCell->fromBin(mergeCell);
			}
		}
		XLS::BaseObjectPtr CMergeCells::toBin()
		{
			auto castedPtr(new XLSB::MERGECELLS);
            auto beginCells(new XLSB::BeginMergeCells);
            castedPtr->m_BrtBeginMergeCells = XLS::BaseObjectPtr{beginCells};
			XLS::BaseObjectPtr ptr(castedPtr);
			for(auto i:m_arrItems)
			{
				castedPtr->m_arBrtMergeCell.push_back(i->toBin());
			}
            beginCells->cmcs = castedPtr->m_arBrtMergeCell.size();
			return ptr;
		}
		EElementType CMergeCells::getType () const
		{
			return et_x_MergeCells;
		}
		void CMergeCells::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"), m_oCount )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CMergeCells::ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
			{
				m_oCount = (_UINT32)obj.size();
			}

	} //Spreadsheet
} // namespace OOX

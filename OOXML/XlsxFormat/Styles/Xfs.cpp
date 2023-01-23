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

#include "Xfs.h"
#include "../../XlsbFormat/Biff12_records/CommonRecords.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CAligment::CAligment()
		{
		}
		CAligment::~CAligment()
		{
		}
		void CAligment::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CAligment::toXML() const
		{
			return _T("");
		}
		void CAligment::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXMLWithNS(writer, L"", L"alignment", L"");
		}
		void CAligment::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
		{
			writer.StartNodeWithNS(node_ns, node_name);
			writer.StartAttributes();
				WritingStringNullableAttrString(L"horizontal", m_oHorizontal, m_oHorizontal->ToString());
				WritingStringNullableAttrInt(L"indent", m_oIndent, *m_oIndent);
				WritingStringNullableAttrBool(L"justifyLastLine", m_oJustifyLastLine);
				WritingStringNullableAttrInt(L"readingOrder", m_oReadingOrder, *m_oReadingOrder);
				WritingStringNullableAttrInt(L"relativeIndent", m_oRelativeIndent, *m_oRelativeIndent);
				WritingStringNullableAttrBool(L"shrinkToFit", m_oShrinkToFit);
				WritingStringNullableAttrInt(L"textRotation", m_oTextRotation, *m_oTextRotation);
				WritingStringNullableAttrString(L"vertical", m_oVertical, m_oVertical->ToString());
				WritingStringNullableAttrBool(L"wrapText", m_oWrapText);
			writer.EndAttributesAndNode();
		}
		void CAligment::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CAligment::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CAligment::getType () const
		{
			return et_x_Aligment;
		}
		bool CAligment::IsEmpty()
		{
			return !(m_oHorizontal.IsInit() || m_oIndent.IsInit() || m_oJustifyLastLine.IsInit() || m_oReadingOrder.IsInit() || m_oRelativeIndent.IsInit() ||
				m_oShrinkToFit.IsInit() || m_oTextRotation.IsInit() || (m_oVertical.IsInit() && SimpleTypes::Spreadsheet::verticalalignmentBottom != m_oVertical->GetValue()) || m_oWrapText.IsInit());
		}
		void CAligment::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			nullable_string readingOrder;
			nullable_double rotate;

			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("horizontal"),			m_oHorizontal )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("indent"),			m_oIndent )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("justifyLastLine"),	m_oJustifyLastLine )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("readingOrder"),	m_oReadingOrder )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("relativeIndent"),	m_oRelativeIndent )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("shrinkToFit"),		m_oShrinkToFit )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("textRotation"),	m_oTextRotation )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("vertical"),		m_oVertical )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrapText"),		m_oWrapText )
		// 2003
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:Horizontal"),	m_oHorizontal)
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:Vertical"),		m_oVertical)
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:WrapText"),		m_oWrapText)
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:Indent"),		m_oIndent)
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:ReadingOrder"),	readingOrder)
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:Rotate"),		rotate)
				WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:ShrinkToFit"),	m_oShrinkToFit)
			WritingElement_ReadAttributes_End( oReader )

			if (readingOrder.IsInit())
			{
				if (*readingOrder == L"Context")			m_oReadingOrder = (unsigned int)0;
				else if (*readingOrder == L"LeftToRight")	m_oReadingOrder = (unsigned int)1;
				else if (*readingOrder == L"RightToLeft")	m_oReadingOrder = (unsigned int)2;
			}
			if (rotate.IsInit())
			{
				if (*rotate >= 0)
					m_oTextRotation = (unsigned int)(*rotate);
				else
					m_oTextRotation = (unsigned int)(90 - *rotate);
			}
		}
		void CAligment::ReadAttributes(XLS::BaseObjectPtr& obj)
			{
				auto ptrBiff = dynamic_cast<XLS::BiffRecord*>(obj.get());
				if(ptrBiff->getTypeId() == XLSB::rt_XF)
				{
					auto ptr = static_cast<XLS::XF*>(obj.get());
					if(ptr != nullptr)
					{
						m_oIndent              = ptr->cIndent;
						m_oJustifyLastLine     = ptr->fJustLast;
						m_oReadingOrder        = ptr->iReadOrder;
						m_oRelativeIndent      = ptr->iReadOrder;
						m_oShrinkToFit         = ptr->fShrinkToFit;
						m_oTextRotation        = ptr->trot;
						m_oWrapText            = ptr->fWrap;

						switch(ptr->alc)
						{
							case 0:
								m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentGeneral; break;
							case 1:
								m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentLeft; break;
							case 2:
								m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentCenter; break;
							case 3:
								m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentRight; break;
							case 4:
								m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentFill; break;
							case 5:
								m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentJustify; break;
							case 6:
								m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentCenterContinuous; break;
							case 7:
								m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentDistributed; break;
						}

						switch(ptr->alcV)
						{
							case 0:
								m_oVertical = SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentTop; break;
							case 1:
								m_oVertical = SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentCenter; break;
							case 2:
								m_oVertical = SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentBottom; break;
							case 3:
								m_oVertical = SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentJustify; break;
							case 4:
								m_oVertical = SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentDistributed; break;
						}
					}
				}
			}

		CProtection::CProtection()
		{
		}
		CProtection::~CProtection()
		{
		}
		void CProtection::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CProtection::toXML() const
		{
			return _T("");
		}
		void CProtection::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXMLWithNS(writer, L"", L"protection", L"");
		}
		void CProtection::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
		{
			writer.WriteString(L"<" + node_ns + node_name);
			WritingStringNullableAttrBool(L"hidden", m_oHidden);
			WritingStringNullableAttrBool(L"locked", m_oLocked);
			writer.WriteString(L"/>");
		}
		void CProtection::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CProtection::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CProtection::getType () const
		{
			return et_x_Protection;
		}
		void CProtection::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if ( oReader, _T("hidden"), m_oHidden )
				WritingElement_ReadAttributes_Read_else_if ( oReader, _T("locked"), m_oLocked )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CProtection::ReadAttributes(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::XF*>(obj.get());
				if(ptr != nullptr)
				{
					m_oHidden     = ptr->fHidden;
					m_oLocked     = ptr->fLocked;
				}
			}

		CXfs::CXfs()
		{
		}
		CXfs::~CXfs()
		{
		}
		void CXfs::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CXfs::toXML() const
		{
			return _T("");
		}
		void CXfs::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<xf"));
			WritingStringNullableAttrInt(L"fontId", m_oFontId, m_oFontId->GetValue());
			WritingStringNullableAttrInt(L"fillId", m_oFillId, m_oFillId->GetValue());
			WritingStringNullableAttrInt(L"borderId", m_oBorderId, m_oBorderId->GetValue());
			WritingStringNullableAttrInt(L"numFmtId", m_oNumFmtId, m_oNumFmtId->GetValue());
			WritingStringNullableAttrInt(L"xfId", m_oXfId, m_oXfId->GetValue());
			WritingStringNullableAttrBool(L"applyNumberFormat", m_oApplyNumberFormat);
			WritingStringNullableAttrBool(L"applyFont", m_oApplyFont);
			WritingStringNullableAttrBool(L"applyFill", m_oApplyFill);
			WritingStringNullableAttrBool(L"applyBorder", m_oApplyBorder);
			WritingStringNullableAttrBool(L"applyAlignment", m_oApplyAlignment);
			WritingStringNullableAttrBool(L"applyProtection", m_oApplyProtection);
			WritingStringNullableAttrBool(L"quotePrefix", m_oQuotePrefix);
			WritingStringNullableAttrBool(L"pivotButton", m_oPivotButton);
			if (m_oAligment.IsInit() || m_oProtection.IsInit())
			{
				writer.WriteString(_T(">"));

				if (m_oAligment.IsInit())m_oAligment->toXML(writer);
				if (m_oProtection.IsInit())m_oProtection->toXML(writer);

				writer.WriteString(_T("</xf>"));
			}
			else
				writer.WriteString(_T("/>"));
		}
		void CXfs::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("alignment") == sName )
					m_oAligment = oReader;
				else if( _T("protection") == sName )
					m_oProtection = oReader;
			}
		}
		EElementType CXfs::getType () const
		{
			return et_x_Xfs;
		}
		void CXfs::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);

			m_oAligment     = obj;
			m_oProtection   = obj;
		}
		void CXfs::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyAlignment"),      m_oApplyAlignment )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyBorder"),      m_oApplyBorder )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyFill"),      m_oApplyFill )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyFont"),      m_oApplyFont )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyNumberFormat"),      m_oApplyNumberFormat )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyProtection"),      m_oApplyProtection )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("borderId"),      m_oBorderId )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("fillId"),      m_oFillId )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("fontId"),      m_oFontId )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("numFmtId"),      m_oNumFmtId )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("pivotButton"),      m_oPivotButton )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("quotePrefix"),      m_oQuotePrefix )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("xfId"),      m_oXfId )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CXfs::ReadAttributes(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::XF*>(obj.get());
				if(ptr != nullptr)
				{
					m_oBorderId     = ptr->ixBorder;
					m_oFillId       = ptr->iFill;
					m_oFontId       = ptr->font_index;
					m_oNumFmtId     = ptr->ifmt;
					m_oPivotButton  = ptr->fsxButton;
					m_oQuotePrefix  = ptr->f123Prefix;

					if(ptr->ixfParent != 0xFFFF)
						m_oXfId     = ptr->ixfParent;

					m_oApplyAlignment       = ptr->fAtrAlc;
					m_oApplyBorder          = ptr->fAtrBdr;
					m_oApplyFill            = ptr->fAtrPat;
					m_oApplyFont            = ptr->fAtrFnt;
					m_oApplyNumberFormat    = ptr->fAtrNum;
					m_oApplyProtection      = ptr->fAtrProt;
				}
			}

		CCellXfs::CCellXfs()
		{
		}
		CCellXfs::~CCellXfs()
		{
		}
		void CCellXfs::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CCellXfs::toXML() const
		{
			return _T("");
		}
		void CCellXfs::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<cellXfs"));
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			writer.WriteString(_T(">"));

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(_T("</cellXfs>"));
		}
		void CCellXfs::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("xf") == sName )
					m_arrItems.push_back( new CXfs( oReader ));
			}
		}
		void CCellXfs::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			ReadAttributes(obj);

			for(auto &xfs : obj)
			{
				CXfs *pXfs = new CXfs(xfs);
				m_arrItems.push_back(pXfs);
			}
		}
		EElementType CCellXfs::getType () const
		{
			return et_x_CellXfs;
		}
		void CCellXfs::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )

				WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )

				WritingElement_ReadAttributes_End( oReader )
		}
		void CCellXfs::ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
			{
				m_oCount = (_UINT32)obj.size();
			}

		CCellStyleXfs::CCellStyleXfs()
		{
		}
		CCellStyleXfs::~CCellStyleXfs()
		{
		}
		void CCellStyleXfs::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CCellStyleXfs::toXML() const
		{
			return _T("");
		}
		void CCellStyleXfs::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<cellStyleXfs"));
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			writer.WriteString(_T(">"));

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString(_T("</cellStyleXfs>"));
		}
		void CCellStyleXfs::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("xf") == sName )
					m_arrItems.push_back( new CXfs( oReader ));
			}
		}
		EElementType CCellStyleXfs::getType () const
		{
			return et_x_CellStyleXfs;
		}
		void CCellStyleXfs::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			ReadAttributes(obj);

			for(auto &xfs : obj)
			{
				CXfs *pXfs = new CXfs(xfs);
				m_arrItems.push_back(pXfs);
			}
		}
		void CCellStyleXfs::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CCellStyleXfs::ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
			{
				m_oCount = (_UINT32)obj.size();
			}

	} //Spreadsheet
} // namespace OOX


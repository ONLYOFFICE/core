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
#pragma once
#ifndef OOX_SHEETDATA_FILE_INCLUDE_H_
#define OOX_SHEETDATA_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "../SharedStrings/Si.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CFormula : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFormula)
			CFormula()
			{
			}
			virtual ~CFormula()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<f"));
				WritingStringNullableAttrBool(L"aca", m_oAca);
				WritingStringNullableAttrBool(L"bx", m_oBx);
				WritingStringNullableAttrBool(L"ca", m_oCa);
				WritingStringNullableAttrBool(L"del1", m_oDel1);
				WritingStringNullableAttrBool(L"del2", m_oDel2);
				WritingStringNullableAttrBool(L"dt2D", m_oDt2D);
				WritingStringNullableAttrBool(L"dtr", m_oDtr);
				WritingStringNullableAttrString(L"r1", m_oR1, m_oR1.get());
				WritingStringNullableAttrString(L"r2", m_oR2, m_oR2.get());
				WritingStringNullableAttrString(L"ref", m_oRef, m_oRef.get());
				WritingStringNullableAttrInt(L"si", m_oSi, m_oSi->GetValue());
				WritingStringNullableAttrString(L"t", m_oT, m_oT->ToString());
                writer.WriteString(_T(">"));
                writer.WriteEncodeXmlString(m_sText);
				writer.WriteString(_T("</f>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
                    return;

				m_sText = oReader.GetText3();
			}

			virtual EElementType getType () const
			{
				return et_x_Formula;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("aca"),		m_oAca )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("bx"),      m_oBx )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ca"),      m_oCa )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("del1"),	m_oDel1 )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("del2"),	m_oDel2 )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("dt2D"),	m_oDt2D )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("dtr"),		m_oDtr )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r1"),		m_oR1 )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r2"),		m_oR2 )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),		m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("si"),		m_oSi )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("t"),		m_oT )

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<SimpleTypes::COnOff<>>							m_oAca;
				nullable<SimpleTypes::COnOff<>>							m_oBx;
				nullable<SimpleTypes::COnOff<>>							m_oCa;
				nullable<SimpleTypes::COnOff<>>							m_oDel1;
				nullable<SimpleTypes::COnOff<>>							m_oDel2;
				nullable<SimpleTypes::COnOff<>>							m_oDt2D;
				nullable<SimpleTypes::COnOff<>>							m_oDtr;
				nullable<std::wstring>									m_oR1;
				nullable<std::wstring>									m_oR2;
				nullable<std::wstring>									m_oRef;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oSi;
				nullable<SimpleTypes::Spreadsheet::CCellFormulaType<>>	m_oT;

				std::wstring m_sText;
		};

		class CCell : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCell)
			CCell()
			{
			}
			virtual ~CCell()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("f") == sName )
						m_oFormula = oReader;
					else if ( _T("is") == sName )
						m_oRichText = oReader;
					else if ( _T("v") == sName )
						m_oValue = oReader;
				}
				PrepareForBinaryWriter();
			}

			virtual EElementType getType () const
			{
				return et_x_Cell;
			}

			bool isInitRef() const
			{
				return m_oRef.IsInit() || (m_oRow.IsInit() && m_oCol.IsInit());
			}
			std::wstring getRef() const
			{
				if (m_oRef.IsInit())
				{
					return m_oRef.get();
				}
				else if (m_oRow.IsInit() && m_oCol.IsInit())
				{
					return combineRef(m_oRow->GetValue(), m_oCol->GetValue());
				}
				else
				{
					return L"A1";
				}
			}
			void setRef(const std::wstring& sRef) 
			{
				m_oRef = sRef;
			}
			bool getRowCol(int& nRow, int& nCol) const
			{
				bool bRes = false;
				nRow = 0;
				nCol = 0;
				if (m_oRef.IsInit())
				{
					if (parseRef(m_oRef.get(), nRow, nCol))
					{
						bRes = true;
						nRow--;
						nCol--;
					}

				}
				else if (m_oRow.IsInit() && m_oCol.IsInit())
				{
					bRes = true;
					nRow = m_oRow->GetValue();
					nCol = m_oCol->GetValue();
				}
				return bRes;
			}
			void setRowCol(int nRow, int nCol)
			{
				m_oRow.Init();
				m_oRow->SetValue(nRow);
				m_oCol.Init();
				m_oCol->SetValue(nCol);
			}

			static bool parse3DRef(const std::wstring& sRef, std::wstring& workbook, std::wstring& sheetFrom, std::wstring& sheetTo, int& nRow1, int& nCol1, int& nRow2, int& nCol2)
			{
				bool bRes = false;
				int nIndex = (int)sRef.find('!');
				std::wstring sCellRef;

				if (std::wstring::npos != nIndex)
				{
					std::wstring sSheetPrefix = sRef.substr(0, nIndex);
					if (sSheetPrefix.length() > 0 && '\'' == sSheetPrefix[0] && '\'' == sSheetPrefix[sSheetPrefix.length() - 1])
					{
						sSheetPrefix = sSheetPrefix.substr(1, sSheetPrefix.length() - 2);
					}
					NSStringExt::Replace(sSheetPrefix, L"''", L"'");

					int nIndexWbStart = (int)sSheetPrefix.find('[');
					int nIndexWbEnd = (int)sSheetPrefix.find(']');

					if (-1 != nIndexWbStart && -1 != nIndexWbEnd)
					{
						workbook = sSheetPrefix.substr(nIndexWbStart + 1, nIndexWbEnd - nIndexWbStart - 1);
						sSheetPrefix = sSheetPrefix.substr(nIndexWbEnd + 1, sSheetPrefix.length() - nIndexWbEnd - 1);
					}
					int nIndexColon = (int)sSheetPrefix.find(':');
					if (-1 != nIndexColon)
					{
						sheetFrom = sSheetPrefix.substr(0, nIndexColon);
						sheetTo = sSheetPrefix.substr(nIndexColon + 1, sSheetPrefix.length() - nIndexColon - 1);
					}
					else
					{
						sheetFrom = sSheetPrefix;
					}
					sCellRef = sRef.substr(nIndex + 1, sRef.length() - nIndex - 1);
				}
				else
				{
					sCellRef = sRef;
				}
				NSStringExt::Replace(sCellRef, L"$", L"");
				int nIndexColon = (int)sCellRef.find(':');
				if (std::wstring::npos != nIndexColon)
				{
					bRes = parseRef(sCellRef.substr(0, nIndexColon), nRow1, nCol1) && parseRef(sCellRef.substr(nIndexColon + 1, sCellRef.length() - nIndexColon - 1), nRow2, nCol2);
				}
				else
				{
					bRes = parseRef(sCellRef, nRow1, nCol1);
					nRow2 = nRow1;
					nCol2 = nCol1;
				}
				return bRes;
			}
			static bool parseRef(std::wstring sRef, int& nRow, int& nCol)
			{
				bool bRes = false;

				nRow = 0;
				nCol = 0;
				int nLegnth = (int)sRef.length();
				if (nLegnth > 0)
				{
					int nIndex = 0;
					NSStringExt::ToUpper(sRef);
                    wchar_t cCurLetter = sRef[nIndex];
					while ('A' <= cCurLetter && cCurLetter <= 'Z' && nIndex < nLegnth)
					{
						nIndex++;
						cCurLetter = sRef[nIndex];
					}
					if (nIndex > 0)
					{
						std::wstring sAdd = sRef.substr(0, nIndex);
						std::wstring sDig = sRef.substr(nIndex, nLegnth - nIndex);
						for (size_t i = 0, length = sAdd.length(); i < length; ++i)
						{
							nCol = nCol * 26 + sAdd[i] - 'A' + 1;
						}
						if (!sDig.empty())
						{
							nRow = _wtoi(sDig.c_str());
							bRes = true;
						}
					}
				}
				return bRes;
			}
			static std::wstring combineRef(int nRow, int nCol);
		private:
			void PrepareForBinaryWriter();

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if ( oReader, _T("cm"), m_oCellMetadata )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("ph"), m_oShowPhonetic )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("r"), m_oRef )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("s"), m_oStyle )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("t"), m_oType )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("vm"), m_oValueMetadata )

					WritingElement_ReadAttributes_End( oReader )
			}

			nullable<std::wstring>								m_oRef;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oRow;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCol;
		public:
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCellMetadata;
				nullable<SimpleTypes::COnOff<>>						m_oShowPhonetic;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oStyle;
				nullable<SimpleTypes::Spreadsheet::CCellTypeType<>>	m_oType;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oValueMetadata;

				nullable<CFormula>	m_oFormula;
				nullable<CSi>		m_oRichText;
				nullable<CText>		m_oValue;
		};

		//необработано:
		//<extLst>
		class CRow : public WritingElementWithChilds<CCell>
		{
		public:
			WritingElement_AdditionConstructors(CRow)
			CRow()
			{
			}
			virtual ~CRow()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLStart(writer);
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				
				toXMLEnd(writer);
			}
			virtual void toXMLStart(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<row"));
				WritingStringNullableAttrBool(L"collapsed", m_oCollapsed);
				WritingStringNullableAttrBool(L"customFormat", m_oCustomFormat);
				WritingStringNullableAttrDouble(L"ht", m_oHt, m_oHt->GetValue());
				WritingStringNullableAttrBool(L"customHeight", m_oCustomHeight);
				WritingStringNullableAttrBool(L"hidden", m_oHidden);
				WritingStringNullableAttrInt(L"outlineLevel", m_oOutlineLevel, m_oOutlineLevel->GetValue());
				WritingStringNullableAttrBool(L"ph", m_oPh);
				WritingStringNullableAttrInt(L"r", m_oR, m_oR->GetValue());
				WritingStringNullableAttrInt(L"s", m_oS, m_oS->GetValue());
				WritingStringNullableAttrBool(L"thickBot", m_oThickBot);
				WritingStringNullableAttrBool(L"thickTop", m_oThickTop);
				writer.WriteString(_T(">"));
			}
			virtual void toXMLEnd(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("</row>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("c") == sName )
					{
						CCell *pCell = new CCell();
						if (pCell)
						{
							pCell->m_pMainDocument = m_pMainDocument;
							pCell->fromXML(oReader);
							m_arrItems.push_back(pCell);
						}
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Row;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("collapsed"),		m_oCollapsed )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("customFormat"),    m_oCustomFormat )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("customHeight"),    m_oCustomHeight )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("hidden"),			m_oHidden )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ht"),				m_oHt )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("outlineLevel"),    m_oOutlineLevel )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ph"),				m_oPh )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r"),				m_oR )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("s"),				m_oS )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("thickBot"),		m_oThickBot )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("thickTop"),		m_oThickTop )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("x14ac:dyDescent"),	m_oDyDescent )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
			nullable<SimpleTypes::COnOff<>>					m_oCollapsed;
			nullable<SimpleTypes::COnOff<>>					m_oCustomFormat;
			nullable<SimpleTypes::COnOff<>>					m_oCustomHeight;
			nullable<SimpleTypes::COnOff<>>					m_oHidden;
			nullable<SimpleTypes::CDouble>					m_oHt;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oOutlineLevel;
			nullable<SimpleTypes::COnOff<>>					m_oPh;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oR;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oS;
			nullable<SimpleTypes::COnOff<>>					m_oThickBot;
			nullable<SimpleTypes::COnOff<>>					m_oThickTop;
			nullable<SimpleTypes::CDouble>					m_oDyDescent;

		};

		class CSheetData  : public WritingElementWithChilds<CRow>
		{
		public:
			WritingElement_AdditionConstructors(CSheetData)
			CSheetData()
			{
			}
			virtual ~CSheetData()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				toXMLStart(writer);
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				toXMLEnd(writer);
			}
			virtual void toXMLStart(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<sheetData>"));
			}
			virtual void toXMLEnd(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("</sheetData>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("row") == sName )
					{
						CRow *pRow = new CRow();
						if (pRow)
						{
							pRow->m_pMainDocument = m_pMainDocument;
							pRow->fromXML(oReader);
							m_arrItems.push_back(pRow);
						}
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_SheetData;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SHEETDATA_FILE_INCLUDE_H_

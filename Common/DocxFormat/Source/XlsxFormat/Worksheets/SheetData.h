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

namespace NSBinPptxRW
{
	class CBinaryFileReader;
	class CXlsbBinaryWriter;
}
namespace CSVWriter
{
	class CCSVWriter;
}
namespace NSFile
{
	class CStreamWriter;
}

namespace OOX
{
	namespace Spreadsheet
	{
		class CFormulaXLSB
		{
		public:
			CFormulaXLSB();
			void Clean();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			_UINT32 getXLSBSize() const;
			_UINT16 toXLSB(NSBinPptxRW::CXlsbBinaryWriter& oStream, bool bIsBlankFormula);
			void toXLSBExt(NSBinPptxRW::CXlsbBinaryWriter& oStream);
		public:
			bool m_bIsInit;
			CStringXLSB m_oFormula;
			SimpleTypes::Spreadsheet::CCellFormulaType<SimpleTypes::Spreadsheet::cellformulatypeNormal> m_oT;
			_INT32 m_nSi;
			CStringXLSB m_oRef;
			CStringXLSB m_oR1;
			CStringXLSB m_oR2;
			SimpleTypes::COnOff<> m_oAca;
			SimpleTypes::COnOff<> m_oBx;
			SimpleTypes::COnOff<> m_oCa;
			SimpleTypes::COnOff<> m_oDel1;
			SimpleTypes::COnOff<> m_oDel2;
			SimpleTypes::COnOff<> m_oDt2D;
			SimpleTypes::COnOff<> m_oDtr;
		protected:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CCellXLSB
		{
		public:
			CCellXLSB();
			void Clean();
			void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void toXLSB(NSBinPptxRW::CXlsbBinaryWriter& oStream);
		public:
			_UINT32 m_nCol;
			_UINT32 m_nStyle;
			SimpleTypes::Spreadsheet::CCellTypeType<SimpleTypes::Spreadsheet::celltypeNumber> m_oType;
			SimpleTypes::COnOff<> m_oShowPhonetic;

			CTextXLSB m_oValue;
			CFormulaXLSB m_oFormula;
			nullable<CSi> m_oRichText;
		protected:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};
		class CRowXLSB
		{
		public:
			CRowXLSB();
			void Clean();
			void fromXMLToXLSB(XmlUtils::CXmlLiteReader& oReader, NSBinPptxRW::CXlsbBinaryWriter& oStream, CCellXLSB& oCell);
			void toXLSB(NSBinPptxRW::CXlsbBinaryWriter& oStream);
		public:
			_UINT32 m_nR;
			_UINT32 m_nS;
			SimpleTypes::COnOff<> m_oCustomFormat;
			double m_dHt;
			SimpleTypes::COnOff<> m_oHidden;
			SimpleTypes::COnOff<> m_oCustomHeight;
			BYTE m_nOutlineLevel;
			SimpleTypes::COnOff<> m_oCollapsed;
			SimpleTypes::COnOff<> m_oThickBot;
			SimpleTypes::COnOff<> m_oThickTop;
			SimpleTypes::COnOff<> m_oPh;
		protected:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};


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
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
                    return;

				m_sText = oReader.GetText3();
			}
			void fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream);
			void fromXLSBExt (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nFlags);
			virtual EElementType getType () const
			{
				return et_x_Formula;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_StartChar( oReader )

					WritingElement_ReadAttributes_Read_ifChar		( oReader, "t",		m_oT )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "si",	m_oSi )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "aca",	m_oAca )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "bx",    m_oBx )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "ca",    m_oCa )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "del1",	m_oDel1 )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "del2",	m_oDel2 )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "dt2D",	m_oDt2D )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "dtr",	m_oDtr )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "r1",	m_oR1 )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "r2",	m_oR2 )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "ref",	m_oRef )
					WritingElement_ReadAttributes_Read_else_ifChar	( oReader, "si",	m_oSi )

					WritingElement_ReadAttributes_EndChar( oReader )
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType, _UINT32 nRow);

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
					const std::string& s = m_oRef.get();
					return std::wstring(s.begin(), s.end());
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
				m_oRef = std::string(sRef.begin(), sRef.end());
			}
			bool getRowCol(int& nRow, int& nCol) const
			{
				bool bRes = false;
				nRow = 0;
				nCol = 0;
				if (m_oRow.IsInit() && m_oCol.IsInit())
				{
					bRes = true;
					nRow = m_oRow->GetValue();
					nCol = m_oCol->GetValue();
				}
				else if (m_oRef.IsInit())
				{
					if (parseRefA(m_oRef->c_str(), nRow, nCol))
					{
						bRes = true;
						nRow--;
						nCol--;
					}
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
			static bool parseRef(std::wstring sRef, int& nRow, int& nCol);
			static bool parseRefA(const char* sRef, int& nRow, int& nCol);
			static bool parseRefColA(const char* sRef, _UINT32& nCol);
			static std::wstring combineRef(int nRow, int nCol);
		private:
			void PrepareForBinaryWriter();
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable<std::string>								m_oRef;
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
			void toXMLStart(NSStringUtils::CStringBuilder& writer) const;
			void toXMLEnd(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromXMLToXLSB(XmlUtils::CXmlLiteReader& oReader, NSBinPptxRW::CXlsbBinaryWriter& oStream, CCellXLSB& oCell);
			void fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType);
			void toXLSB (NSBinPptxRW::CXlsbBinaryWriter& oStream) const;
			virtual EElementType getType () const
			{
				return et_x_Row;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void CheckIndex();

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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType, CSVWriter::CCSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter);
			virtual EElementType getType () const
			{
				return et_x_SheetData;
			}
		
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oXlsbPos;
		private:
			void fromXLSBToXmlCell (CCell& pCell, CSVWriter::CCSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter);
			void fromXLSBToXmlRowStart (CRow* pRow, CSVWriter::CCSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter);
			void fromXLSBToXmlRowEnd (CRow* pRow, CSVWriter::CCSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SHEETDATA_FILE_INCLUDE_H_

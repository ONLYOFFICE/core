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

#include "../CommonInclude.h"

#include "../SharedStrings/Si.h"
#include "Cols.h"

namespace NSBinPptxRW
{
	class CBinaryFileReader;
	class CXlsbBinaryWriter;
}
namespace NSFile
{
	class CStreamWriter;
}
class CSVWriter;
//--------------------------------------------------------------------------------------------
namespace OOX
{
	namespace Spreadsheet
	{
		class CCommentItem;

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
			SimpleTypes::Spreadsheet::CCellFormulaType m_oT;
			_INT32 m_nSi;
			CStringXLSB m_oRef;
			CStringXLSB m_oR1;
			CStringXLSB m_oR2;
			SimpleTypes::COnOff m_oAca;
			SimpleTypes::COnOff m_oBx;
			SimpleTypes::COnOff m_oCa;
			SimpleTypes::COnOff m_oDel1;
			SimpleTypes::COnOff m_oDel2;
			SimpleTypes::COnOff m_oDt2D;
			SimpleTypes::COnOff m_oDtr;
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

			_UINT32 m_nCol;
			_UINT32 m_nStyle;
			SimpleTypes::Spreadsheet::CCellTypeType m_oType;
			SimpleTypes::COnOff m_oShowPhonetic;

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
			SimpleTypes::COnOff m_oCustomFormat;
			double m_dHt;
			SimpleTypes::COnOff m_oHidden;
			SimpleTypes::COnOff m_oCustomHeight;
			BYTE m_nOutlineLevel;
			SimpleTypes::COnOff m_oCollapsed;
			SimpleTypes::COnOff m_oThickBot;
			SimpleTypes::COnOff m_oThickTop;
			SimpleTypes::COnOff m_oPh;
		protected:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

		class CFormula : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFormula)
			CFormula();
			virtual ~CFormula();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream);
			void fromXLSBExt (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nFlags);
            void fromBin(XLS::BaseObjectPtr& obj, SimpleTypes::Spreadsheet::ECellFormulaType eType);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::COnOff>							m_oAca;
			nullable<SimpleTypes::COnOff>							m_oBx;
			nullable<SimpleTypes::COnOff>							m_oCa;
			nullable<SimpleTypes::COnOff>							m_oDel1;
			nullable<SimpleTypes::COnOff>							m_oDel2;
			nullable<SimpleTypes::COnOff>							m_oDt2D;
			nullable<SimpleTypes::COnOff>							m_oDtr;
			nullable<std::wstring>									m_oR1;
			nullable<std::wstring>									m_oR2;
			nullable<std::wstring>									m_oRef;
			nullable<SimpleTypes::CUnsignedDecimalNumber>			m_oSi;
			nullable<SimpleTypes::Spreadsheet::CCellFormulaType>	m_oT;

			std::wstring m_sText;
		};

		class CData : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CData)
			CData();
			virtual ~CData();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
					void fromXML2(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void dump(const std::wstring &text);

			nullable_bool	bBold;
			nullable_bool	bItalic;
			nullable_bool	bUnderline;
			nullable_bool	bSubscript;
			nullable_bool	bSuperscript;
			nullable_string sColor;
			nullable_int	nFontSize;


		public:
			nullable<SimpleTypes::Spreadsheet::CCellTypeType>	m_oType;
			nullable<CSi>										m_oRichText;
			nullable<CText>										m_oValue;
		};

		class CCell : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCell)
			CCell(OOX::Document *pMain = NULL);
			virtual ~CCell();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType, _UINT32 nRow);
            void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

			bool isInitRef() const;
			std::wstring getRef() const;
			void setRef(const std::wstring& sRef);
			bool getRowCol(int& nRow, int& nCol) const;
			void setRowCol(int nRow, int nCol);

			static bool parse3DRef(const std::wstring& sRef, std::wstring& workbook, std::wstring& sheetFrom, std::wstring& sheetTo, int& nRow1, int& nCol1, int& nRow2, int& nCol2);
			static bool parseRef(std::wstring sRef, int& nRow, int& nCol);
			static bool parseRefA(const char* sRef, int& nRow, int& nCol);
			static bool parseRefColA(const char* sRef, _UINT32& nCol);
			static std::wstring combineRef(int nRow, int nCol);
		private:
			void PrepareForBinaryWriter();
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadComment(XmlUtils::CXmlLiteReader& oReader, CCommentItem* pComment);

			void AfterRead();
	//----------- 2003			
			void After2003Read();

			nullable<CCommentItem> pCommentItem;
			nullable_string sStyleId;
			nullable_string sArrayRange;
			nullable_string sHyperlink;
			nullable_int iColIndex;
			nullable_int iAcross;
			nullable_int iDown;
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oCellMetadata;
			nullable<SimpleTypes::COnOff>						m_oShowPhonetic;
			nullable_uint										m_oStyle;
			nullable<SimpleTypes::Spreadsheet::CCellTypeType>	m_oType;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oValueMetadata;

			nullable<std::string>	m_oRef;
			nullable_uint			m_oRow;
			nullable_uint			m_oCol;
			nullable<CFormula>		m_oFormula;
			nullable<CSi>			m_oRichText;
			nullable<CText>			m_oValue;
//-----------------------------			
			nullable_string			m_oCacheValue;
		};

		//необработано:
		//<extLst>
		class CRow : public WritingElementWithChilds<CCell>
		{
		public:
			WritingElement_AdditionConstructors(CRow)
			CRow(OOX::Document *pMain = NULL);
			virtual ~CRow();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			void toXMLStart(NSStringUtils::CStringBuilder& writer) const;
			void toXMLEnd(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromXMLToXLSB(XmlUtils::CXmlLiteReader& oReader, NSBinPptxRW::CXlsbBinaryWriter& oStream, CCellXLSB& oCell);
			void fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType);
			void toXLSB (NSBinPptxRW::CXlsbBinaryWriter& oStream) const;            
                        void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void CheckIndex();

		public:
			nullable<SimpleTypes::COnOff>					m_oCollapsed;
			nullable<SimpleTypes::COnOff>					m_oCustomFormat;
			nullable<SimpleTypes::COnOff>					m_oCustomHeight;
			nullable<SimpleTypes::COnOff>					m_oHidden;
			nullable<SimpleTypes::CDouble>					m_oHt;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oOutlineLevel;
			nullable<SimpleTypes::COnOff>					m_oPh;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oR;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oS;
			nullable<SimpleTypes::COnOff>					m_oThickBot;
			nullable<SimpleTypes::COnOff>					m_oThickTop;
			nullable<SimpleTypes::CDouble>					m_oDyDescent;
		};

		class CSheetData  : public WritingElementWithChilds<CRow>
		{
		public:
			WritingElement_AdditionConstructors(CSheetData)
            //WritingElement_XlsbConstructors(CSheetData)
			CSheetData(OOX::Document *pMain = NULL);
			virtual ~CSheetData();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void toXMLStart(NSStringUtils::CStringBuilder& writer) const;
			virtual void toXMLEnd(NSStringUtils::CStringBuilder& writer) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType, CSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter);
            void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;
		
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oXlsbPos;
		
			std::map<int, std::map<int, unsigned int>>	m_mapStyleMerges2003; // map(row, map(col, style))
			void StyleFromMapStyleMerges2003(std::map<int, unsigned int> &mapStyleMerges);
			void AfterRead();

		private:
			void fromXLSBToXmlCell (CCell& pCell, CSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter);
			void fromXLSBToXmlRowStart (CRow* pRow, CSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter);
			void fromXLSBToXmlRowEnd (CRow* pRow, CSVWriter* pCSVWriter, NSFile::CStreamWriter& oStreamWriter);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	// spreadsheets 2003

			nullable_string m_sStyleID;
			nullable_double m_dDefaultColumnWidth;
			nullable_double m_dDefaultRowHeight;
			nullable_int m_nExpandedColumnCount;
			nullable_int m_nExpandedRowCount;
			nullable_int m_nFullColumns;
			nullable_int m_nFullRows;
		};

	} //Spreadsheet
} // namespace OOX


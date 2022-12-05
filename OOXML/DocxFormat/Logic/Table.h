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

#include "TableProperty.h"

namespace ComplexTypes
{
	namespace Word
	{
		//--------------------------------------------------------------------------------
		// TblGridCol 17.4.16 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblGridCol : public ComplexType
		{
		public:
			ComplexTypes_AdditionConstructors(CTblGridCol)
			CTblGridCol();
			virtual ~CTblGridCol();

			virtual void    FromXML(XmlUtils::CXmlNode& oNode);
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring ToString() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CTwipsMeasure > m_oW;
		};

	} // Word
} // ComplexTypes

namespace OOX
{
	namespace Logic
    {
		//--------------------------------------------------------------------------------
		// CTblGridChange 17.13.5.33 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblGrid;

		class CTblGridChange : public WritingElement
		{
		public:
			CTblGridChange(OOX::Document *pMain = NULL);
			CTblGridChange(XmlUtils::CXmlNode &oNode);
			CTblGridChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTblGridChange();
			const CTblGridChange& operator = (const XmlUtils::CXmlNode &oNode);
			const CTblGridChange& operator = (const XmlUtils::CXmlLiteReader& oReader);

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDecimalNumber>	m_oId;

			CTblGrid								*m_pTblGrid;
		};

		//--------------------------------------------------------------------------------
		// CTblGrid 17.4.49 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblGrid : public WritingElement
		{
		public:
			CTblGrid(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
				m_bTblGridChange = false;
			}
			CTblGrid(XmlUtils::CXmlNode &oNode) : WritingElement(NULL)
			{
				m_bTblGridChange = false;

				fromXML( oNode );
			}
			CTblGrid(XmlUtils::CXmlLiteReader& oReader) : WritingElement(NULL)
			{
				m_bTblGridChange = false;

				fromXML( oReader );
			}
			virtual ~CTblGrid()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrGridCol.size(); nIndex++ )
				{
					if ( m_arrGridCol[nIndex] ) delete m_arrGridCol[nIndex];
					m_arrGridCol[nIndex] = NULL;
				}
				m_arrGridCol.clear();
			}
			const CTblGrid& operator = (const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CTblGrid& operator = (const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;

				if ( !m_bTblGridChange && oNode.GetNode( L"w:tblGridChange", oChild ) )
					m_oTblGridChange = oChild;

				XmlUtils::CXmlNodes oGridColNodes;
				if ( oNode.GetNodes( L"w:gridCol", oGridColNodes ) )
				{
					XmlUtils::CXmlNode oGridColNode;
					for ( int nIndex = 0; nIndex < oGridColNodes.GetCount(); nIndex++ )
					{
						if ( oGridColNodes.GetAt( nIndex, oGridColNode ) )
						{
							ComplexTypes::Word::CTblGridCol *oGridCol = new ComplexTypes::Word::CTblGridCol(oGridColNode);
							if (oGridCol) m_arrGridCol.push_back( oGridCol );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( !m_bTblGridChange && L"w:tblGridChange" == sName )
						m_oTblGridChange = oReader;
					else if ( L"w:gridCol" == sName )
					{
						ComplexTypes::Word::CTblGridCol *oGridCol = new ComplexTypes::Word::CTblGridCol(oReader);
						if (oGridCol) m_arrGridCol.push_back( oGridCol );
					}
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = L"<w:tblGrid>";

				if ( !m_bTblGridChange && m_oTblGridChange.IsInit() )
					sResult += m_oTblGridChange->toXML();

				for (unsigned  int nIndex = 0; nIndex < m_arrGridCol.size(); nIndex++ )
				{
					sResult += L"<w:gridCol ";
					if (m_arrGridCol[nIndex])
						sResult += m_arrGridCol[nIndex]->ToString();
					sResult += L"/>";
				}

				sResult += L"</w:tblGrid>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_tblGrid;
			}

			bool                                           m_bTblGridChange;

			nullable<OOX::Logic::CTblGridChange          >	m_oTblGridChange;
			std::vector<ComplexTypes::Word::CTblGridCol *>	m_arrGridCol;
		};

		//--------------------------------------------------------------------------------
		// CTblPrExChange 17.13.5.35 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblPrEx;

		class CTblPrExChange : public WritingElement
		{
		public:
			CTblPrExChange(OOX::Document *pMain = NULL);
			CTblPrExChange(XmlUtils::CXmlNode &oNode);
			CTblPrExChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTblPrExChange();
			const CTblPrExChange& operator = (const XmlUtils::CXmlNode &oNode);
			const CTblPrExChange& operator = (const XmlUtils::CXmlLiteReader& oReader);

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			// Attributes
			nullable<std::wstring					> m_sAuthor;
			nullable<SimpleTypes::CDateTime			> m_oDate;
			nullable<SimpleTypes::CDecimalNumber	> m_oId;
			nullable<std::wstring					> m_sUserId;

			// Childs
			nullable<CTblPrEx						> m_pTblPrEx;
		};

		//--------------------------------------------------------------------------------
		// CTblPrEx 17.4.61 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblPrEx : public WritingElement
		{
		public:
			CTblPrEx(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
				m_bTblPrExChange = false;
			}
			CTblPrEx(XmlUtils::CXmlNode &oNode) : WritingElement(NULL)
			{
				m_bTblPrExChange = false;

				fromXML( oNode );
			}
			CTblPrEx(XmlUtils::CXmlLiteReader& oReader) : WritingElement(NULL)
			{
				m_bTblPrExChange = false;

				fromXML( oReader );
			}
			virtual ~CTblPrEx()
			{
			}
			const CTblPrEx& operator = (const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CTblPrEx& operator = (const XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;

				WritingElement_ReadNode( oNode, oChild, L"w:jc",             m_oJc );
				WritingElement_ReadNode( oNode, oChild, L"w:shd",            m_oShd );
				WritingElement_ReadNode( oNode, oChild, L"w:tblBorders",     m_oTblBorders );
				WritingElement_ReadNode( oNode, oChild, L"w:tblCellMar",     m_oTblCellMar );
				WritingElement_ReadNode( oNode, oChild, L"w:tblCellSpacing", m_oTblCellSpacing );
				WritingElement_ReadNode( oNode, oChild, L"w:tblInd",         m_oTblInd );
				WritingElement_ReadNode( oNode, oChild, L"w:tblLayout",      m_oTblLayout );
				WritingElement_ReadNode( oNode, oChild, L"w:tblLook",        m_oTblLook );

				if ( !m_bTblPrExChange && oNode.GetNode( L"w:tblPrExChange", oChild ) )
					m_oTblPrExChange = oChild;

				WritingElement_ReadNode( oNode, oChild, L"w:tblW",           m_oTblW );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if      ( L"w:jc"				== sName ) m_oJc = oReader;
					else if ( L"w:shd"				== sName ) m_oShd = oReader;
					else if ( L"w:tblBorders"		== sName ) m_oTblBorders = oReader;
					else if ( L"w:tblCellMar"		== sName ) m_oTblCellMar = oReader;
					else if ( L"w:tblCellSpacing"	== sName ) m_oTblCellSpacing = oReader;
					else if ( L"w:tblInd"			== sName ) m_oTblInd = oReader;
					else if ( L"w:tblLayout"		== sName ) m_oTblLayout = oReader;
					else if ( L"w:tblLook"			== sName ) m_oTblLook = oReader;
					else if ( !m_bTblPrExChange && L"w:tblPrExChange" == sName ) m_oTblPrExChange = oReader;
					else if ( L"w:tblW"				== sName ) m_oTblW = oReader;
				}
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = L"<w:tblPrEx>";

				WritingElement_WriteNode_1( L"<w:jc ",             m_oJc );
				WritingElement_WriteNode_1( L"<w:shd ",            m_oShd );
				WritingElement_WriteNode_2( m_oTblBorders );
				WritingElement_WriteNode_2( m_oTblCellMar );
				WritingElement_WriteNode_1( L"<w:tblCellSpacing ", m_oTblCellSpacing );
				WritingElement_WriteNode_1( L"<w:tblInd ",         m_oTblInd );
				WritingElement_WriteNode_1( L"<w:tblLayout ",      m_oTblLayout );
				WritingElement_WriteNode_1( L"<w:tblLook ",        m_oTblLook );

				if ( !m_bTblPrExChange && m_oTblPrExChange.IsInit() )
					sResult += m_oTblPrExChange->toXML();

				WritingElement_WriteNode_1( L"<w:tblW ",           m_oTblW );

				sResult += L"</w:tblPrEx>";

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_tblPrEx;
			}


			bool                                           m_bTblPrExChange;

			// Childs
			nullable<ComplexTypes::Word::CJcTable       > m_oJc;
			nullable<ComplexTypes::Word::CShading       > m_oShd;
			nullable<OOX::Logic::CTblBorders            > m_oTblBorders;
			nullable<OOX::Logic::CTblCellMar            > m_oTblCellMar;
			nullable<ComplexTypes::Word::CTblWidth      > m_oTblCellSpacing;
			nullable<ComplexTypes::Word::CTblWidth      > m_oTblInd;
			nullable<ComplexTypes::Word::CTblLayoutType > m_oTblLayout;
			nullable<ComplexTypes::Word::CTblLook       > m_oTblLook;
			nullable<OOX::Logic::CTblPrExChange         > m_oTblPrExChange;
			nullable<ComplexTypes::Word::CTblWidth      > m_oTblW;
		};

		//--------------------------------------------------------------------------------
		// CTbl 17.4.38 (Part 1)
		//--------------------------------------------------------------------------------
		class CTbl : public WritingElementWithChilds<>
		{
		public:
			CTbl(OOX::Document *pMain = NULL);
			CTbl(XmlUtils::CXmlNode &oNode);
			CTbl(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTbl();

			const CTbl &operator =(const XmlUtils::CXmlNode& oNode);
			const CTbl &operator =(const XmlUtils::CXmlLiteReader& oReader);

			virtual void ClearItems();
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			void CreateElements(XmlUtils::CXmlLiteReader &oReader, int Depth);
			
			int										m_nCountRow;

			nullable<OOX::Logic::CTblGrid>			m_oTblGrid;			
			OOX::Logic::CTableProperty*				m_oTableProperties; //todooo - выкинуть из m_arrItems, переделать на nullable<>
		};

		//--------------------------------------------------------------------------------
		// CTr 17.4.79 (Part 1)
		//--------------------------------------------------------------------------------
		class CTr : public WritingElementWithChilds<>
		{
		public:
			CTr(OOX::Document *pMain = NULL);
			CTr(XmlUtils::CXmlNode &oNode);
			CTr(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTr();

			const CTr &operator =(const XmlUtils::CXmlNode& oNode);
			const CTr &operator =(const XmlUtils::CXmlLiteReader& oReader);

			virtual void ClearItems();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void CreateElements(XmlUtils::CXmlLiteReader &oReader, int Depth);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			int										m_nCountCell;

			nullable<SimpleTypes::CLongHexNumber> m_oRsidDel;
			nullable<SimpleTypes::CLongHexNumber> m_oRsidR;
			nullable<SimpleTypes::CLongHexNumber> m_oRsidRPr;
			nullable<SimpleTypes::CLongHexNumber> m_oRsidTr;

			// Childs
			OOX::Logic::CTableRowProperties*	m_pTableRowProperties; //todooo - выкинуть из m_arrItems, переделать на nullable<>
		};

		//--------------------------------------------------------------------------------
		// CTc 17.4.66 (Part 1)
		//--------------------------------------------------------------------------------
		class CTc : public WritingElementWithChilds<>
		{
		public:
			CTc(OOX::Document *pMain = NULL);
			CTc(XmlUtils::CXmlNode &oNode);
			CTc(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTc();

			const CTc &operator =(const XmlUtils::CXmlNode& oNode);
			const CTc &operator =(const XmlUtils::CXmlLiteReader& oReader);

			virtual void ClearItems();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void CreateElements(XmlUtils::CXmlLiteReader &oReader, int Depth);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			int									m_nNumCol; // Номер колонки
			nullable<std::wstring >				m_sId;
			OOX::Logic::CTableCellProperties*	m_pTableCellProperties; //todooo - выкинуть из m_arrItems, переделать на nullable<>
		};

	} // namespace Logic
} // namespace OOX


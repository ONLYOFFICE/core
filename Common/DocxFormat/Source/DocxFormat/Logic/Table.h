#pragma once
#ifndef OOX_LOGIC_TABLE_INCLUDE_H_
#define OOX_LOGIC_TABLE_INCLUDE_H_

#include "../WritingElement.h"
#include "../../Common/SimpleTypes_Word.h"
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
			CTblGridCol()
			{
			}
			virtual ~CTblGridCol()
			{
			}

			virtual void    FromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("w:w"), m_oW );
			}
			virtual void    FromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:w"), m_oW )
				WritingElement_ReadAttributes_End( oReader )
			}
			virtual CString ToString() const
			{
				CString sResult;

				if ( m_oW.IsInit() )
				{
					sResult += "w:w=\"";
					sResult += m_oW->ToString();
					sResult += "\" ";
				}

				return sResult;
			}

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
			CTblGridChange();
			CTblGridChange(XmlUtils::CXmlNode &oNode);
			CTblGridChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTblGridChange();
			const CTblGridChange& operator = (const XmlUtils::CXmlNode &oNode);
			const CTblGridChange& operator = (const XmlUtils::CXmlLiteReader& oReader);
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return et_w_tblGridChange;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			// Attributes
			nullable<SimpleTypes::CDecimalNumber<> > m_oId;

			// Childs
			CTblGrid                                *m_pTblGrid;
		};
		//--------------------------------------------------------------------------------
		// CTblGrid 17.4.49 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblGrid : public WritingElement
		{
		public:
			CTblGrid()
			{
				m_bTblGridChange = false;
			}
			CTblGrid(XmlUtils::CXmlNode &oNode)
			{
				m_bTblGridChange = false;

				fromXML( oNode );
			}
			CTblGrid(XmlUtils::CXmlLiteReader& oReader)
			{
				m_bTblGridChange = false;

				fromXML( oReader );
			}
			virtual ~CTblGrid()
			{
			}
		public:

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
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;

				if ( !m_bTblGridChange && oNode.GetNode( _T("w:tblGridChange"), oChild ) )
					m_oTblGridChange = oChild;

				XmlUtils::CXmlNodes oGridColNodes;
				if ( oNode.GetNodes( _T("w:gridCol"), oGridColNodes ) )
				{
					XmlUtils::CXmlNode oGridColNode;
					for ( int nIndex = 0; nIndex < oGridColNodes.GetCount(); nIndex++ )
					{
						if ( oGridColNodes.GetAt( nIndex, oGridColNode ) )
						{
							ComplexTypes::Word::CTblGridCol oGridCol = oGridColNode;
							m_arrGridCol.Add( oGridCol );
						}
					}
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if ( !m_bTblGridChange && _T("w:tblGridChange") == sName )
						m_oTblGridChange = oReader;
					else if ( _T("w:gridCol") == sName )
					{
						ComplexTypes::Word::CTblGridCol oGridCol = oReader;
						m_arrGridCol.Add( oGridCol );
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:tblGrid>");

				if ( !m_bTblGridChange && m_oTblGridChange.IsInit() )
					sResult += m_oTblGridChange->toXML();

				for ( int nIndex = 0; nIndex < m_arrGridCol.GetSize(); nIndex++ )
				{
					sResult += _T("<w:gridCol ");
					sResult += m_arrGridCol[nIndex].ToString();
					sResult += _T("/>");
				}

				sResult += _T("</w:tblGrid>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_tblGrid;
			}

		public:

			bool                                           m_bTblGridChange;

			// Childs
			nullable<OOX::Logic::CTblGridChange          > m_oTblGridChange;
			CSimpleArray<ComplexTypes::Word::CTblGridCol > m_arrGridCol;
		};

		//--------------------------------------------------------------------------------
		// CTblPrExChange 17.13.5.35 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblPrEx;
		class CTblPrExChange : public WritingElement
		{
		public:
			CTblPrExChange();
			CTblPrExChange(XmlUtils::CXmlNode &oNode);
			CTblPrExChange(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTblPrExChange();
			const CTblPrExChange& operator = (const XmlUtils::CXmlNode &oNode);
			const CTblPrExChange& operator = (const XmlUtils::CXmlLiteReader& oReader);
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return et_w_tblPrExChange;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			// Attributes
			nullable<CString                       > m_sAuthor;
			nullable<SimpleTypes::CDateTime        > m_oDate;
			nullable<SimpleTypes::CDecimalNumber<> > m_oId;

			// Childs
			nullable<CTblPrEx                      > m_pTblPrEx;
		};
		//--------------------------------------------------------------------------------
		// CTblPrEx 17.4.61 (Part 1)
		//--------------------------------------------------------------------------------
		class CTblPrEx : public WritingElement
		{
		public:
			CTblPrEx()
			{
				m_bTblPrExChange = false;
			}
			CTblPrEx(XmlUtils::CXmlNode &oNode)
			{
				m_bTblPrExChange = false;

				fromXML( oNode );
			}
			CTblPrEx(XmlUtils::CXmlLiteReader& oReader)
			{
				m_bTblPrExChange = false;

				fromXML( oReader );
			}
			virtual ~CTblPrEx()
			{
			}
		public:

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
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlUtils::CXmlNode oChild;

				WritingElement_ReadNode( oNode, oChild, _T("w:jc"),             m_oJc );
				WritingElement_ReadNode( oNode, oChild, _T("w:shd"),            m_oShd );
				WritingElement_ReadNode( oNode, oChild, _T("w:tblBorders"),     m_oTblBorders );
				WritingElement_ReadNode( oNode, oChild, _T("w:tblCellMar"),     m_oTblCellMar );
				WritingElement_ReadNode( oNode, oChild, _T("w:tblCellSpacing"), m_oTblCellSpacing );
				WritingElement_ReadNode( oNode, oChild, _T("w:tblInd"),         m_oTblInd );
				WritingElement_ReadNode( oNode, oChild, _T("w:tblLayout"),      m_oTblLayout );
				WritingElement_ReadNode( oNode, oChild, _T("w:tblLook"),        m_oTblLook );

				if ( !m_bTblPrExChange && oNode.GetNode( _T("w:tblPrExChange"), oChild ) )
					m_oTblPrExChange = oChild;

				WritingElement_ReadNode( oNode, oChild, _T("w:tblW"),           m_oTblW );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if      ( _T("w:jc")             == sName ) m_oJc = oReader;
					else if ( _T("w:shd")            == sName ) m_oShd = oReader;
					else if ( _T("w:tblBorders")     == sName ) m_oTblBorders = oReader;
					else if ( _T("w:tblCellMar")     == sName ) m_oTblCellMar = oReader;
					else if ( _T("w:tblCellSpacing") == sName ) m_oTblCellSpacing = oReader;
					else if ( _T("w:tblInd")         == sName ) m_oTblInd = oReader;
					else if ( _T("w:tblLayout")      == sName ) m_oTblLayout = oReader;
					else if ( _T("w:tblLook")        == sName ) m_oTblLook = oReader;
					else if ( !m_bTblPrExChange && _T("w:tblPrExChange") == sName ) m_oTblPrExChange = oReader;
					else if ( _T("w:tblW")           == sName ) m_oTblW = oReader;
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<w:tblPrEx>");

				WritingElement_WriteNode_1( _T("<w:jc "),             m_oJc );
				WritingElement_WriteNode_1( _T("<w:shd "),            m_oShd );
				WritingElement_WriteNode_2( m_oTblBorders );
				WritingElement_WriteNode_2( m_oTblCellMar );
				WritingElement_WriteNode_1( _T("<w:tblCellSpacing "), m_oTblCellSpacing );
				WritingElement_WriteNode_1( _T("<w:tblInd "),         m_oTblInd );
				WritingElement_WriteNode_1( _T("<w:tblLayout "),      m_oTblLayout );
				WritingElement_WriteNode_1( _T("<w:tblLook "),        m_oTblLook );

				if ( !m_bTblPrExChange && m_oTblPrExChange.IsInit() )
					sResult += m_oTblPrExChange->toXML();

				WritingElement_WriteNode_1( _T("<w:tblW "),           m_oTblW );

				sResult += _T("</w:tblPrEx>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return et_w_tblPrEx;
			}

		public:

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
		class CTbl : public WritingElement
		{
		public:
			CTbl()
			{
			}
			CTbl(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CTbl(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CTbl()
			{
				for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.RemoveAll();
			}

		public:

			const CTbl &operator =(const XmlUtils::CXmlNode& oNode)
			{
				Clear();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}

			const CTbl &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				Clear();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

			void Clear()
			{	
				for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.RemoveAll();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return et_w_tbl;
			}

		public:

			// Childs
			CSimpleArray<WritingElement *> m_arrItems;
			nullable<OOX::Logic::CTblGrid> m_oTblGrid;
		};

		//--------------------------------------------------------------------------------
		// CTr 17.4.79 (Part 1)
		//--------------------------------------------------------------------------------
		class CTr : public WritingElement
		{
		public:
			CTr()
			{
			}
			CTr(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CTr(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CTr()
			{
				for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.RemoveAll();
			}

		public:

			const CTr &operator =(const XmlUtils::CXmlNode& oNode)
			{
				Clear();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CTr &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				Clear();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

			void Clear()
			{
				m_oRsidDel.reset();
				m_oRsidR.reset();
				m_oRsidRPr.reset();
				m_oRsidTr.reset();

				for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.RemoveAll();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return et_w_tr;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			// Attributes
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidDel;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidR;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidRPr;
			nullable<SimpleTypes::CLongHexNumber<> > m_oRsidTr;

			// Childs
			CSimpleArray<WritingElement *> m_arrItems;
		};

		//--------------------------------------------------------------------------------
		// CTc 17.4.66 (Part 1)
		//--------------------------------------------------------------------------------
		class CTc : public WritingElement
		{
		public:
			CTc()
			{
				m_nNumCol = 0;
			}
			CTc(XmlUtils::CXmlNode &oNode)
			{
				m_nNumCol = 0;
				fromXML( oNode );
			}
			CTc(XmlUtils::CXmlLiteReader& oReader)
			{
				m_nNumCol = 0;
				fromXML( oReader );
			}
			virtual ~CTc()
			{
				for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.RemoveAll();
			}

		public:

			const CTc &operator =(const XmlUtils::CXmlNode& oNode)
			{
				Clear();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CTc &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				Clear();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}
			void Clear()
			{
				m_sId.reset();

				for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.RemoveAll();
			}

			CTableCellProperties *GetProperties()
			{
				for ( int nIndex = m_arrItems.GetSize() - 1; nIndex >= 0; nIndex-- )
				{
					if ( m_arrItems[nIndex] && OOX::et_w_tcPr == m_arrItems[nIndex]->getType() )
					{
						return ((CTableCellProperties*)m_arrItems[nIndex]);
					}
				}

				return NULL;
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return et_w_tc;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			int                            m_nNumCol; // Номер колонки

			// Attributes
			nullable<CString >             m_sId;

			// Childs
			CSimpleArray<WritingElement *> m_arrItems;
		};

	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_INCLUDE_H_
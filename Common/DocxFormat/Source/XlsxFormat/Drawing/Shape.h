#pragma once
#ifndef OOX_SHAPE_FILE_INCLUDE_H_
#define OOX_SHAPE_FILE_INCLUDE_H_

#include "../CommonInclude.h"
//#include "../../DocxFormat/Logic/Paragraph.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//--------------------------------------------------------------------------------
		// 20.5.2.34 txBody (Shape Text Body)
		//--------------------------------------------------------------------------------	
		class CTextBody : public WritingElementWithChilds<Drawing::CParagraph>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CTextBody)
			CTextBody()
			{
			}
			virtual ~CTextBody()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("a:bodyPr") == sName )
						m_oBodyPr = oReader;
					//if ( _T("a:lstStyle") == sName )
					//	m_oLstStyle = oReader;
					else if ( _T("a:p") == sName )
						m_arrItems.Add( new Drawing::CParagraph( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_ShapeTextBody;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_End	( oReader )
			}
		public:
			nullable<OOX::Drawing::CTextBodyProperties>	m_oBodyPr;
			//nullable<OOX::Drawing::CTextListStyle>	m_oLstStyle;
		};
		//--------------------------------------------------------------------------------
		// 20.5.2.9 cNvSpPr (Connection Non-Visual Shape Properties)
		//--------------------------------------------------------------------------------	
		class CConnectionNonVisualShapeProps : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CConnectionNonVisualShapeProps)
			CConnectionNonVisualShapeProps()
			{
			}
			virtual ~CConnectionNonVisualShapeProps()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					sName = oReader.GetName();
					if ( _T("a:spLocks") == sName )
						m_oSpLocks = oReader;
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_ConnectionNonVisualShapeProps;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("txBox"), m_otxBox )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_otxBox;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable<OOX::Drawing::CPictureLocking>			m_oSpLocks;//todo CShapeLocking
		};

		//--------------------------------------------------------------------------------
		// 20.5.2.23 nvSpPr (Non-Visual Properties for a Shape)
		//--------------------------------------------------------------------------------	
		class CShapeNonVisual : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CShapeNonVisual)
			CShapeNonVisual()
			{
			}
			virtual ~CShapeNonVisual()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("xdr:cNvSpPr") == sName )
						m_oCNvSpPr = oReader;
					else if ( _T("xdr:cNvPr") == sName )
						m_oCNvPr = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_ShapeNonVisual;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Childs
			nullable<CConnectionNonVisualShapeProps>		m_oCNvSpPr;
			nullable<OOX::Drawing::CNonVisualDrawingProps>	m_oCNvPr;
		};

		//--------------------------------------------------------------------------------
		//			20.5.2.29 sp (Shape)
		//--------------------------------------------------------------------------------
		class CShape : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CShape)
			CShape()
			{
			}
			virtual ~CShape()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("xdr:nvSpPr") == sName )
						m_oNvSpPr = oReader;
					else if ( _T("xdr:spPr") == sName )
						m_oSpPr = oReader;
					else if ( _T("xdr:style") == sName )
						m_oShapeStyle = oReader;	
					else if (_T("xdr:txBody") == sName)
						m_oTxBody = oReader; 		
				}
			}

			virtual EElementType getType () const
			{
				return et_Shape;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CShapeNonVisual>					m_oNvSpPr;
			nullable<OOX::Drawing::CShapeProperties>	m_oSpPr;
			nullable<OOX::Drawing::CShapeStyle>			m_oShapeStyle;
			nullable<OOX::Spreadsheet::CTextBody>		m_oTxBody;	
		};
		
		//--------------------------------------------------------------------------------
		// 20.5.2.4 cNvCxnSpPr (Non-Visual Connector Shape Drawing Properties)
		//--------------------------------------------------------------------------------	
		class CConnectionNonVisualConnShapeProps : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CConnectionNonVisualConnShapeProps)
			CConnectionNonVisualConnShapeProps()
			{
			}
			virtual ~CConnectionNonVisualConnShapeProps()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					sName = oReader.GetName();
					if ( _T("a:picLocks") == sName )
						m_oPicLocks = oReader;
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_ConnectionNonVisualConnShapeProps;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("preferRelativeResize"), m_oPreferRelativeResize )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffTrue> m_oPreferRelativeResize;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable<OOX::Drawing::CPictureLocking>         m_oPicLocks;
		};

		//--------------------------------------------------------------------------------
		// 20.5.2.19 nvCxnSpPr (Non-Visual Properties for a Connection Shape)
		//--------------------------------------------------------------------------------	
		class CConnShapeNonVisual : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CConnShapeNonVisual)
			CConnShapeNonVisual()
			{
			}
			virtual ~CConnShapeNonVisual()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("xdr:cNvCxnSpPr") == sName )
						m_oCNvConnSpPr = oReader;
					else if ( _T("xdr:cNvPr") == sName )
						m_oCNvPr = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_ConnShapeNonVisual;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Childs
			nullable<CConnectionNonVisualConnShapeProps>	m_oCNvConnSpPr;
			nullable<OOX::Drawing::CNonVisualDrawingProps>	m_oCNvPr;
		};


		//--------------------------------------------------------------------------------
		//20.5.2.13 cxnSp (Connection Shape)
		//--------------------------------------------------------------------------------
		class CConnShape : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CConnShape)
			CConnShape()
			{
			}
			virtual ~CConnShape()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

					if ( _T("xdr:nvCxnSpPr") == sName )
						m_oNvConnSpPr = oReader;
					if ( _T("xdr:spPr") == sName )
						m_oSpPr = oReader;
					if ( _T("xdr:style") == sName )
						m_oShapeStyle = oReader;				}
			}

			virtual EElementType getType () const
			{
				return et_ConnShape;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CConnShapeNonVisual>				m_oNvConnSpPr;
			nullable<OOX::Drawing::CShapeProperties>	m_oSpPr;
			nullable<OOX::Drawing::CShapeStyle>			m_oShapeStyle;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SHAPE_FILE_INCLUDE_H_
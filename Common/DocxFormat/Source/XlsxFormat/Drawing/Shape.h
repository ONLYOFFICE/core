#pragma once
#ifndef OOX_SHAPE_FILE_INCLUDE_H_
#define OOX_SHAPE_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
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
					if ( _T("a:picLocks") == sName )
						m_oPicLocks = oReader;
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
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("preferRelativeResize"), m_oPreferRelativeResize )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			EElementType      m_eType;
			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffTrue> m_oPreferRelativeResize;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			nullable<OOX::Drawing::CPictureLocking>         m_oPicLocks;
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
			EElementType      m_eType;
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
					if ( _T("xdr:spPr") == sName )
						m_oSpPr = oReader;
					if ( _T("xdr:style") == sName )
						m_oShapeStyle = oReader;				}
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
			//txBody (Shape Text Body)
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SHAPE_FILE_INCLUDE_H_
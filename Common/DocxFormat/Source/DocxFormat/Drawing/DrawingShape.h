#pragma once

#include "../../Base/Nullable.h"
#include "../../Common/SimpleTypes_Drawing.h"
#include "../../Common/SimpleTypes_Shared.h"

#include "../WritingElement.h"

#include "DrawingCoreInfo.h"
#include "DrawingText.h"

namespace OOX
{
	namespace Drawing
	{

		//--------------------------------------------------------------------------------
		// 20.1.2.2.27 nvGrpSpPr (Non-Visual Properties for a Group Shape)
		//--------------------------------------------------------------------------------	
		class CGroupShapeNonVisual : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGroupShapeNonVisual)
			CGroupShapeNonVisual()
			{
			}
			virtual ~CGroupShapeNonVisual()
			{
			}
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
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
                    CString sName = oReader.GetName();

					//if ( _T("a:cNvGrpSpPr") == sName )
					//	m_oCNvGroupSpPr = oReader;
					//else 
						if ( _T("a:cNvPr") == sName )
						m_oCNvPr = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_a_GroupShapeNonVisual;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Childs
			//nullable<OOX::Drawing::CNonVisualGroupShapeDrawingProps>	m_oCNvGroupSpPr;
			nullable<OOX::Drawing::CNonVisualDrawingProps>				m_oCNvPr;
		};
		//--------------------------------------------------------------------------------
		// 20.5.2.9 cNvSpPr (Connection Non-Visual Shape Properties)
		//--------------------------------------------------------------------------------	
		class CConnectionNonVisualShapeProps : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CConnectionNonVisualShapeProps)
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
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
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
                    CString sName = oReader.GetName();

					sName = oReader.GetName();
					if ( _T("a:spLocks") == sName )
						m_oSpLocks = oReader;
					else if ( _T("a:extLst") == sName )
						m_oExtLst = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_a_ConnectionNonVisualShapeProps;
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
			WritingElement_AdditionConstructors(CShapeNonVisual)
			CShapeNonVisual()
			{
			}
			virtual ~CShapeNonVisual()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
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
                    CString sName = oReader.GetName();

					if ( _T("a:cNvSpPr") == sName )
						m_oCNvSpPr = oReader;
					else if ( _T("a:cNvPr") == sName )
						m_oCNvPr = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_a_ShapeNonVisual;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Childs
			nullable<OOX::Drawing::CConnectionNonVisualShapeProps>	m_oCNvSpPr;
			nullable<OOX::Drawing::CNonVisualDrawingProps>			m_oCNvPr;
		};

		//--------------------------------------------------------------------------------
		//		20.1.2.2.33 sp (Shape)
		//--------------------------------------------------------------------------------
		class CShape : public WritingElement
		{
		public:
			CShape(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CShape(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
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
			virtual void         fromXML(XmlUtils::CXmlNode& node)
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
                    CString sName = oReader.GetName();

					if ( _T("a:spPr") == sName )
						m_oSpPr = oReader;
					//else if ( _T("a:nvSpPr") == sName )
					//	m_oNvSpPr = oReader;
					else if ( _T("a:style") == sName )
						m_oShapeStyle = oReader;	
					else if (_T("a:txSp") == sName)
						m_oTxSp = oReader; 		
				}
			}

			virtual EElementType getType () const
			{
				return et_a_Shape;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<OOX::Drawing::CShapeNonVisual>			m_oNvSpPr;

			nullable<OOX::Drawing::CShapeProperties>		m_oSpPr;
			nullable<OOX::Drawing::CShapeStyle>				m_oShapeStyle;
			nullable<OOX::Drawing::CTxSp>					m_oTxSp;	

//extLst (Extension List) §20.1.2.2.15

		};

		//--------------------------------------------------------------------------------
		//		20.3.2.1 lockedCanvas (Locked Canvas Container)
		//--------------------------------------------------------------------------------
		class CLockedCanvas : public WritingElementWithChilds<WritingElement>
		{
		public:
			CLockedCanvas(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CLockedCanvas(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			CLockedCanvas()
			{
			}
			virtual ~CLockedCanvas()
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
			virtual void         fromXML(XmlUtils::CXmlNode& node)
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
                    CString sName = oReader.GetName();

					if ( _T("a:grpSpPr") == sName )
						m_oGroupSpPr = oReader;
					else if ( _T("a:nvGrpSpPr") == sName )
						m_oNvGroupSpPr = oReader;
					else if (_T("a:txSp") == sName)
						m_oTxSp = oReader; 		

					else if ( _T("a:sp") == sName )
						m_arrItems.push_back( new OOX::Drawing::CShape( oReader ));
					else if ( _T("pic:pic") == sName )//???
						m_arrItems.push_back( new OOX::Drawing::CPicture( oReader ));
					//else if ( _T("a:grpSp") == sName )//????
					//	m_arrItems.Add( new OOX::Drawing::CGroupShape( oReader ));	

				}
			}

			virtual EElementType getType () const
			{
				return et_a_LockedCanvas;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<OOX::Drawing::CGroupShapeProperties>		m_oGroupSpPr;
			nullable<OOX::Drawing::CGroupShapeNonVisual>		m_oNvGroupSpPr;
			nullable<OOX::Drawing::CShapeProperties>			m_oSpPr;
			nullable<OOX::Drawing::CTxSp>						m_oTxSp;	

//extLst (Extension List) §20.1.2.2.15

		};

		//--------------------------------------------------------------------------------
		//		21.4.2.22 relIds (Explicit Relationships to Diagram Parts)
		//--------------------------------------------------------------------------------
		class CDiagrammParts : public WritingElement
		{
		public:
			CDiagrammParts(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CDiagrammParts(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			CDiagrammParts()
			{
			}
			virtual ~CDiagrammParts()
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
			virtual void         fromXML(XmlUtils::CXmlNode& node)
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
                    CString sName = oReader.GetName();

				}
			}

			virtual EElementType getType () const
			{
				return et_dgm_DiagrammParts;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if	  ( oReader, _T("r:cs"), m_oRId_Colors)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:dm"), m_oRId_Data)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:lo"), m_oRId_Layout)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:qs"), m_oRId_Style)
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId> m_oRId_Colors;
			nullable<SimpleTypes::CRelationshipId> m_oRId_Data;
			nullable<SimpleTypes::CRelationshipId> m_oRId_Layout;
			nullable<SimpleTypes::CRelationshipId> m_oRId_Style;
		};


	} // namespace Drawing
} // namespace OOX


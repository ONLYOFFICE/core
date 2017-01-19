/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
            virtual std::wstring      toXML() const
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
                    std::wstring sName = oReader.GetName();

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
            virtual std::wstring      toXML() const
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
                    std::wstring sName = oReader.GetName();

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
            virtual std::wstring      toXML() const
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
                    std::wstring sName = oReader.GetName();

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
            virtual std::wstring      toXML() const
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
                    std::wstring sName = oReader.GetName();

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

//extLst (Extension List) ยง20.1.2.2.15

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
            virtual std::wstring      toXML() const
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
                    std::wstring sName = oReader.GetName();

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

//extLst (Extension List) ยง20.1.2.2.15

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
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
            virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    std::wstring sName = oReader.GetName();

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


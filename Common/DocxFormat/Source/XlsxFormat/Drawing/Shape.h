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
#ifndef OOX_SHAPE_FILE_INCLUDE_H_
#define OOX_SHAPE_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//--------------------------------------------------------------------------------
		// 20.5.2.34	txBody	(Shape Text Body)
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
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("bodyPr") == sName )
						m_oBodyPr = oReader;
					//if ( _T("lstStyle") == sName )
					//	m_oLstStyle = oReader;
					else if ( _T("p") == sName )
						m_arrItems.push_back( new Drawing::CParagraph( oReader ));
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
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("spLocks") == sName )
						m_oSpLocks = oReader;
					else if ( _T("extLst") == sName )
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
		// 20.5.2.6 cNvGrpSpPr (Non-Visual Group Shape Drawing Properties)
		//--------------------------------------------------------------------------------	
		class CNonVisualGroupShapeDrawingProps : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CNonVisualGroupShapeDrawingProps)
			CNonVisualGroupShapeDrawingProps()
			{
			}
			virtual ~CNonVisualGroupShapeDrawingProps()
			{
			}

		public:
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("extLst") == sName )
						m_oExtLst = oReader;
					//else ( _T("grpSpLocks") == sName )
					//	m_oSpLocks = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_NonVisualGroupShapeDrawingProps;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Attributes
			SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_otxBox;

			// Childs
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			//nullable<OOX::Drawing::CPictureLocking>			m_oSpLocks;//todo grpSpLocks
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
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("cNvSpPr") == sName )
						m_oCNvSpPr = oReader;
					else if ( _T("cNvPr") == sName )
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
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("nvSpPr") == sName )
						m_oNvSpPr = oReader;
					else if ( _T("spPr") == sName )
						m_oSpPr = oReader;
					else if ( _T("style") == sName )
						m_oShapeStyle = oReader;	
					else if (_T("txBody") == sName)
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
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("picLocks") == sName )
						m_oPicLocks = oReader;
					else if ( _T("extLst") == sName )
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
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("cNvCxnSpPr") == sName )
						m_oCNvConnSpPr = oReader;
					else if ( _T("cNvPr") == sName )
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
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("nvCxnSpPr") == sName )
						m_oNvConnSpPr = oReader;
					if ( _T("spPr") == sName )
						m_oSpPr = oReader;
					if ( _T("style") == sName )
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
		//--------------------------------------------------------------------------------
		// 20.5.2.21 nvGrpSpPr (Non-Visual Properties for a Group Shape)
		//--------------------------------------------------------------------------------	
		class CGroupShapeNonVisual : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGroupShapeNonVisual)
			CGroupShapeNonVisual()
			{
			}
			virtual ~CGroupShapeNonVisual()
			{
			}

		public:
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("cNvGrpSpPr") == sName )
						m_oCNvGroupSpPr = oReader;
					else if ( _T("cNvPr") == sName )
						m_oCNvPr = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_GroupShapeNonVisual;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Childs
			nullable<CNonVisualGroupShapeDrawingProps>	m_oCNvGroupSpPr;
			nullable<OOX::Drawing::CNonVisualDrawingProps>	m_oCNvPr;
		};


		//--------------------------------------------------------------------------------
		//			20.5.2.17 grpSp (Group Shape)
		//--------------------------------------------------------------------------------
		class CGroupShape : public WritingElementWithChilds<WritingElement>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGroupShape)
			CGroupShape()
			{
			}
			virtual ~CGroupShape()
			{
			}

		public:
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("nvGrpSpPr") == sName )
						m_oNvGroupSpPr = oReader;
					else if ( _T("grpSpPr") == sName )
						m_oGroupSpPr = oReader;

					else if ( _T("sp") == sName )
						m_arrItems.push_back( new OOX::Spreadsheet::CShape( oReader ));
					else if ( _T("cxnSp") == sName )
						m_arrItems.push_back( new OOX::Spreadsheet::CConnShape( oReader ));
					else if ( _T("pic") == sName )
						m_arrItems.push_back( new OOX::Spreadsheet::CPic( oReader ));
					else if ( _T("grpSp") == sName )
						m_arrItems.push_back( new OOX::Spreadsheet::CGroupShape( oReader ));		
				}
			}

			virtual EElementType getType () const
			{
				return et_GroupShape;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<OOX::Drawing::CGroupShapeProperties>	m_oGroupSpPr;
			nullable<CGroupShapeNonVisual>					m_oNvGroupSpPr;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SHAPE_FILE_INCLUDE_H_

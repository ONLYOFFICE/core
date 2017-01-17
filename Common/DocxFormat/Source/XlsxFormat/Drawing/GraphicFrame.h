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
#ifndef OOX_GRAPHICFRAME_FILE_INCLUDE_H_
#define OOX_GRAPHICFRAME_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//--------------------------------------------------------------------------------
		// 20.1.2.2.5 cNvGraphicFramePr (Non-Visual Graphic Frame Drawing Properties)
		//--------------------------------------------------------------------------------	
		class CConnectionNonVisualGraphicFrameProps : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CConnectionNonVisualGraphicFrameProps)
			CConnectionNonVisualGraphicFrameProps()
			{
			}
			virtual ~CConnectionNonVisualGraphicFrameProps()
			{
			}

		public:
            virtual std::wstring      toXML() const
			{
				return _T("<xdr:cNvGraphicFramePr/>");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<xdr:cNvGraphicFramePr/>"));
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
				return et_ConnectionNonVisualGraphicFrameProps;
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
		// 20.1.2.2.26 nvGraphicFramePr (Non-Visual Properties for a Graphic Frame)
		//--------------------------------------------------------------------------------	
		class CGraphicFrameNonVisual : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGraphicFrameNonVisual)
			CGraphicFrameNonVisual()
			{
			}
			virtual ~CGraphicFrameNonVisual()
			{
			}

		public:
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<xdr:nvGraphicFramePr>"));

				if (m_oCNvPr.IsInit())
				{
                    std::wstring sCNvPr = m_oCNvPr->toXML();
                    writer.WriteString(sCNvPr.c_str());
				}
                else
                    writer.WriteString(_T("<xdr:cNvPr id=\"1\" name=\"diagram\"/>"));

				if (m_oCNvGraphicFramePr.IsInit())	m_oCNvGraphicFramePr->toXML(writer);
				
				writer.WriteString(_T("</xdr:nvGraphicFramePr>"));
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

					if ( _T("cNvGraphicFramePr") == sName )
						m_oCNvGraphicFramePr = oReader;
					else if ( _T("cNvPr") == sName )
						m_oCNvPr = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_GraphicFrameNonVisual;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Childs
			nullable<CConnectionNonVisualGraphicFrameProps>	m_oCNvGraphicFramePr;
			nullable<OOX::Drawing::CNonVisualDrawingProps>	m_oCNvPr;
		};

		//"c:chart"  - нужно перенести в общую часть 
		class CGraphicChart : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGraphicChart)
			CGraphicChart()
			{
			}
			virtual ~CGraphicChart()
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

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_GraphicChart;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),      m_oRId )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId>				m_oRId;
		};

		//--------------------------------------------------------------------------------
		// 20.1.2.2.17 graphicData (Graphic Object Data)
		//--------------------------------------------------------------------------------	
		class CGraphicData : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGraphicData)
			CGraphicData()
			{
			}
			virtual ~CGraphicData()
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

					if ( _T("chart") == sName )
					{
						m_oChart = oReader;
					}
					else if (_T("legacyDrawing") == sName )
					{
						ReadAttributes( oReader );
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_xdr_GraphicData;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("spid"),  m_sSpId)


				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<CGraphicChart>		m_oChart;
			nullable<std::wstring>			m_sSpId;
			//Any element in any namespace
		};
		
		//--------------------------------------------------------------------------------
		// 20.1.2.2.16 graphic (Graphic Object) - тю ..это может быть не только чарт !!
		//--------------------------------------------------------------------------------		
		class CChartGraphic : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CChartGraphic)
			CChartGraphic()
			{
			}
			virtual ~CChartGraphic()
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

					if ( _T("graphicData") == sName )
						m_oGraphicData = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_xdr_GraphicFrame;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CGraphicData>		m_oGraphicData;
		};

		//--------------------------------------------------------------------------------
		// 20.1.2.2.18 graphicFrame (Graphic Frame)
		//--------------------------------------------------------------------------------			
		class CGraphicFrame : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CGraphicFrame)
			CGraphicFrame()
			{
			}
			virtual ~CGraphicFrame()
			{
			}

		public:
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oChartGraphic.IsInit() && m_oChartGraphic->m_oGraphicData.IsInit() && m_oChartGraphic->m_oGraphicData->m_oChart.IsInit() && m_oChartGraphic->m_oGraphicData->m_oChart->m_oRId.IsInit())
				{
					writer.WriteString(_T("<xdr:graphicFrame macro=\"\">"));

						if (m_oNvGraphicFramePr.IsInit())
						{
							m_oNvGraphicFramePr->toXML(writer);
						}else
						{
							writer.WriteString(_T("<xdr:nvGraphicFramePr><xdr:cNvPr id=\"1\" name=\"diagram\"/><xdr:cNvGraphicFramePr/></xdr:nvGraphicFramePr>"));
						}
						writer.WriteString(_T("<xdr:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/></xdr:xfrm>"));
						writer.WriteString(_T("<a:graphic><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/chart\">"));
						writer.WriteString(_T("<c:chart xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "));
							writer.WriteString(_T("r:id=\""));
								writer.WriteString(m_oChartGraphic->m_oGraphicData->m_oChart->m_oRId->ToString());
							writer.WriteString(_T("\"/>"));
						writer.WriteString(_T("</a:graphicData>"));
						writer.WriteString(_T("</a:graphic>"));
					writer.WriteString(_T("</xdr:graphicFrame>"));
					writer.WriteString(_T("<xdr:clientData/>"));
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				m_sXml.Init();
                m_sXml->append(oReader.GetOuterXml());

                std::wstring sXml = L"<root xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">";
                sXml += m_sXml.get();
                sXml += L"</root>";
				XmlUtils::CXmlLiteReader oSubReader;
				oSubReader.FromString(sXml);
				oSubReader.ReadNextNode();//root
				oSubReader.ReadNextNode();//GraphicFrame

				int nCurDepth = oSubReader.GetDepth();
				while( oSubReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oSubReader.GetName());

					if ( _T("graphic") == sName )
						m_oChartGraphic = oSubReader;
					else if ( _T("nvGraphicFramePr") == sName )
						m_oNvGraphicFramePr = oSubReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_xdr_GraphicFrame;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CChartGraphic>				m_oChartGraphic;
			nullable<CGraphicFrameNonVisual>	m_oNvGraphicFramePr;
			//xfrm
			//extLst
            nullable<std::wstring>					m_sXml;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_GRAPHICFRAME_FILE_INCLUDE_H_

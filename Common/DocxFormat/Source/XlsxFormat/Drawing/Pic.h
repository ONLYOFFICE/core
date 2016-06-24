/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#ifndef OOX_PIC_FILE_INCLUDE_H_
#define OOX_PIC_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//--------------------------------------------------------------------------------
		// 20.5.2.7 cNvPicPr (Non-Visual Picture Drawing Properties)
		//--------------------------------------------------------------------------------	
		class CNonVisualPictureDrawingProps : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CNonVisualPictureDrawingProps)
			CNonVisualPictureDrawingProps()
			{
			}
			virtual ~CNonVisualPictureDrawingProps()
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
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("picLocks") == sName )
						m_oPicLocks = oReader;
					else if ( _T("extLst") == sName )
						m_oExtLst = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_NonVisualPictureDrawingProps;
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
		// 20.5.2.22 nvPicPr (Non-Visual Properties for a Picture)
		//--------------------------------------------------------------------------------	
		class CPictureNonVisual : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CPictureNonVisual)
			CPictureNonVisual()
			{
			}
			virtual ~CPictureNonVisual()
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
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("cNvPicPr") == sName )
						m_oCNvPicPr = oReader;
					else if ( _T("cNvPr") == sName )
						m_oCNvPr = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_PictureNonVisual;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			EElementType									m_eType;
			// Childs
			nullable<CNonVisualPictureDrawingProps>			m_oCNvPicPr;
			nullable<OOX::Drawing::CNonVisualDrawingProps>	m_oCNvPr;
		};

		class CBlipFill : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CBlipFill)
			CBlipFill()
			{
			}
			virtual ~CBlipFill()
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
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("blip") == sName )
						m_oBlip = oReader;
					else if ( _T("srcRect") == sName )
						m_oSrcRect = oReader;
					else if ( _T("stretch") == sName )
						m_oStretch      = oReader;
					else if ( _T("tile") == sName )
						m_oTile         = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_BlipFill;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("dpi"),          m_oDpi )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("rotWithShape"), m_oRotWithShape )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			// Attributes
			nullable<SimpleTypes::CDecimalNumber<>>        m_oDpi;
			nullable<SimpleTypes::COnOff<>>                m_oRotWithShape;
			// Childs
			nullable<OOX::Drawing::CBlip>					m_oBlip;
			nullable<OOX::Drawing::CRelativeRect>			m_oSrcRect;
			nullable<OOX::Drawing::CTileInfoProperties>		m_oTile;
			nullable<OOX::Drawing::CStretchInfoProperties>	m_oStretch;
		};

		//--------------------------------------------------------------------------------
		//			20.5.2.25 pic (Picture)
		//--------------------------------------------------------------------------------
		class CPic : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CPic)
			CPic()
			{
			}
			virtual ~CPic()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oBlipFill.IsInit() && m_oBlipFill->m_oBlip.IsInit())
				{
					CString sRes;
					sRes.Format(_T("<xdr:pic><xdr:nvPicPr><xdr:cNvPr id=\"1\" name=\"image\"/><xdr:cNvPicPr><a:picLocks noChangeAspect=\"1\"/></xdr:cNvPicPr></xdr:nvPicPr><xdr:blipFill><a:blip xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:embed=\"%ls\" cstate=\"print\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:stretch><a:fillRect/></a:stretch></xdr:blipFill><xdr:spPr><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></xdr:spPr></xdr:pic><xdr:clientData/>"), m_oBlipFill->m_oBlip->m_oEmbed.GetValue());
					writer.WriteString(sRes);
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("blipFill") == sName )
						m_oBlipFill = oReader;
					if ( _T("nvPicPr") == sName )
						m_oNvPicPr = oReader;
					if ( _T("spPr") == sName )
						m_oSpPr = oReader;
					if ( _T("style") == sName )
						m_oShapeStyle = oReader;				
				}
			}

			virtual EElementType getType () const
			{
				return et_Pic;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CBlipFill>							m_oBlipFill;
			nullable<CPictureNonVisual>					m_oNvPicPr;
			nullable<OOX::Drawing::CShapeProperties>	m_oSpPr;
			nullable<OOX::Drawing::CShapeStyle>			m_oShapeStyle;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_PIC_FILE_INCLUDE_H_
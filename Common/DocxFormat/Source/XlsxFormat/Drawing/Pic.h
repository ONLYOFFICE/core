#pragma once
#ifndef OOX_PIC_FILE_INCLUDE_H_
#define OOX_PIC_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	using namespace Drawing;

	namespace Spreadsheet
	{
		//--------------------------------------------------------------------------------
		// Non-Visual Properties for a Picture  20.5.2.22
		//--------------------------------------------------------------------------------	
		class CNonVisualDrawingProps : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CNonVisualDrawingProps)
			CNonVisualDrawingProps()
			{
			}
			virtual ~CNonVisualDrawingProps()
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

					//if ( _T("xdr:cNvPicPr") == sName )
					//	m_oCNvPicPr = oReader;
					//else if ( _T("xdr:cNvPr") == sName )
					//	m_oCNvPr = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_NonVisualDrawingProps;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("descr"),  m_sDescr )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("hidden"), m_oHidden )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("id"),     m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("name"),   m_sName )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("title"),  m_sTitle )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			EElementType      m_eType;
			// Attributes
			nullable<CString>                               m_sDescr;
			nullable<SimpleTypes::COnOff<>>                 m_oHidden;
			nullable<SimpleTypes::CDrawingElementId<>>      m_oId;
			nullable<CString>                               m_sName;
			nullable<CString>                               m_sTitle;

			// Childs
			//nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
			//nullable<OOX::Drawing::CHyperlink             > m_oHlinkClick;
			//nullable<OOX::Drawing::CHyperlink             > m_oHlinkHover;
		};
		//--------------------------------------------------------------------------------
		// Non-Visual Picture Drawing Properties 20.5.2.5
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
					CWCharWrapper sName = oReader.GetName();

/*					if ( _T("xdr:cNvPicPr") == sName )
						m_oCNvPicPr = oReader;
					else */if ( _T("xdr:cNvPr") == sName )
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
			EElementType      m_eType;
			// Childs
			//nullable<CNonVisualPictureProperties> m_oCNvPicPr;
			nullable<CNonVisualDrawingProps>      m_oCNvPr;
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
					CWCharWrapper sName = oReader.GetName();

					if ( _T("a:blip") == sName )
						m_oBlip = oReader;
					else if ( _T("a:srcRect") == sName )
						m_oSrcRect = oReader;
					else if ( _T("a:stretch") == sName )
						m_oStretch      = oReader;
					else if ( _T("a:tile") == sName )
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
			nullable<CBlip>									m_oBlip;
			nullable<OOX::Drawing::CRelativeRect>			m_oSrcRect;
			nullable<OOX::Drawing::CTileInfoProperties>		m_oTile;
			nullable<OOX::Drawing::CStretchInfoProperties>	m_oStretch;
		};
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
					sRes.Format(_T("<xdr:pic><xdr:nvPicPr><xdr:cNvPr id=\"1\" name=\"image\"/><xdr:cNvPicPr><a:picLocks noChangeAspect=\"1\"/></xdr:cNvPicPr></xdr:nvPicPr><xdr:blipFill><a:blip xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:embed=\"%s\" cstate=\"print\"><a:extLst><a:ext uri=\"{28A0092B-C50C-407E-A947-70E740481C1C}\"><a14:useLocalDpi xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" val=\"0\"/></a:ext></a:extLst></a:blip><a:stretch><a:fillRect/></a:stretch></xdr:blipFill><xdr:spPr><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></xdr:spPr></xdr:pic><xdr:clientData/>"), m_oBlipFill->m_oBlip->m_oEmbed.GetValue());
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
					CWCharWrapper sName = oReader.GetName();

					if ( _T("xdr:blipFill") == sName )
						m_oBlipFill = oReader;
					if ( _T("xdr:nvPicPr") == sName )
						m_oNvPicPr = oReader;
					//if ( _T("xdr:spPr") == sName )
					//	m_oBlipFill = oReader;
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
			nullable<CBlipFill>						m_oBlipFill;
			nullable<CPictureNonVisual>				m_oNvPicPr;
			//nullable<CNonVisualDrawingProps>		m_oCSpPr;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_PIC_FILE_INCLUDE_H_
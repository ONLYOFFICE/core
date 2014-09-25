#pragma once

#ifndef OOX_FONT_INCLUDE_H_
#define OOX_FONT_INCLUDE_H_

#include "../Base/Nullable.h"
#include "WritingElement.h"

#include "../Common/SimpleTypes_Word.h"
#include "../Common/SimpleTypes_Shared.h"

namespace OOX
{
	class CFont : public WritingElement
	{
	public:
		CFont() 
		{
			m_sName = _T("Arial");
		}

		virtual ~CFont() {}
	public:
		virtual void         fromXML(XmlUtils::CXmlNode& oNode)
		{
			Reset();
			if ( _T("w:font") == oNode.GetName() )
			{
				oNode.ReadAttributeBase( _T("w:name"), m_sName );

				XmlUtils::CXmlNode oChild;

				if ( oNode.GetNode( _T("w:altName"), oChild ) )
					oChild.ReadAttributeBase( _T("w:val"), m_oAltName );

				if ( oNode.GetNode( _T("w:charset"), oChild ) )
					oChild.ReadAttributeBase( _T("w:val"), m_oCharset );

				if ( oNode.GetNode( _T("w:family"), oChild ) )
					oChild.ReadAttributeBase( _T("w:val"), m_oFamily );

				if ( oNode.GetNode( _T("w:notTrueType"), oChild ) )
					oChild.ReadAttributeBase( _T("w:val"), m_oNotTrueType );

				if ( oNode.GetNode( _T("w:panose1"), oChild ) )
					oChild.ReadAttributeBase( _T("w:val"), m_oPanose );

				if ( oNode.GetNode( _T("w:pitch"), oChild ) )
					oChild.ReadAttributeBase( _T("w:val"), m_oPitch );

				if ( oNode.GetNode( _T("w:sig"), oChild ) )
				{
					oChild.ReadAttributeBase( _T("w:csb0"), m_oCsb0 );
					oChild.ReadAttributeBase( _T("w:csb1"), m_oCsb1 );

					oChild.ReadAttributeBase( _T("w:usb0"), m_oUsb0 );
					oChild.ReadAttributeBase( _T("w:usb1"), m_oUsb1 );
					oChild.ReadAttributeBase( _T("w:usb2"), m_oUsb2 );
					oChild.ReadAttributeBase( _T("w:usb3"), m_oUsb3 );
				}

			}
		}
		virtual CString      toXML  () const
		{
			CString sResult = _T("<w:font w:name=\"");
			sResult += m_sName;
			sResult += _T("\">");

			if ( m_oAltName.IsInit() )
			{
				sResult += _T("<w:altName w:val=\"");
				sResult += m_oAltName.get();
				sResult += _T("\"/>");
			}

			if(m_oCharset.IsInit())
			{
				sResult += _T("<w:charset w:val=\"");
				sResult += m_oCharset->ToString();
				sResult += _T("\"/>");
			}
			if(m_oFamily.IsInit())
			{
				sResult += _T("<w:family w:val=\"");
				sResult += m_oFamily->ToString();
				sResult += _T("\"/>");
			}


			if ( m_oNotTrueType.IsInit() )
			{
				sResult += _T("<w:notTrueType w:val=\"");
				sResult += m_oNotTrueType->ToString();
				sResult += _T("\"/>");
			}

			if ( m_oPanose.IsInit() )
			{
				sResult += _T("<w:panose1 w:val=\"");
				sResult += m_oPanose->ToString();
				sResult += _T("\"/>");
			}
			if(m_oPitch.IsInit())
			{
				sResult += _T("<w:pitch w:val=\"");
				sResult += m_oPitch->ToString();
				sResult += _T("\"/>");
			}

			if ( m_oCsb0.IsInit() && m_oCsb1.IsInit() && m_oUsb0.IsInit() && m_oUsb1.IsInit() && m_oUsb2.IsInit() && m_oUsb3.IsInit() )
			{
				sResult += _T("<w:sig w:usb0=\"");
				sResult += m_oUsb0->ToString();
				sResult += _T("\" w:usb1=\"");
				sResult += m_oUsb1->ToString();
				sResult += _T("\" w:usb2=\"");
				sResult += m_oUsb2->ToString();
				sResult += _T("\" w:usb3=\"");
				sResult += m_oUsb3->ToString();
				sResult += _T("\" w:csb0=\"");
				sResult += m_oCsb0->ToString();
				sResult += _T("\" w:csb1=\"");
				sResult += m_oCsb1->ToString();
				sResult += _T("\"/>");
			}

			sResult += _T("</w:font>");

			return sResult;
		}



		virtual EElementType getType() const
		{
			return et_w_font;
		}
	private:

		void Reset()
		{
			m_sName = _T("Arial");
			m_oCharset.reset();
			m_oFamily.reset();
			m_oPitch.reset();
			m_oAltName.reset();
			m_oNotTrueType.reset();
			m_oPanose.reset();
			m_oCsb0.reset();
			m_oCsb1.reset();
			m_oUsb0.reset();
			m_oUsb1.reset();
			m_oUsb2.reset();
			m_oUsb3.reset();
		}

	public:

		CString                                                 m_sName;           
		nullable<SimpleTypes::CFontCharset<SimpleTypes::fontcharsetANSI>>	m_oCharset;  
		nullable<SimpleTypes::CFontFamily<SimpleTypes::fontfamilyAuto>>		m_oFamily;
		nullable<SimpleTypes::CPitch<SimpleTypes::pitchDefault>>			m_oPitch;

		nullable<CString                                      > m_oAltName;        
		nullable<SimpleTypes::COnOff<>                        > m_oNotTrueType;
		nullable<SimpleTypes::CPanose                         > m_oPanose;

		nullable<SimpleTypes::CLongHexNumber<>>                 m_oCsb0;
		nullable<SimpleTypes::CLongHexNumber<>>                 m_oCsb1;
		nullable<SimpleTypes::CLongHexNumber<>>                 m_oUsb0;
		nullable<SimpleTypes::CLongHexNumber<>>                 m_oUsb1;
		nullable<SimpleTypes::CLongHexNumber<>>                 m_oUsb2;
		nullable<SimpleTypes::CLongHexNumber<>>                 m_oUsb3;
	};

}

#endif /* OOX_FONT_INCLUDE_H_ */

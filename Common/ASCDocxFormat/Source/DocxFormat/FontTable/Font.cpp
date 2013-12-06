
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../FontTable.h"

namespace OOX
{
	FontTable::Font::Font()
	{
		m_name		=	std::wstring(_T("Arial"));
		m_usb0		=	std::wstring(_T("00000000"));
		m_usb1		=	std::wstring(_T("00000000"));
		m_usb2		=	std::wstring(_T("00000000"));	
		m_usb3		=	std::wstring(_T("00000000"));
		m_csb0		=	std::wstring(_T("00000000"));	
		m_csb1		=	std::wstring(_T("00000000"));
		m_family	=	std::wstring(L"");
		m_charset	=	std::wstring(L"");
		m_pitch		=	std::wstring(L"");
		m_panose1	=	std::wstring(L"");
	}

	FontTable::Font::~Font()
	{
	}

	FontTable::Font::Font(XmlUtils::CXmlNode& node)
	{
		fromXML(node);
	}

	const FontTable::Font& FontTable::Font::operator =(XmlUtils::CXmlNode& node)
	{
		fromXML(node);
		return *this;
	}

	void FontTable::Font::fromXML(XmlUtils::CXmlNode& oNode)
	{
		if ( _T("w:font") == oNode.GetName() )
		{
			m_name = std::wstring(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:name"))));

			XmlUtils::CXmlNode oChild;
			if ( oNode.GetNode( _T("w:panose1"), oChild ) )
				m_panose1 = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			if ( oNode.GetNode( _T("w:charset"), oChild ) )
				m_charset = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));
			if ( oNode.GetNode( _T("w:family"), oChild ) )
				m_family = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));	
			if ( oNode.GetNode( _T("w:pitch"), oChild ) )
				m_pitch = std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:val"))));	
			if ( oNode.GetNode( _T("w:sig"), oChild ) )
			{
				m_usb0	= std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:usb0"))));		
				m_usb1	= std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:usb1"))));		
				m_usb2	= std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:usb2"))));		
				m_usb3	= std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:usb3"))));	
				m_csb0	= std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:csb0"))));	
				m_csb1	= std::wstring(static_cast<const wchar_t*>(oChild.GetAttributeBase( _T("w:csb1"))));	
			}
		}
	}
} // namespace OOX
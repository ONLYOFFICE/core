#pragma once
#ifndef OOX_THEME_THEME_INCLUDE_H_
#define OOX_THEME_THEME_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../WritingElement.h"

#include "../Drawing/DrawingStyles.h"
#include "../Drawing/DrawingShared.h"

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CTheme 20.1.6.9 (Part 1)
	//--------------------------------------------------------------------------------	
	class CTheme : public OOX::File
	{
	public:
		CTheme()
		{
			m_bWriteContent = true;
		}

		CTheme(const CPath& oPath)
		{
			m_bWriteContent = true;
			read( oPath );
		}

		virtual ~CTheme()
		{
		}

	public:
		virtual void read(const CPath& oFilePath)
		{
			m_oReadPath = oFilePath;
			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oFilePath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			CWCharWrapper sName = oReader.GetName();
			if ( _T("a:theme") == sName && !oReader.IsEmptyNode() )
			{
				ReadAttributes( oReader );

				int nThemeDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nThemeDepth ) )
				{
					sName = oReader.GetName();

					wchar_t wChar0 = sName[0];
					wchar_t wChar2 = sName[2];

					if ( 'a' == wChar0 )
					{
						switch ( wChar2 )
						{
						case 'c':
							if      ( _T("a:custClrLst")        == sName ) m_oCustClrLst        = oReader;
							break;
						case 'e':
							if      ( _T("a:extLst")            == sName ) m_oExtLst            = oReader;
							else if ( _T("a:extraClrSchemeLst") == sName ) m_oExtraClrSchemeLst = oReader;
							break;
						case 'o':
							if      ( _T("a:objectDefaults")    == sName ) m_oObjectDefaults    = oReader;
							break;
						case 't':
							if      ( _T("a:themeElements")     == sName ) m_oThemeElements     = oReader;
							break;
						}
					}
				}
			}
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			if(m_bWriteContent)
			{
				CString sXml;
				sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" name=\"") + m_sName + _T("\">");

				sXml += m_oThemeElements.toXML();

				if ( m_oObjectDefaults.IsInit() )
					sXml += m_oObjectDefaults->toXML();

				if ( m_oExtraClrSchemeLst.IsInit() )
					sXml += m_oExtraClrSchemeLst->toXML();

				if ( m_oCustClrLst.IsInit() )
					sXml += m_oCustClrLst->toXML();

				if ( m_oExtLst.IsInit() )
					sXml += m_oExtLst->toXML();

				sXml += _T("</a:theme>");

				CDirectory::SaveToFile( oFilePath.GetPath(), sXml );
			}
			oContent.Registration( type().OverrideType(), oDirectory, oFilePath.GetFilename() );
		}

	public:
		virtual const OOX::FileType type() const
		{
			return FileTypes::Theme;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

	public:

		const CString GetMajorFont() const
		{
			if ( m_oThemeElements.m_oFontScheme.m_oMajorFont.m_oLatin.m_oTypeFace.IsInit() )
				return m_oThemeElements.m_oFontScheme.m_oMajorFont.m_oLatin.m_oTypeFace->GetValue();
			else
				return _T("Times New Roman");
		}		
		const CString GetMinorFont() const
		{
			if ( m_oThemeElements.m_oFontScheme.m_oMinorFont.m_oLatin.m_oTypeFace.IsInit() )
				return m_oThemeElements.m_oFontScheme.m_oMinorFont.m_oLatin.m_oTypeFace->GetValue();
			else
				return _T("Times New Roman");
		}
		const CString GetMajorFontOrEmpty() const
		{
			if ( m_oThemeElements.m_oFontScheme.m_oMajorFont.m_oLatin.m_oTypeFace.IsInit() )
				return m_oThemeElements.m_oFontScheme.m_oMajorFont.m_oLatin.m_oTypeFace->GetValue();
			else
				return CString();
		}		
		const CString GetMinorFontOrEmpty() const
		{
			if ( m_oThemeElements.m_oFontScheme.m_oMinorFont.m_oLatin.m_oTypeFace.IsInit() )
				return m_oThemeElements.m_oFontScheme.m_oMinorFont.m_oLatin.m_oTypeFace->GetValue();
			else
				return CString();
		}
		void DoNotWriteContent(bool bDoNotWriteContent)
		{
			m_bWriteContent = !bDoNotWriteContent;
		}
	private:

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, _T("name"), m_sName )
			WritingElement_ReadAttributes_End( oReader )
		}

	public:
		CPath									m_oReadPath;
		// Attributes
		CString                                        m_sName;

		// Childs
		nullable<OOX::Drawing::CCustomColorList>        m_oCustClrLst;
		nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		nullable<OOX::Drawing::CColorSchemeList>       m_oExtraClrSchemeLst;
		nullable<OOX::Drawing::CObjectStyleDefaults>   m_oObjectDefaults;
		OOX::Drawing::CBaseStyles                      m_oThemeElements;

		bool m_bWriteContent;
	};
} // namespace OOX

#endif // OOX_THEME_THEME_INCLUDE_H_
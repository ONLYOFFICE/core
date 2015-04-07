#pragma once
#ifndef OOX_BIBLIOGRAPHY_INCLUDE_H_
#define OOX_BIBLIOGRAPHY_INCLUDE_H_

#include "../Base/Nullable.h"
#include "WritingElement.h"
#include "File.h"

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CBibliography 22.6.2.60 (Part 1)
	//--------------------------------------------------------------------------------	
	class CBibliography : public OOX::File
	{
	public:
		CBibliography()
		{

		}
		CBibliography(const CPath& oPath)
		{
			read( oPath );
		}
		virtual ~CBibliography()
		{

		}

	public:
		virtual void read(const CPath& oFilePath)
		{
			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oFilePath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			CWCharWrapper sName = oReader.GetName();
			if ( _T("b:Sources") == sName && !oReader.IsEmptyNode() )
			{
				ReadAttributes( oReader );
			}
		}
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			CString sXml;
			sXml = _T("<b:Sources");
			
			if ( m_sSelectedStyle.IsInit() )
			{
				sXml += _T(" SelectedStyle=\"");
				sXml += m_sSelectedStyle->GetString();
				sXml += _T("\"");
			}

			if ( m_sStyleName.IsInit() )
			{
				sXml += _T(" StyleName=\"");
				sXml += m_sStyleName->GetString();
				sXml += _T("\"");
			}

			if ( m_sURI.IsInit() )
			{
				sXml += _T(" URI=\"");
				sXml += m_sURI->GetString();
				sXml += _T("\"");
			}

			sXml += _T(" xmlns:b=\"http://schemas.openxmlformats.org/officeDocument/2006/bibliography\" xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/bibliography\">");


			sXml += _T("</a:Sources>");

			CDirectory::SaveToFile( oFilePath.GetPath(), sXml );
			oContent.Registration( type().OverrideType(), oDirectory, oFilePath );
		}


	public:

		virtual const OOX::FileType type() const
		{
			return FileTypes::Bibliography;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

	private:

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("SelectedStyle"), m_sSelectedStyle )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("StyleName"),     m_sStyleName )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("URI"),           m_sURI )
			WritingElement_ReadAttributes_End( oReader )
		}

	private:

		// Attributes
		nullable<CString> m_sSelectedStyle;
		nullable<CString> m_sStyleName;
		nullable<CString> m_sURI;

		// Childs

		// TO DO: Реализовать полное чтение библиографии
	};
} // namespace OOX

#endif // OOX_BIBLIOGRAPHY_INCLUDE_H_
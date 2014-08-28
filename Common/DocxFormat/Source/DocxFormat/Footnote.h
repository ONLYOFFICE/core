#pragma once
#ifndef OOX_FOOTNOTE_INCLUDE_H_
#define OOX_FOOTNOTE_INCLUDE_H_

#include "File.h"
#include "IFileContainer.h"
#include "FtnEdn.h"
#include "Logic/RunContent.h"

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CFootnotes 17.11.15
	//--------------------------------------------------------------------------------	
	class CFootnotes : public OOX::File, public OOX::IFileContainer
	{
	public:

		CFootnotes()
		{
		}
		CFootnotes(const CPath& oPath)
		{
			read( oPath );
		}
		virtual ~CFootnotes()
		{
			for (unsigned int nIndex = 0; nIndex < m_arrFootnote.size(); nIndex++ )
			{
				if ( m_arrFootnote[nIndex] )
					delete m_arrFootnote[nIndex];
			}

			m_arrFootnote.clear();
		}
	public:

		virtual void read(const CPath& oPath)
		{
			IFileContainer::Read( oPath );

			XmlUtils::CXmlNode oFootnotes;
			oFootnotes.FromXmlFile( oPath.GetPath(), true );

			if ( _T("w:footnotes") == oFootnotes.GetName() )
			{
				XmlUtils::CXmlNodes oFootnoteList;
				oFootnotes.GetNodes( _T("w:footnote"), oFootnoteList );

				for ( int nIndex = 0; nIndex < oFootnoteList.GetCount(); nIndex++ )
				{
					XmlUtils::CXmlNode oFootnoteNode;
					if ( oFootnoteList.GetAt( nIndex, oFootnoteNode ) )
					{
						CFtnEdn *pFootnote = new CFtnEdn( oFootnoteNode );
						if (pFootnote) m_arrFootnote.push_back( pFootnote );
					}
				}
			}
		}
		virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			CString sXml;
			sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 wp14\">");
			for (unsigned int nIndex = 0; nIndex < m_arrFootnote.size(); nIndex++ )
			{
				if ( m_arrFootnote[nIndex] )
					sXml += m_arrFootnote[nIndex]->toXML();
			}
			sXml += _T("</w:footnotes>");
			CDirectory::SaveToFile( oPath.GetPath(), sXml );

			oContent.Registration( type().OverrideType(), oDirectory, oPath );
			IFileContainer::Write( oPath, oDirectory, oContent );
		}

	public:
		virtual const OOX::FileType type() const
		{
			return FileTypes::FootNote;
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

		OOX::CFtnEdn* Find(const OOX::Logic::CFootnoteReference& oReference) const
		{
			if ( !oReference.m_oId.IsInit() )
				return NULL;

			for ( unsigned int nIndex = 0; nIndex < m_arrFootnote.size(); nIndex++ )
			{
				if ( m_arrFootnote[nIndex]->m_oId.IsInit() && ( m_arrFootnote[nIndex]->m_oId == oReference.m_oId ) )
					return m_arrFootnote[nIndex];
			}

			return NULL;
		}
		void       Add(OOX::CFtnEdn* pFootnote)
		{
			m_arrFootnote.push_back( pFootnote );
		}		
		const unsigned int  GetCount() const
		{
			return m_arrFootnote.size();
		}

	public:

		std::vector<OOX::CFtnEdn*> m_arrFootnote;
	};
} // namespace OOX

#endif // OOX_FOOTNOTE_INCLUDE_H_

// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Core.h"
#include "FileTypes.h"

namespace OOX
{
	Core::Core()
	{
	}

	Core::Core(const OOX::CPath& filename)
	{
		read(filename);
	}

	Core::~Core()
	{
	}

	void Core::read(const OOX::CPath& oPath)
	{
		XmlUtils::CXmlNode document;
		document.FromXmlFile( oPath.GetPath(), true );

		if ( _T("cp:coreProperties") == document.GetName() )
		{
			XmlUtils::CXmlNode item;

			if ( document.GetNode( _T("cp:category"), item ) )
				m_sCategory = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("cp:contentStatus"), item ) )
				m_sContentStatus = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("dcterms:created"), item ) )
				m_sCreated = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("dc:creator"), item ) )
				m_sCreator = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("dc:description"), item ) )
				m_sDescription = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("dc:identifier"), item ) )
				m_sIdentifier = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("cp:keywords"), item ) )
				m_sKeywords = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("dc:language"), item ) )
				m_sLanguage = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("cp:lastModifiedBy"), item ) )
				m_sLastModifiedBy = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("cp:lastPrinted"), item ) )
				m_sLastPrinted = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("dcterms:modified"), item ) )
				m_sModified = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("cp:revision"), item ) )
				m_sRevision = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("dc:subject"), item ) )
				m_sSubject = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("dc:title"), item ) )
				m_sTitle = std::wstring(static_cast<const wchar_t*>(item.GetText()));

			if ( document.GetNode( _T("cp:version"), item ) )
				m_sVersion = std::wstring(static_cast<const wchar_t*>(item.GetText()));
		}
	}

	void Core::write(const OOX::CPath& oPath, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType Core::type() const
	{
		return FileTypes::Core;
	}

	const OOX::CPath Core::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath Core::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX
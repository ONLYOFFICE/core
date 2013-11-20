#pragma once
#ifndef OOX_APP_INCLUDE_H_
#define OOX_APP_INCLUDE_H_

#include "File.h"
#include "property.h"
#include "nullable.h"


namespace OOX
{
	class App : public OOX::File
	{
	public:
		App();
		App(const boost::filesystem::wpath& filename);
		virtual ~App();

	public:
		virtual void read(const boost::filesystem::wpath& filename);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		nullable_property<int, setter::only_positive<int> > Pages;
		nullable_property<int, setter::only_positive<int> > Words;
		nullable_property<int, setter::only_positive<int> > Paragraphs;
		nullable_property<int, setter::only_positive<int> > Characters;
		nullable_property<int, setter::only_positive<int> > CharactersWithSpaces;

	private:
		nullable<std::string> m_template;
		nullable<int>		  m_totalTime;
		nullable<std::string> m_appllication;
		nullable<int>		  m_docSecurity;
		nullable<int>		  m_lines;
		nullable<int>		  m_paragraphs;
		nullable<bool>		  m_scaleCrop;
		nullable<std::string> m_company;
		nullable<bool>		  m_linksUpToDate;
		nullable<bool>		  m_shareDoc;
		nullable<bool>		  m_hyperLinksChanged;
		nullable<std::string> m_appVersion;
	};
} // namespace OOX

#endif // OOX_APP_INCLUDE_H_
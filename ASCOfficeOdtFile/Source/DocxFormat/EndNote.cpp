
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "EndNote.h"
#include "Log.h"
#include <algorithm>
#include <boost/bind.hpp>
#include "Exception/log_range_error.h"
#include "FileTypes.h"


namespace OOX
{

	EndNote::EndNote()
	{
	}


	EndNote::EndNote(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	EndNote::~EndNote()
	{
	}


	void EndNote::read(const boost::filesystem::wpath& filename)
	{
		IFileContainer::read(filename);

		const XML::XDocument document(filename);
		XML::Fill(Notes, document.Root, "endnote");
	}


	void EndNote::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.w + "endnotes",
			XML::Write(Notes)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		IFileContainer::write(filename, directory, content);
	}


	const FileType EndNote::type() const
	{
		return FileTypes::EndNote;
	}


	const boost::filesystem::wpath EndNote::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath EndNote::DefaultFileName() const
	{
		return type().DefaultFileName();
	}


	const EndNote::Note EndNote::find(const Logic::EndnoteReference& reference) const
	{
		std::vector<EndNote::Note>::const_iterator it = std::find_if(Notes->begin(), Notes->end(), 
																										boost::bind(&Note::Id, _1) == *reference.Id);
		if (it == Notes->end())
			throw log_range_error("endnote");
		return *it;
	}


	void EndNote::add(const EndNote::Note& endnote)
	{
		push_back(endnote);
	}


	void EndNote::push_back(const EndNote::Note& endnote)
	{
		Notes->push_back(endnote);
	}


	const size_t EndNote::size() const
	{
		return Notes->size();
	}

} // namespace OOX
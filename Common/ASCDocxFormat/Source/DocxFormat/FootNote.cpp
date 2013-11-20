
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FootNote.h"
#include "Log.h"
#include <algorithm>
#include <boost/bind.hpp>
#include "Exception/log_range_error.h"
#include "FileTypes.h"


namespace OOX
{

	FootNote::FootNote()
	{
	}


	FootNote::FootNote(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	FootNote::~FootNote()
	{
	}


	void FootNote::read(const boost::filesystem::wpath& filename)
	{
		IFileContainer::read(filename);

		const XML::XDocument document(filename);
		XML::Fill(Notes, document.Root, "footnote");	
	}


	void FootNote::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.w + "footnotes",
			XML::Write(Notes)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		IFileContainer::write(filename, directory, content);
	}


	const FileType FootNote::type() const
	{
		return FileTypes::FootNote;
	}


	const boost::filesystem::wpath FootNote::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath FootNote::DefaultFileName() const
	{
		return type().DefaultFileName();
	}


	const FootNote::Note FootNote::find(const Logic::FootnoteReference& reference) const
	{
		std::vector<FootNote::Note>::const_iterator it = std::find_if(Notes->begin(), Notes->end(), 
																								boost::bind(&Note::Id, _1) == *reference.Id);
		if (it == Notes->end())
			throw log_range_error("footnote");
		return *it;
	}


	void FootNote::add(const FootNote::Note& footnote)
	{
		push_back(footnote);
	}

		
	void FootNote::push_back(const FootNote::Note& footnote)
	{
		Notes->push_back(footnote);
	}


	const size_t FootNote::size() const
	{
		return Notes->size();
	}

} // namespace OOX

// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FootNote.h"
#include "Log.h"
#include <algorithm>
#include "Exception/log_range_error.h"
#include "FileTypes.h"

namespace OOX
{
	FootNote::FootNote()
	{
	}

	FootNote::FootNote(const OOX::CPath& filename)
	{
		read(filename);
	}

	FootNote::~FootNote()
	{
	}

	void FootNote::read(const OOX::CPath& filename)
	{
		IFileContainer::read(filename);

		const XML::XDocument document(filename.GetPath());
		XML::Fill(Notes, document.Root, "footnote");	
	}

	void FootNote::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType FootNote::type() const
	{
		return FileTypes::FootNote;
	}

	const OOX::CPath FootNote::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const OOX::CPath FootNote::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	const FootNote::Note FootNote::find(const Logic::FootnoteReference& reference) const
	{
		const std::vector<FootNote::Note>& notes = Notes.get();
	
		for (std::vector<FootNote::Note>::const_iterator iter = notes.begin(); iter != notes.end(); ++iter)
		{
			if (*(*iter).Id == (*reference.Id))
			{
				return (*iter);
			}
		}
		
		throw log_range_error("footnote");
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
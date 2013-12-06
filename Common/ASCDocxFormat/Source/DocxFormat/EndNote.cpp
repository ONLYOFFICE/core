
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "EndNote.h"
#include "Log.h"
#include <algorithm>
#include "Exception/log_range_error.h"
#include "FileTypes.h"

namespace OOX
{
	EndNote::EndNote()
	{
	}

	EndNote::EndNote(const OOX::CPath& filename)
	{
		read(filename);
	}

	EndNote::~EndNote()
	{
	}

	void EndNote::read(const OOX::CPath& filename)
	{
		IFileContainer::read(filename);

		const XML::XDocument document(filename.GetPath());
		XML::Fill(Notes, document.Root, "endnote");
	}

	void EndNote::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType EndNote::type() const
	{
		return FileTypes::EndNote;
	}


	const OOX::CPath EndNote::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath EndNote::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	const EndNote::Note EndNote::find(const Logic::EndnoteReference& reference) const
	{
		const std::vector<EndNote::Note>& notes = Notes.get();
	
		for (std::vector<EndNote::Note>::const_iterator iter = notes.begin(); iter != notes.end(); ++iter)
		{
			if (*(*iter).Id == (*reference.Id))
			{
				return (*iter);
			}
		}
		
		throw log_range_error("endnote");
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
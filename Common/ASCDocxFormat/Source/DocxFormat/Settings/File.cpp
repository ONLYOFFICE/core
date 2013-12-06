
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "File.h"
#include "./../FileTypes.h"

namespace OOX
{
	namespace Settings
	{
		File::File()
		{
		}

		File::File(const OOX::CPath& filename)
		{
			read(filename);
		}

		File::~File()
		{
		}

		void File::read(const OOX::CPath& filename)
		{
		}

		void File::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
		{
		}

		const FileType File::type() const
		{
			return FileTypes::Setting;
		}

		const OOX::CPath File::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}

		const OOX::CPath File::DefaultFileName() const
		{
			return type().DefaultFileName();
		}

	} // namespace Settings
} // namespace OOXFile
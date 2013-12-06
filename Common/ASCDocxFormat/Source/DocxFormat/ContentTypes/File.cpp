
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "File.h"
#include "./../FileTypes.h"

namespace OOX
{
	namespace ContentTypes
	{
		static const OOX::CPath s_filename = L"[Content_Types].xml";

		File::File()
		{
		}

		File::File(const OOX::CPath& path)
		{
			read(path);
		}

		File::~File()
		{
		}

		void File::read(const OOX::CPath& path)
		{
		}

		void File::write(const OOX::CPath& path) const
		{
		}

		const bool File::isValid() const
		{
			return true;
		}

		void File::registration(const std::string& type, const OOX::CPath& directory, const OOX::CPath& filename)
		{
		}

	} // namespace ContentTypes
} // namespace OOX
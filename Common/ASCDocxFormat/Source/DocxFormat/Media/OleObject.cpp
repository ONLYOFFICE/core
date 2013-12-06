
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "OleObject.h"
#include "..\FileTypes.h"

namespace OOX
{
	OleObject::OleObject()
	{
	}

	OleObject::OleObject(const OOX::CPath& filename)
	{
		read(filename);
	}

	OleObject::~OleObject()
	{
	}

	void OleObject::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType OleObject::type() const
	{
		return FileTypes::OleObject;
	}

	const OOX::CPath OleObject::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath OleObject::DefaultFileName() const
	{
		return m_filename;
	}

} // namespace OOX
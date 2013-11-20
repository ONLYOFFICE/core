
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "OleObject.h"
#include "..\FileTypes.h"
#include <boost/algorithm/string/replace.hpp>


namespace OOX
{

	OleObject::OleObject()
	{
	}


	OleObject::OleObject(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	OleObject::~OleObject()
	{
	}

	void OleObject::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		std::wstring newFilename = filename.filename();
		boost::filesystem::wpath newFilePath = filename.parent_path();
		boost::replace_all(newFilename, L" ", L"_");
		if (m_filename.extension() != L".bin")
			newFilename += L".bin" ;
		
		boost::filesystem::copy_file(m_filename, newFilePath/newFilename);
		content.Default->add(newFilePath/newFilename);
	}


	const FileType OleObject::type() const
	{
		return FileTypes::OleObject;
	}


	const boost::filesystem::wpath OleObject::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath OleObject::DefaultFileName() const
	{
		return m_filename.filename();
	}

} // namespace OOX
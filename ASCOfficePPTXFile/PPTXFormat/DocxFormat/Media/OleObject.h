#pragma once
#ifndef OOX_OLE_OBJECT_INCLUDE_H_
#define OOX_OLE_OBJECT_INCLUDE_H_

#include "Media.h"


namespace OOX
{
	class OleObject : public Media
	{
	public:
		OleObject()
		{
		}
		OleObject(const OOX::CPath& filename)
		{
			read(filename);
		}
		virtual ~OleObject()
		{
		}

	public:
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
		{
			//std::wstring newFilename = filename.filename();
			//boost::filesystem::wpath newFilePath = filename.parent_path();
			//boost::replace_all(newFilename, L" ", L"_");
			//if (m_filename.extension() != L".bin")
			//	newFilename += L".bin" ;
			//
			//boost::filesystem::copy_file(m_filename, newFilePath/newFilename);
			//content.Default->add(newFilePath/newFilename);
		}

	public:
		virtual const FileType type() const
		{
			return FileTypes::OleObject;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return m_filename.GetFilename();
		}
	};
} // namespace OOX

#endif // OOX_OLE_OBJECT_INCLUDE_H_
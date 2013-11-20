#pragma once
#ifndef OOX_OLE_OBJECT_INCLUDE_H_
#define OOX_OLE_OBJECT_INCLUDE_H_

#include "Media.h"


namespace OOX
{
	class OleObject : public Media
	{
	public:
		OleObject();
		OleObject(const boost::filesystem::wpath& filename);
		virtual ~OleObject();

	public:
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_OLE_OBJECT_INCLUDE_H_
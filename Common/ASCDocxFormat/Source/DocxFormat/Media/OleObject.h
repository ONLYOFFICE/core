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
		OleObject(const OOX::CPath& filename);
		virtual ~OleObject();

	public:
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_OLE_OBJECT_INCLUDE_H_
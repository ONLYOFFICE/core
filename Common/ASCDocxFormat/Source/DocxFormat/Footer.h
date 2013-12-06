#pragma once
#ifndef OOX_FOOTER_INCLUDE_H_
#define OOX_FOOTER_INCLUDE_H_

#include <vector>

#include "File.h"
#include "IFileContainer.h"
#include "Logic/TextItem.h"

namespace OOX
{
	class Footer : public OOX::File, public IFileContainer
	{
	public:
		Footer();
		Footer(const OOX::CPath& filename);
		virtual ~Footer();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

	public:

		std::vector<Logic::TextItem>	m_items;
	};
} // namespace OOX

#endif // OOX_FOOTER_INCLUDE_H_
#pragma once
#ifndef OOX_EXTERNAL_INCLUDE_H_
#define OOX_EXTERNAL_INCLUDE_H_

#include "..\File.h"

namespace OOX
{
	class External : public File
	{
	public:
		External();
		External(const OOX::CPath& uri);
		~External();

	public:
		virtual void read(const OOX::CPath& uri);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		const OOX::CPath Uri() const;

		inline std::wstring GetPath() {	return std::wstring(m_uri.GetPath()); }

	protected:
		OOX::CPath m_uri;
	};
} // namespace OOX

#endif // OOX_EXTERNAL_INCLUDE_H_
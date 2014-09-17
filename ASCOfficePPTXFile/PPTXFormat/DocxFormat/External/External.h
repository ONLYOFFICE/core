#pragma once
#ifndef OOX_EXTERNAL_INCLUDE_H_
#define OOX_EXTERNAL_INCLUDE_H_

#include "../File.h"
#include "../FileTypes.h"


namespace PPTX
{
	class External : public File
	{
	public:
		External()
		{
		}
		External(const OOX::CPath& uri)
		{
			read(uri);
		}
		~External()
		{
		}

	public:
		virtual void read(const OOX::CPath& uri)
		{
			m_uri = uri;
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
		{
		}

	public:
		AVSINLINE OOX::CPath Uri() const
		{
			return m_uri;
		}

	protected:
		OOX::CPath m_uri;
	};
} // namespace PPTX

#endif // OOX_EXTERNAL_INCLUDE_H_
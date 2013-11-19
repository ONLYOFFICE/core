#pragma once
#ifndef OOX_EXTERNAL_INCLUDE_H_
#define OOX_EXTERNAL_INCLUDE_H_

#include "../File.h"
#include "../FileTypes.h"


namespace OOX
{
	class External : public File
	{
	public:
		External()
		{
		}
		External(const CPath& uri)
		{
			read(uri);
		}
		~External()
		{
		}

	public:
		virtual void read(const CPath& uri)
		{
			m_uri = uri;
		}
		virtual void write(const CPath& filename, const CPath& directory, CContentTypes& content) const
		{
		}

	public:
		AVSINLINE CPath Uri() const
		{
			return m_uri;
		}

	protected:
		CPath m_uri;
	};
} // namespace OOX

#endif // OOX_EXTERNAL_INCLUDE_H_
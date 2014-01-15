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
		External(const boost::filesystem::wpath& uri);
		~External();

	public:
		virtual void read(const boost::filesystem::wpath& uri);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		const boost::filesystem::wpath Uri() const;

	protected:
		boost::filesystem::wpath m_uri;
	};
} // namespace OOX

#endif // OOX_EXTERNAL_INCLUDE_H_
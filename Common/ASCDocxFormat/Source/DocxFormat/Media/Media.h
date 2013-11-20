#pragma once
#ifndef OOX_MEDIA_INCLUDE_H_
#define OOX_MEDIA_INCLUDE_H_

#include "..\File.h"


namespace OOX
{
	class Media : public File
	{
	public:
		Media();
		Media(const boost::filesystem::wpath& filename);
		virtual ~Media();

	public:
		virtual void read(const boost::filesystem::wpath& filename);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		const boost::filesystem::wpath filename() const;
		void copy_to(const boost::filesystem::wpath& path) const;

	protected:
		boost::filesystem::wpath	m_filename;
	};
} // namespace OOX

#endif // OOX_MEDIA_INCLUDE_H_
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
		Media(const OOX::CPath& filename);
		virtual ~Media();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		const OOX::CPath filename() const;
		void copy_to(const OOX::CPath& path) const;

		inline std::wstring GetPath() const { return std::wstring(m_filename.GetPath()); }

	protected:
		OOX::CPath	m_filename;
	};

	class Image : public Media
	{
	public:
		Image();
		Image(const OOX::CPath& filename);
		virtual ~Image();

	public:
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};

	class Video : public Media
	{
	public:
		Video();
		Video(const OOX::CPath& filename);
		virtual ~Video();

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};
	class Audio : public Media
	{
	public:
		Audio();
		Audio(const OOX::CPath& filename);
		virtual ~Audio();

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;
	};

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

#endif // OOX_MEDIA_INCLUDE_H_
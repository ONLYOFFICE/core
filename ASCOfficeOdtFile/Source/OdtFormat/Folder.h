#pragma once
#ifndef ODT_FOLDER_INCLUDE_H_
#define ODT_FOLDER_INCLUDE_H_

#include <string>
#include "property.h"
#include "Manifest/File.h"
#include "Settings/File.h"
#include "Meta/File.h"
#include "MimeType/File.h"
#include "Styles/File.h"
#include "Content/File.h"
#include <boost_filesystem_version.h>


namespace Odt
{
	class Folder
	{
	public:
		Folder();

	public:
		void read(const boost::filesystem::wpath& path);
		void write(const boost::filesystem::wpath& path) const;
		const bool isValid(const boost::filesystem::wpath& path) const;

	public:
		const bool hasList() const;
		const bool hasNote() const;

	public:
		void extractPictures(const boost::filesystem::wpath& path) const;
		void extractPictures(const boost::filesystem::wpath& source, const boost::filesystem::wpath& path);

	public:
		const bool existNamedStyle(const std::string& name) const;
		void findNamedStyle(const std::string& name, Logic::NamedStyle& oStyle) const;
		const Logic::NamedStyle& findNamedStyle(const std::string& name) const;
		Logic::NamedStyle& findNamedStyle(const std::string& name);

	public:
		property<Manifest::File>							Manifest;
		property<Settings::File>							Settings;
		property<MimeType::File>							Mimetype;
		property<Meta::File>									Meta;
		property<Styles::File>								Styles;
		property<Content::File>								Content;

	private:
		template<typename T, class S, class G> void read(nullable_property<T, S, G>& file, const boost::filesystem::wpath& path);
		template<typename T, class S, class G> void read(property<T, S, G>& file, const boost::filesystem::wpath& path);

		template<typename T, class S, class G> void write(const nullable_property<T, S, G>& file, const boost::filesystem::wpath& path) const;
		template<typename T, class S, class G> void write(const property<T, S, G>& file, const boost::filesystem::wpath& path) const;

		template<typename T, class S, class G> const bool isValid(const nullable_property<T, S, G>& file, const boost::filesystem::wpath& path) const;
		template<typename T, class S, class G> const bool isValid(const property<T, S, G>& file, const boost::filesystem::wpath& path) const;
	};


	template<typename T, class S, class G>
	void Folder::read(nullable_property<T, S, G>& file, const boost::filesystem::wpath& path)
	{
		if (T::exist(path))
		{
			if (!file.is_init())
				file = T();
			file->read(path);
		}
	}


	template<typename T, class S, class G>
	void Folder::read(property<T, S, G>& file, const boost::filesystem::wpath& path)
	{
			file->read(path);
	}


	template<typename T, class S, class G>
	void Folder::write(const nullable_property<T, S, G>& file, const boost::filesystem::wpath& path) const
	{
		if (file.is_init())
			file->write(path);
	}


	template<typename T, class S, class G>
	void Folder::write(const property<T, S, G>& file, const boost::filesystem::wpath& path) const
	{
		file->write(path);
	}


	template<typename T, class S, class G>
	const bool Folder::isValid(const nullable_property<T, S, G>& file, const boost::filesystem::wpath& path) const
	{
		if (file.is_init())
			return file->isValid(path);
		return true;
	}


	template<typename T, class S, class G> 
	const bool Folder::isValid(const property<T, S, G>& file, const boost::filesystem::wpath& path) const
	{
		return file->isValid(path);
	}

} // namespace Odt

#endif // ODT_FOLDER_INCLUDE_H_
#pragma once
#ifndef ODP_FOLDER_INCLUDE_H_
#define ODP_FOLDER_INCLUDE_H_

#include <boost/utility.hpp>
#include <string>
#include "property.h"

#include "Manifest/File.h"
//#include "OdtFormat/Configurations2/File.h"
//#include "OdtFormat/Settings/File.h"
//#include "Meta/File.h"
#include "MimeType/File.h"
#include "Styles/File.h"
#include "Content/File.h"
//#include "OdtFormat/Thumbnails/File.h"
#include "OdtFormat/Logic/Properties.h"

#include <boost/filesystem.hpp>


namespace Odp
{
	class Folder : private boost::noncopyable
	{
	public:
		Folder();
	private:
		boost::filesystem::wpath m_path;
	public:
		const std::wstring GetPath()const;

	public:
		void read(const boost::filesystem::wpath& path);
		void write(const boost::filesystem::wpath& path) const;
		const bool isValid(const boost::filesystem::wpath& path) const;

		const Odp::Styles::MasterPage& GetMasterPage(const std::string& str)const;
		const Odp::Styles::PageLayout& GetPageLayout(const std::string& str)const;
		const Odt::Logic::ListStyle GetListStyle(const std::string& styleName, bool master)const;
		const Odt::Logic::Properties GetPropertiesFromStyle(const std::string& styleName, bool master)const;
		const Odt::Logic::Properties GetPropertiesFromListStyle(const std::string& styleName, const int level)const;
		const Odp::Styles::TableTemplate& GetTableTemplate(const std::string& name)const;

	public:
		property<Odp::Manifest::File>			Manifest;
//		property<Odt::Configurations2::File>	Configurations2;
		//property<Odt::Settings::File>			Settings;
		property<Odp::MimeType::File>			Mimetype;
//		property<Odp::Meta::File>				Meta;
		property<Odp::Styles::File>				Styles;
		property<Odp::Content::File>			Content;
//		property<Odt::Thumbnails::File>			Thumbnails;

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
} // namespace Odp
#endif // ODP_FOLDER_INCLUDE_H_
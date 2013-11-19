#include "Folder.h"
#include "OdtFormat/FormatError.h"
#include <boost/filesystem.hpp>
#include "Log.h"
#include <stdexcept>

namespace Odp
{
	Folder::Folder()
	{
	}

	const std::wstring Folder::GetPath()const
	{
		return m_path.string();
	}

	void Folder::read(const boost::filesystem::wpath& path)
	{
		m_path = path;
		//read(Configurations2, path);
		//read(Settings, path);
		//read(Meta, path);
		read(Mimetype, path);
		read(Styles, path);
		read(Content, path);
		read(Manifest, path);
		//read(Thumbnails, path);
	}

	void Folder::write(const boost::filesystem::wpath& path) const
	{
		if (!boost::filesystem::exists(path))
			boost::filesystem::create_directories(path);

		//write(Configurations2, path);
		write(Mimetype, path);
		//write(Meta, path);
		//write(Settings, path);
		write(Styles, path);
		write(Content, path);
		//write(Thumbnails, path);
		write(Manifest, path);
	}

	const bool Folder::isValid(const boost::filesystem::wpath& path) const
	{
		//TODO не всегда присутствуют внутренние папки в Configurations2, разобраться
		//if (!isValid(Configurations2, path))
		//	return false;
		//else 
		if (!isValid(Mimetype, path))
			return false;
		//else if (!isValid(Settings, path))
		//	return false;
		//else if (!isValid(Meta, path))
		//	return false;
		else if (!isValid(Styles, path))
			return false;
		else if (!isValid(Content, path))
			return false;

		//TODO не всегда присутствуют внутренние папки в Configurations2, разобраться
		//else if (!isValid(Manifest, path))
		//	return false;
		//TODO не всегда присутствует Thumbnails, разобраться
		//else if (!isValid(Thumbnails, path))
		//	return false;
		return true;
	}

	const Odp::Styles::MasterPage& Folder::GetMasterPage(const std::string& str)const
	{
		return Styles->MasterPages->find(str);
	}

	const Odp::Styles::PageLayout& Folder::GetPageLayout(const std::string& str)const
	{
		return Styles->PageLayouts->find(str);
	}

	const Odt::Logic::ListStyle Folder::GetListStyle(const std::string& styleName, bool master)const
	{
		Odt::Logic::ListStyle style;
		if(master)
		{
			if(Styles->ListStyles->exist(styleName))
				return Styles->ListStyles->find(styleName);
		}
		else
		{
			if(Content->ListStyles->exist(styleName))
				return Content->ListStyles->find(styleName);
		}
		return style;
	}

	const Odt::Logic::Properties Folder::GetPropertiesFromStyle(const std::string& styleName, bool master)const
	{
		Odt::Logic::Properties properties;
		std::string family = "";
		std::string parent = "";
		std::string tmpStyleName = styleName;
		bool exist;
		do
		{
			exist = false;
			if(!master)
			{
				exist = Content->NamedStyles->exist(tmpStyleName);
				if(exist)
				{
					family = Content->NamedStyles->find(tmpStyleName).Family.get();
					parent = Content->NamedStyles->find(tmpStyleName).ParentName.get_value_or("");
					properties = Odt::Logic::Properties::merge(Content->NamedStyles->find(tmpStyleName).Properties.get(), properties);
				}
			}
			else
			{
				exist = Styles->AutomaticStyles->exist(tmpStyleName);
				if(exist)
				{
					family = Styles->AutomaticStyles->find(tmpStyleName).Family.get();
					parent = Styles->AutomaticStyles->find(tmpStyleName).ParentName.get_value_or("");
					properties = Odt::Logic::Properties::merge(Styles->AutomaticStyles->find(tmpStyleName).Properties.get(), properties);
				}
			}
			if(exist)
				tmpStyleName = parent;
		}
		while((exist) && (parent != ""));

		do
		{
			if(Styles->Styles->exist(tmpStyleName))
			{
				family = Styles->Styles->find(tmpStyleName).Family.get();
				parent = Styles->Styles->find(tmpStyleName).ParentName.get_value_or("");
				properties = Odt::Logic::Properties::merge(Styles->Styles->find(tmpStyleName).Properties.get(), properties);
			}
			tmpStyleName = parent;
		}
		while(parent != "");

		if(Styles->DefaultStyles->exist(family))
			properties = Odt::Logic::Properties::merge(Styles->DefaultStyles->find(family).Properties.get(), properties);

		return properties;
	}

	const Odt::Logic::Properties Folder::GetPropertiesFromListStyle(const std::string& styleName, const int level)const
	{
		Odt::Logic::Properties properties;
		if(Content->ListStyles->exist(styleName))
			properties = Content->ListStyles->find(styleName).listLevelStyleProperties(level);//Items->at(level).getProperties();
		else if(Styles->Styles->exist(styleName))
			properties = Styles->ListStyles->find(styleName).listLevelStyleProperties(level);//Items->at(level).getProperties();
		return properties;
	}

	const Odp::Styles::TableTemplate& Folder::GetTableTemplate(const std::string& name)const
	{
		return Styles->TableTemplates->find(name);
	}
} // namespace Odp
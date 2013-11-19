
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Folder.h"
#include "FormatError.h"
#include <boost_filesystem_version.h>
#include "Log.h"
#include <stdexcept>


namespace Odt
{

	Folder::Folder()
	{
	}


	void Folder::read(const boost::filesystem::wpath& path)
	{
		read(Manifest, path);
		if(Manifest->isFolderCrypted())
			return;
		read(Settings, path);
		read(Meta, path);
		read(Mimetype, path);
		read(Styles, path);
		read(Content, path);
	}


	void Folder::write(const boost::filesystem::wpath& path) const
	{
		if (!boost::filesystem::exists(path))
			boost::filesystem::create_directories(path);

		write(Mimetype, path);
		write(Meta, path);
		write(Settings, path);
		write(Styles, path);
		write(Content, path);
		write(Manifest, path);		
	}


	const bool Folder::isValid(const boost::filesystem::wpath& path) const
	{
		if (!isValid(Manifest, path))
			return false;
		if (!isValid(Mimetype, path))
			return false;
		else if (!isValid(Settings, path))
			return false;
		else if (!isValid(Meta, path))
			return false;
		else if (!isValid(Styles, path))
			return false;
		else if (!isValid(Content, path))
			return false;
		return true;
	}


	const bool Folder::hasList() const
	{
		return Content->hasList();
	}


	const bool Folder::hasNote() const
	{
		return Content->hasNote();
	}


	void Folder::extractPictures(const boost::filesystem::wpath& path) const
	{
		Manifest->extractPictures(path);
	}


	void Folder::extractPictures(const boost::filesystem::wpath& source, const boost::filesystem::wpath& path)
	{
		Manifest->read(source);
		Manifest->extractPictures(path);
	}


	const bool Folder::existNamedStyle(const std::string& name) const
	{
		return	Content->NamedStyles->exist(name) || 
						Styles->Styles->NamedStyles->exist(name) ||
						Styles->AutomaticStyles->NamedStyles->exist(name);
	}

	void Folder::findNamedStyle(const std::string& name, Logic::NamedStyle& oStyle) const
	{
		if (Content->NamedStyles->exist(name))
			oStyle = Content->NamedStyles->find(name);
		else if (Styles->Styles->NamedStyles->exist(name))
			oStyle = Styles->Styles->NamedStyles->find(name);
		else if (Styles->AutomaticStyles->NamedStyles->exist(name))
			oStyle = Styles->AutomaticStyles->NamedStyles->find(name);
		
		if (Content->NamedStyles->exist("Standard"))
			oStyle = Content->NamedStyles->find("Standard");
		else if (Styles->Styles->NamedStyles->exist("Standard"))
			oStyle = Styles->Styles->NamedStyles->find("Standard");
		else if (Styles->AutomaticStyles->NamedStyles->exist("Standard"))
			oStyle = Styles->AutomaticStyles->NamedStyles->find("Standard");
	}

	const Logic::NamedStyle& Folder::findNamedStyle(const std::string& name) const
	{
		if (Content->NamedStyles->exist(name))
			return Content->NamedStyles->find(name);
		else if (Styles->Styles->NamedStyles->exist(name))
			return Styles->Styles->NamedStyles->find(name);
		else if (Styles->AutomaticStyles->NamedStyles->exist(name))
			return Styles->AutomaticStyles->NamedStyles->find(name);
		
		if (Content->NamedStyles->exist("Standard"))
			return Content->NamedStyles->find("Standard");
		else if (Styles->Styles->NamedStyles->exist("Standard"))
			return Styles->Styles->NamedStyles->find("Standard");
		else if (Styles->AutomaticStyles->NamedStyles->exist("Standard"))
			return Styles->AutomaticStyles->NamedStyles->find("Standard");

		if (Content->NamedStyles->size() > 0)
			return *Content->NamedStyles->begin();
		else if (Styles->Styles->NamedStyles->size() > 0)
			return *Styles->Styles->NamedStyles->begin();
		else if (Styles->AutomaticStyles->NamedStyles->size() > 0)
			return *Styles->AutomaticStyles->NamedStyles->begin();


		throw std::range_error("Named style error");
	}

	
	Logic::NamedStyle& Folder::findNamedStyle(const std::string& name)
	{
		if (Content->NamedStyles->exist(name))
			return Content->NamedStyles->find(name);
		else if (Styles->Styles->NamedStyles->exist(name))
			return Styles->Styles->NamedStyles->find(name);
		else if (Styles->AutomaticStyles->NamedStyles->exist(name))
			return Styles->AutomaticStyles->NamedStyles->find(name);
		
		if (Content->NamedStyles->exist("Standard"))
			return Content->NamedStyles->find("Standard");
		else if (Styles->Styles->NamedStyles->exist("Standard"))
			return Styles->Styles->NamedStyles->find("Standard");
		else if (Styles->AutomaticStyles->NamedStyles->exist("Standard"))
			return Styles->AutomaticStyles->NamedStyles->find("Standard");

		if (Content->NamedStyles->size() > 0)
			return *Content->NamedStyles->begin();
		else if (Styles->Styles->NamedStyles->size() > 0)
			return *Styles->Styles->NamedStyles->begin();
		else if (Styles->AutomaticStyles->NamedStyles->size() > 0)
			return *Styles->AutomaticStyles->NamedStyles->begin();


		throw std::range_error("Named style error");
	}

} // namespace Odt
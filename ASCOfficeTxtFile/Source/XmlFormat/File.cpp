#include "File.h"
#include "Utility.h"


namespace XmlFormat
{

	File::File()
	{
	}


	void File::read(const boost::filesystem::wpath& filename, bool space)
	{
		if (filename.empty())
			return;
		Document.reset(new XML::XDocument(filename, space));
	}


	void File::write(const boost::filesystem::wpath& filename) const
	{
		Document->Save(filename);
	}


	const bool File::isValid(const boost::filesystem::wpath& filename) const
	{
		if (filename.empty())
			return true;
		return boost::filesystem::exists(filename);
	}

} // namespace XmlFormat
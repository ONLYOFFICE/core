#include "TableStyles.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX
{
	TableStyles::TableStyles()
	{
	}

	TableStyles::TableStyles(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	TableStyles::~TableStyles()
	{
	}

	void TableStyles::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		const XML::XDocument document(filename, true);

		def = document.Root.attribute("def").value();
		//TblStyles->clear();
		//XML::Fill(TblStyles, document.Root, "tblStyle");
		//for(std::list<Logic::TableStyle>::iterator i = TblStyles->begin(); i != TblStyles->end(); i++)
		//	i->SetParentFilePointer(this);
		Styles->clear();
		Logic::TableStyle Style;
		for(XML::element_iterator i = document.Root.Elements.begin(); i != document.Root.Elements.end(); i++)
		{
			Style = *i;
			Styles->insert(std::pair<std::string, Logic::TableStyle>(*Style.styleId, Style));
		}
		for(std::map<std::string, Logic::TableStyle>::iterator i = Styles->begin(); i != Styles->end(); i++)
			i->second.SetParentFilePointer(*this);
	}

	void TableStyles::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement element(ns.a + "tblStyleLst",
			XML::XAttribute("def", def)
			//XML::Write(TblStyles)
		);
		for(std::map<std::string, Logic::TableStyle>::const_iterator i = Styles->begin(); i != Styles->end(); i++)
			element.Add(XML::Write(i->second));

		element.Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
	}

	const OOX::FileType TableStyles::type() const
	{
		return OOX::FileTypes::TableStyles;
	}

	const boost::filesystem::wpath TableStyles::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath TableStyles::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	void TableStyles::SetTheme(const boost::shared_ptr<PPTX::Theme> theme)
	{
		m_Theme = theme;
		for(std::map<std::string, Logic::TableStyle>::iterator i = Styles->begin(); i != Styles->end(); i++)
			i->second.SetTheme(m_Theme);
	}

	DWORD TableStyles::GetRGBAFromMap(const std::string& str)const
	{
		if(m_Theme != 0)
			return m_Theme->GetRGBAFromMap(str);
		return 0;
	}

	DWORD TableStyles::GetARGBFromMap(const std::string& str)const
	{
		if(m_Theme != 0)
			return m_Theme->GetARGBFromMap(str);
		return 0;
	}

	DWORD TableStyles::GetBGRAFromMap(const std::string& str)const
	{
		if(m_Theme != 0)
			return m_Theme->GetBGRAFromMap(str);
		return 0;
	}

	DWORD TableStyles::GetABGRFromMap(const std::string& str)const
	{
		if(m_Theme != 0)
			return m_Theme->GetABGRFromMap(str);
		return 0;
	}

} // namespace PPTX
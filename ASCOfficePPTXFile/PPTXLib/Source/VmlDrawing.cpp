#include "VmlDrawing.h"
#include "DocxFormat/FileTypes.h"
#include "DocxFormat/RId.h"
#include "DocxFormat/Media/Image.h"

namespace PPTX
{
	VmlDrawing::VmlDrawing()
	{
	}

	VmlDrawing::VmlDrawing(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	VmlDrawing::~VmlDrawing()
	{
	}

	void VmlDrawing::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);
		XML::XDocument document(filename, true);
		m_strDocument = document.ToString();
	}

	void VmlDrawing::FillRIds()
	{
		const XML::XDocument document(m_strDocument, true);
		GetRIds(document.Root);
	}

	void VmlDrawing::GetRIds(const XML::XElement& element)
	{
		for(XML::element_iterator curIter = element.Elements.begin(); curIter != element.Elements.end(); curIter++)
		{
			XML::XElement curElement(*curIter);
			if(curElement.XName == "shape")
			{
				nullable_property<std::string> id;
				nullable_property<OOX::RId> rid;
				id = curElement.attribute("id").value();
				if(curElement.attribute("spid").exist())
					id = curElement.attribute("spid").value();
				if((curElement.element("imagedata").exist()) && (id.is_init()))
				{
					rid = curElement.element("imagedata").attribute("relid").value().ToString();
					if(rid.is_init())
					{
						boost::filesystem::wpath filename = image(rid.get()).filename();
						SpIds.insert(std::make_pair(id.get(), filename));
					}
				}
			}
			else
				GetRIds(curElement);
		}
	}

	void VmlDrawing::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XDocument document(m_strDocument, true);
		document.Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
		FileContainer::write(filename, directory, content);
	}

	const OOX::FileType VmlDrawing::type() const
	{
		return OOX::FileTypes::VmlDrawing;
	}

	const boost::filesystem::wpath VmlDrawing::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath VmlDrawing::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
} // namespace PPTX
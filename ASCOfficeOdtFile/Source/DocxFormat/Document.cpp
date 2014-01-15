
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Document.h"
#include "FileTypes.h"
#include "Media/Image.h"
#include "Logic/Paragraph.h"
#include "Encoding.h"
#include "External/HyperLink.h"


namespace OOX
{

	Document::Document()
	{
	}


	Document::Document(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	Document::~Document()
	{
	}


	void Document::read(const boost::filesystem::wpath& filename)
	{
		IFileContainer::read(filename);

		const XML::XDocument document(filename);
		const XML::XElement element = document.Root.element("body");
		SectorProperty = element.element("sectPr");
		Background	   = document.Root.element("background");

		std::map<int, int> ListsAndCounts;
		ListsAndCounts.clear();
		for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			const XML::XElement element(*i);
			if (element.XName == "p" || element.XName == "tbl" || element.XName == "sdt" || element.XName == "bookmarkStart" || element.XName == "bookmarkEnd")
			{
				Logic::TextItem text(*i);
				if((text.is<OOX::Logic::Paragraph>()) && (text.as<OOX::Logic::Paragraph>().isInList()))
				{
					int listnum = text.as<OOX::Logic::Paragraph>().GetListNum();
					if(ListsAndCounts.find(listnum) == ListsAndCounts.end())
					{
						ListsAndCounts.insert(std::make_pair(listnum, 1));
						text.as<OOX::Logic::Paragraph>().CountInList = 1;
					}
					else
					{
						ListsAndCounts[listnum]++;
						text.as<OOX::Logic::Paragraph>().CountInList = ListsAndCounts[listnum];
					}
				}
				else if(text.is<OOX::Logic::Paragraph>())
				{
					text.as<OOX::Logic::Paragraph>().CountInList = 0;
				}
				Items->push_back(text);
			}
		}
	}


	void Document::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.w + "document",
			XML::Write(Background) +
			XML::XElement(ns.w + "body",
					XML::Write(Items) +
					XML::Write(SectorProperty)
			)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		IFileContainer::write(filename, directory, content);
	}


	const FileType Document::type() const
	{
		return FileTypes::Document;
	}


	const boost::filesystem::wpath Document::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath Document::DefaultFileName() const
	{
		return type().DefaultFileName();
	}


	void Document::addImage(const boost::filesystem::wpath& imagePath, const long width, const long height)
	{
		Image* image = new Image(imagePath);
		const RId rId = add(boost::shared_ptr<OOX::File>(image));
		Logic::Paragraph paragraph(rId, imagePath, width, height);
		Items->push_back(paragraph);
	}

    void Document::addImage(const boost::filesystem::wpath& imagePath, const long xEmu, const std::string& hRelativeFrom, const long yEmu , const std::string& vRelativeFrom, const long widthEmu, const long heightEmu)
	{
		Image* image = new Image(imagePath);
		const RId rId = add(boost::shared_ptr<OOX::File>(image));
		Logic::Paragraph paragraph(rId, imagePath, xEmu, hRelativeFrom, yEmu, vRelativeFrom, widthEmu, heightEmu);
		Items->push_back(paragraph);
	}

	void Document::addImageInBegin(const boost::filesystem::wpath& imagePath, const long width, const long height)
	{
		Items->clear();
		Image* image = new Image(imagePath);
		const RId rId = add(boost::shared_ptr<OOX::File>(image));
		Logic::Paragraph paragraph(rId, imagePath, width, height);
		Items->push_back(paragraph);
	}


	void Document::addSpaceToLast(const int count)
	{
		if (!Items->empty() && Items->back().is<Logic::Paragraph>())
		{
			Items->back().as<Logic::Paragraph>().AddSpace(count);
		}
	}


	void Document::addPageBreak()
	{
		Logic::Paragraph paragraph;
		paragraph.AddBreak("page");
		Items->push_back(paragraph);
	}


	void Document::addText(const std::wstring& text)
	{
		Logic::Paragraph paragraph;
		paragraph.AddText(Encoding::unicode2utf8(text));
		Items->push_back(paragraph);
	}


	void Document::addText(const std::string& text)
	{
		Logic::Paragraph paragraph;
		paragraph.AddText(text);
		Items->push_back(paragraph);
	}


	void Document::addTextToLast(const std::wstring& text)
	{
		if (!Items->empty() && Items->back().is<Logic::Paragraph>())
		{
			Items->back().as<Logic::Paragraph>().AddText(Encoding::unicode2utf8(text));
		}
	}


	void Document::addTextToLast(const std::string& text)
	{
		if (!Items->empty() && Items->back().is<Logic::Paragraph>())
		{
			Items->back().as<Logic::Paragraph>().AddText(text);
		}
	}


	void Document::addHyperlink(const std::wstring& nameHref, const std::wstring& text)
	{
		Logic::Paragraph paragraph;
		boost::shared_ptr<OOX::File> hyperlink = boost::shared_ptr<OOX::File>(new OOX::HyperLink(nameHref));
		const OOX::RId rId = add(hyperlink);
		paragraph.AddHyperlink(rId, Encoding::unicode2utf8(text));
		Items->push_back(paragraph);		
	}


	void Document::addHyperlink(const std::string& nameHref, const std::string& text)
	{		
		Logic::Paragraph paragraph;
		boost::shared_ptr<OOX::File> hyperlink = boost::shared_ptr<OOX::File>(new OOX::HyperLink(Encoding::utf82unicode(nameHref)));
		const OOX::RId rId = add(hyperlink);
		paragraph.AddHyperlink(rId, text);
		Items->push_back(paragraph);
	}


	void Document::addHyperlinkToLast(const std::wstring& nameHref, const std::wstring& text)
	{
		if (!Items->empty() && Items->back().is<Logic::Paragraph>())
		{
			boost::shared_ptr<OOX::File> hyperlink = boost::shared_ptr<OOX::File>(new OOX::HyperLink(nameHref));
			const OOX::RId rId = add(hyperlink);
			Items->back().as<Logic::Paragraph>().AddHyperlink(rId, Encoding::unicode2utf8(text));
		}
	}


	void Document::addHyperlinkToLast(const std::string& nameHref, const std::string& text)
	{	
		if (!Items->empty() && Items->back().is<Logic::Paragraph>())
		{
			boost::shared_ptr<OOX::File> hyperlink = boost::shared_ptr<OOX::File>(new OOX::HyperLink(Encoding::utf82unicode(nameHref)));
			const OOX::RId rId = add(hyperlink);
			Items->back().as<Logic::Paragraph>().AddHyperlink(rId, text);
		}
	}


	void Document::Commit(const boost::filesystem::wpath& path)
	{
        std::string xmlString;

        if ( !boost::filesystem::exists(path) )
		{
			XML::Private::XDeclaration xDeclaration;
			Namespaces namespaces;

            xmlString = xDeclaration.ToString();

			xmlString += "<w:document ";

			xmlString += ( namespaces.w->ToString() + " " );
            xmlString += ( namespaces.wp->ToString() + " " );
			xmlString += ( namespaces.a->ToString() + " " );
            xmlString += ( namespaces.pic->ToString() + " " );
			xmlString += ( namespaces.r->ToString() + " " );
            xmlString += ( namespaces.v->ToString() + " " );
			xmlString += namespaces.w10->ToString();

			xmlString += ">";
			
			if ( Background.is_init() )
			{
			  xmlString += XML::Write(Background).ToString();
			}

			xmlString += "<w:body>";
		}

		xmlString += XML::Write(Items).ToString();

		std::ofstream file(path.string().c_str(), std::ios_base::app);
		
		if (!file.bad())
		{
			file <<xmlString;
			file.close();

			Items->clear();
		}
	}


	void Document::Finalize(const boost::filesystem::wpath& path, const boost::filesystem::wpath& directory, ContentTypes::File& content)
	{
		std::string xmlString;

        if ( !boost::filesystem::exists(path) )
		{
			XML::Private::XDeclaration xDeclaration;
			Namespaces namespaces;

            xmlString = xDeclaration.ToString();

			xmlString += "<w:document ";

			xmlString += ( namespaces.w->ToString() + " " );
            xmlString += ( namespaces.wp->ToString() + " " );
			xmlString += ( namespaces.a->ToString() + " " );
            xmlString += ( namespaces.pic->ToString() + " " );
			xmlString += ( namespaces.r->ToString() + " " );
			xmlString += ( namespaces.v->ToString() + " " );
			xmlString += namespaces.w10->ToString();

			xmlString += ">";

			if ( Background.is_init() )
			{
			  xmlString += XML::Write(Background).ToString();
			}

			xmlString += "<w:body>";
		}

		xmlString += XML::Write(Items).ToString();
		xmlString += XML::Write(SectorProperty).ToString();
		xmlString += ( "</w:body>" );
		xmlString += ( "</w:document>" );

		std::ofstream file(path.string().c_str(), std::ios_base::app);
		
		if (!file.bad())
		{
			file <<xmlString;
			file.close();

			Items->clear();
		}

		content.registration(type().OverrideType(), directory, path);
		IFileContainer::Finalize(path, directory, content);
	}

} // namespace OOX
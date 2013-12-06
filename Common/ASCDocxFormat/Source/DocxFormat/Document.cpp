
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Document.h"
#include "FileTypes.h"
#include "Media/Media.h"
#include "Logic/Paragraph.h"
#include "Encoding.h"
#include "External/HyperLink.h"

namespace OOX
{
	Document::Document()
	{
	}

	Document::Document(const OOX::CPath& filename)
	{
		read(filename);
	}

	Document::~Document()
	{
	}

	void Document::read(const OOX::CPath& filename)
	{
		IFileContainer::read(filename);

		const XML::XDocument document(filename.GetPath());
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

	void Document::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType Document::type() const
	{
		return FileTypes::Document;
	}

	const OOX::CPath Document::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath Document::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	void Document::addImage(const OOX::CPath& imagePath, const long width, const long height)
	{
		Image* image = new Image(imagePath);
		const RId rId = add(NSCommon::smart_ptr<OOX::File>(image));
		Logic::Paragraph paragraph(rId, imagePath, width, height);
		Items->push_back(paragraph);
	}

    void Document::addImage(const OOX::CPath& imagePath, const long xEmu, const std::string& hRelativeFrom, const long yEmu , const std::string& vRelativeFrom, const long widthEmu, const long heightEmu)
	{
		Image* image = new Image(imagePath);
		const RId rId = add(NSCommon::smart_ptr<OOX::File>(image));
		Logic::Paragraph paragraph(rId, imagePath, xEmu, hRelativeFrom, yEmu, vRelativeFrom, widthEmu, heightEmu);
		Items->push_back(paragraph);
	}

	void Document::addImageInBegin(const OOX::CPath& imagePath, const long width, const long height)
	{
		Items->clear();
		Image* image = new Image(imagePath);
		const RId rId = add(NSCommon::smart_ptr<OOX::File>(image));
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
		
		NSCommon::smart_ptr<OOX::File> hyperlink = NSCommon::smart_ptr<OOX::File>(new OOX::HyperLink(OOX::CPath(nameHref.c_str())));

		const OOX::RId rId = add(hyperlink);
		paragraph.AddHyperlink(rId, Encoding::unicode2utf8(text));
		Items->push_back(paragraph);		
	}

	void Document::addHyperlink(const std::string& nameHref, const std::string& text)
	{		
		Logic::Paragraph paragraph;
	
		NSCommon::smart_ptr<OOX::File> hyperlink = NSCommon::smart_ptr<OOX::File>(new OOX::HyperLink(OOX::CPath(Encoding::utf82unicode(nameHref).c_str())));

		const OOX::RId rId = add(hyperlink);
		paragraph.AddHyperlink(rId, text);
		Items->push_back(paragraph);
	}

	void Document::addHyperlinkToLast(const std::wstring& nameHref, const std::wstring& text)
	{
		if (!Items->empty() && Items->back().is<Logic::Paragraph>())
		{
			NSCommon::smart_ptr<OOX::File> hyperlink = NSCommon::smart_ptr<OOX::File>(new OOX::HyperLink(OOX::CPath(nameHref.c_str())));
			const OOX::RId rId = add(hyperlink);
			Items->back().as<Logic::Paragraph>().AddHyperlink(rId, Encoding::unicode2utf8(text));
		}
	}


	void Document::addHyperlinkToLast(const std::string& nameHref, const std::string& text)
	{	
		if (!Items->empty() && Items->back().is<Logic::Paragraph>())
		{
			NSCommon::smart_ptr<OOX::File> hyperlink = NSCommon::smart_ptr<OOX::File>(new OOX::HyperLink(OOX::CPath(Encoding::utf82unicode(nameHref).c_str())));
			const OOX::RId rId = add(hyperlink);
			Items->back().as<Logic::Paragraph>().AddHyperlink(rId, text);
		}
	}

	void Document::Commit(const OOX::CPath& path)
	{

	}

	void Document::Finalize(const OOX::CPath& path, const OOX::CPath& directory, ContentTypes::File& content)
	{

	}

} // namespace OOX
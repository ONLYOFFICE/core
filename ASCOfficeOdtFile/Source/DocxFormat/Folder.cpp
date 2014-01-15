
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Folder.h"
#include "Rels/File.h"
#include "Document.h"


namespace Docx
{

	Folder::Folder()
	{		
	}


	Folder::Folder(const boost::filesystem::wpath& path)
	{
		read(path);
	}


	void Folder::read(const boost::filesystem::wpath& path)
	{
		OOX::Rels::File rels(path / L"/");
		IFileContainer::read(rels, path);
	}


	void Folder::write(const boost::filesystem::wpath& path) const
	{
		boost::filesystem::create_directories(path);

		OOX::Rels::File rels;
		OOX::ContentTypes::File content;

		IFileContainer::write(rels, path, L"", content);

		rels.write(path / L"/");
		content.write(path);
	}


	void Folder::createFromTemplate(const boost::filesystem::wpath& path)
	{
		read(path);
	}


	void Folder::Commit(const boost::filesystem::wpath& path)
	{
	    boost::filesystem::create_directories(path);

		IFileContainer::Commit(path);
	}


	void Folder::Finalize(const boost::filesystem::wpath& path)
	{
		boost::filesystem::create_directories(path);

		OOX::Rels::File rels;
		OOX::ContentTypes::File content;

		IFileContainer::Finalize(rels, path, L"", content);

		rels.write(path / L"/");
		content.write(path);
	}


	const bool Folder::isValid(const boost::filesystem::wpath& path) const
	{
		return true;
	}


	void Folder::extractPictures(const boost::filesystem::wpath& path)
	{
		boost::filesystem::create_directories(path);
		OOX::IFileContainer::extractPictures(path);
	}


	void Folder::extractPictures(const boost::filesystem::wpath& source, const boost::filesystem::wpath& path)
	{
		read(source);
		extractPictures(path);
	}


	void Folder::addImage(const boost::filesystem::wpath& imagePath, const long width, const long height)
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addImage(imagePath, width, height);
		}
	}


	void Folder::addImageInBegin(const boost::filesystem::wpath& imagePath, const long width, const long height)
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addImageInBegin(imagePath, width, height);
		}
	}


	void Folder::addSpaceToLast(const int count)
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addSpaceToLast(count);
		}
	}


	void Folder::addPageBreak()
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addPageBreak();
		}
	}



	void Folder::addText(const std::wstring& text)
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addText(text);
		}
	}


	void Folder::addText(const std::string& text)
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addText(text);
		}
	}


	void Folder::addTextToLast(const std::wstring& text)
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addTextToLast(text);
		}
	}


	void Folder::addTextToLast(const std::string& text)
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addTextToLast(text);
		}
	}


	void Folder::addHyperlink(const std::wstring& nameHref, const std::wstring& text)
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addHyperlink(nameHref, text);
		}
	}


	void Folder::addHyperlink(const std::string& nameHref, const std::string& text)
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addHyperlink(nameHref, text);
		}
	}


	void Folder::addHyperlinkToLast(const std::wstring& nameHref, const std::wstring& text)
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addHyperlinkToLast(nameHref, text);
		}
	}


	void Folder::addHyperlinkToLast(const std::string& nameHref, const std::string& text)
	{
		if (exist<OOX::Document>())
		{
			OOX::Document& document = find<OOX::Document>();
			document.addHyperlinkToLast(nameHref, text);
		}
	}

} // namespace Docx
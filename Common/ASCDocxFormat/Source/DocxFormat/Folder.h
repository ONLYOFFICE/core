#pragma once
#ifndef DOCX_FOLDER_INCLUDE_H_
#define DOCX_FOLDER_INCLUDE_H_

#include "IFileContainer.h"
#include "RId.h"


namespace Docx
{
	class Folder : public OOX::IFileContainer
	{
	public:
		Folder();
		Folder(const boost::filesystem::wpath& path);

	public:
		void read(const boost::filesystem::wpath& path);
		void write(const boost::filesystem::wpath& path) const;
		void createFromTemplate(const boost::filesystem::wpath& path);

	public:
		void Commit(const boost::filesystem::wpath& path);
		void Finalize(const boost::filesystem::wpath& path);

	public:
		const bool isValid(const boost::filesystem::wpath& path) const;

	public:
		void extractPictures(const boost::filesystem::wpath& path);
		void extractPictures(const boost::filesystem::wpath& source, const boost::filesystem::wpath& path);

	public:
		void addImage(const boost::filesystem::wpath& imagePath, const long width, const long height);
		void addImageInBegin(const boost::filesystem::wpath& imagePath, const long width, const long height);
		void addSpaceToLast(const int count);
		void addPageBreak();
		void addText(const std::wstring& text);
		void addText(const std::string& text);
		void addTextToLast(const std::wstring& text);
		void addTextToLast(const std::string& text);
		void addHyperlink(const std::wstring& nameHref, const std::wstring& text);
		void addHyperlink(const std::string& nameHref, const std::string& text);
		void addHyperlinkToLast(const std::wstring& nameHref, const std::wstring& text);
		void addHyperlinkToLast(const std::string& nameHref, const std::string& text);
	};
} // namespace Docx

#endif //Docx_FOLDER_INCLUDE_H_
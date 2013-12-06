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
		Folder(const OOX::CPath& path);

	public:
		void read(const OOX::CPath& path);
		void write(const OOX::CPath& path) const;
		void createFromTemplate(const OOX::CPath& path);

	public:
		void Commit(const OOX::CPath& path);
		void Finalize(const OOX::CPath& path);

	public:
		const bool isValid(const OOX::CPath& path) const;

	public:
		void extractPictures(const OOX::CPath& path);
		void extractPictures(const OOX::CPath& source, const OOX::CPath& path);

	public:
		void addImage(const OOX::CPath& imagePath, const long width, const long height);
		void addImageInBegin(const OOX::CPath& imagePath, const long width, const long height);
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
#pragma once
#ifndef OOX_DOCUMENT_FILE_INCLUDE_H_
#define OOX_DOCUMENT_FILE_INCLUDE_H_

#include "File.h"
#include "IFileContainer.h"
#include "property.h"
#include "Logic/TextItem.h"
#include "Logic/SectorProperty.h"
#include "Logic/Background.h"
#include "IFileBuilder.h"

namespace OOX
{
	class Document : public OOX::File, public IFileContainer, public IFileBuilder
	{
	public:
		Document();
		Document(const OOX::CPath& filename);
		virtual ~Document();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

	public:
		virtual void Commit(const OOX::CPath& path);
		virtual void Finalize(const OOX::CPath& path, const OOX::CPath& directory, ContentTypes::File& content);

	public:
		void addImage(const OOX::CPath& imagePath, const long width, const long height);
        void addImage(const OOX::CPath& imagePath, const long xEmu, const std::string& hRelativeFrom, const long yEmu , const std::string& vRelativeFrom, const long widthEmu, const long heightEmu);
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

	public:
		property<Logic::SectorProperty>				SectorProperty;
		property<std::vector<Logic::TextItem> >		Items;
		nullable_property<Logic::Background>		Background;
	};
} // namespace OOX

#endif // OOX_DOCUMENT_FILE_INCLUDE_H_
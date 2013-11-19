#pragma once
#ifndef PPTX_APP_FILE_INCLUDE_H_
#define PPTX_APP_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "Logic/PartTitle.h"
#include "Logic/HeadingVariant.h"
#include "property.h"
#include "nullable.h"
#include "XML.h"
#include <list>

namespace PPTX
{
	class App : public WrapperFile
	{
	public:
		App();
		App(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~App();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		nullable_property<std::string> Template;							// (Name of Document Template)
		nullable_property<int, setter::only_positive<int> > TotalTime;		// (Total Edit Time Metadata Element)
		nullable_property<int, setter::only_positive<int> > Words;			// (Word Count)
		nullable_property<std::string> Application;							// (Application Name)
		nullable_property<std::string> PresentationFormat;					// (Intended Format of Presentation)
		nullable_property<int, setter::only_positive <int> > Paragraphs;	// (Total Number of Paragraphs)
		nullable_property<int, setter::only_positive <int> > Slides;		// (Slides Metadata Element)
		nullable_property<int, setter::only_positive <int> > Notes;			// (Number of Slides Containing Notes)
		nullable_property<int, setter::only_positive<int> > HiddenSlides;	// (Number of Hidden Slides)
		nullable_property<int, setter::only_positive<int> > MMClips;		// (Total Number of Multimedia Clips)
		nullable_property<bool> ScaleCrop;									// (Thumbnail Display Mode)
		property<std::list<Logic::HeadingVariant> > HeadingPairs;			// (Heading Pairs)
		property<std::list<Logic::PartTitle> > TitlesOfParts;				// (Part Titles)
		nullable_property<std::string> Company;								// (Name of Company)
		nullable_property<bool> LinksUpToDate;								// (Links Up-to-Date)
		nullable_property<bool> SharedDoc;									// (Shared Document)
		nullable_property<bool> HyperlinksChanged;							// (Hyperlinks Changed)
		nullable_property<std::string> AppVersion;							// (Application Version)
//			nullable_property<int, setter::only_positive<int> > Characters;		// (Total Number of Characters)
//			nullable_property<int, setter::only_positive<int> > CharactersWithSpaces;	// (Number of Characters (With Spaces))
//DigSig (Digital Signature)
//			nullable_property<int, setter::only_positive<int> > DocSecurity;	// (Document Security)
//std::list<std::string> - ??? HLinks;									// (Hyperlink List)
//			nullable_property<std::string> HyperlinkBase;						// (Relative Hyperlink Base)
//			nullable_property<int, setter::only_positive<int> > Lines;			// (Number of Lines)
//			nullable_property<std::string> Manager;								// (Name of Manager)
//			nullable_property<int, setter::only_positive<int> > Pages;			// (Total Number of Pages)
	private:
		nullable_property<int, setter::only_positive<int> > m_VectorSize;
		nullable_property<int, setter::only_positive<int> > m_Headings;
	};
} // namespace PPTX

#endif // PPTX_APP_FILE_INCLUDE_H_
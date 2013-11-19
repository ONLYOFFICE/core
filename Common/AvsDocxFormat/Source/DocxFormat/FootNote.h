#pragma once
#ifndef OOX_FOOTNOTE_INCLUDE_H_
#define OOX_FOOTNOTE_INCLUDE_H_

#include "File.h"
#include "IFileContainer.h"
#include <vector>
#include "property.h"
#include "nullable_property.h"
#include "Logic/FootnoteReference.h"
#include "WritingElement.h"
#include "Logic/TextItem.h"
#include "Limit/SeparatorType.h"


namespace OOX
{
	class FootNote : public OOX::File, public IFileContainer
	{
	public:
		class Note : public WritingElement
		{
		public:
			Note();
			virtual ~Note();			
			explicit Note(const XML::XNode& node);
			const Note& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			void push_back(const OOX::Logic::Paragraph& paragraph);

		public:
			nullable_property<std::string, Limit::SeparatorType> Type;
			property<size_t>	Id;
			property<std::vector<Logic::TextItem> >	Items; 	
		};

	public:
		FootNote();
		FootNote(const boost::filesystem::wpath& filename);
		virtual ~FootNote();

	public:
		virtual void read(const boost::filesystem::wpath& filename);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		const FootNote::Note find(const Logic::FootnoteReference& reference) const;
		void add(const FootNote::Note& note);
		void push_back(const FootNote::Note& note);
		const size_t size() const;

	public:
		property<std::vector<Note> >  Notes;
	};
} // namespace OOX

#endif // OOX_FOOTNOTE_INCLUDE_H_
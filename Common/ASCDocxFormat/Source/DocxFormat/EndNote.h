#pragma once
#ifndef OOX_ENDNOTE_INCLUDE_H_
#define OOX_ENDNOTE_INCLUDE_H_

#include "File.h"
#include "IFileContainer.h"
#include <vector>
#include "property.h"
#include "nullable_property.h"
#include "Logic/EndnoteReference.h"
#include "WritingElement.h"
#include "Logic/TextItem.h"
#include "Limit/SeparatorType.h"


namespace OOX
{
	class EndNote : public OOX::File, public IFileContainer
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
		EndNote();
		EndNote(const OOX::CPath& filename);
		virtual ~EndNote();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

	public:
		const EndNote::Note find(const Logic::EndnoteReference& reference) const;
		void add(const EndNote::Note& endnote);
		void push_back(const EndNote::Note& endnote);
		const size_t size() const;

	public:
		property<std::vector<Note> >  Notes;
	};
} // namespace OOX

#endif // OOX_ENDNOTE_INCLUDE_H_
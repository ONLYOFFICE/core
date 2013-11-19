#pragma once
#ifndef ODT_CONTENT_NOTE_INCLUDE_H_
#define ODT_CONTENT_NOTE_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/NoteClass.h"
#include "NoteCitation.h"
#include <vector>
#include "Text.h"


namespace Odt
{
	namespace Content
	{
		class Note : public ParagraphItemBase
		{
		public:
			Note();
			virtual ~Note();
			explicit Note(const XML::XNode& node);
			const Note& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			property<std::string>					Id;
			property<std::string, Limit::NoteClass> Class;
			nullable_property<NoteCitation>			Citation;
			property<std::vector<Text> >			Texts;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_NOTE_INCLUDE_H_

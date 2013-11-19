#pragma once
#ifndef ODT_STYLES_NOTES_CONFIGURATION_INCLUDE_H_
#define ODT_STYLES_NOTES_CONFIGURATION_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/NoteClass.h"
#include "./../Logic/NumFormat.h"
#include "./../Limit/FootnotesPosition.h"
#include "./../Limit/StartNumberingAt.h"


namespace Odt
{
	namespace Styles
	{
		class NotesConfiguration : public WritingElement
		{
		public:
			NotesConfiguration();
			virtual ~NotesConfiguration();
			explicit NotesConfiguration(const XML::XNode& node);
			const NotesConfiguration& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<Logic::NumFormat>						NumFormat;
			property<std::string, Limit::NoteClass>			NoteClass;
			property<int>									StartValue;
			nullable_property<std::string>					CitationStyleName;
			nullable_property<std::string, Limit::FootnotesPosition>	FootnotesPosition;
			nullable_property<std::string, Limit::StartNumberingAt>		StartNumberingAt;					
			nullable_property<std::string>					CitationBodyStyleName;
			nullable_property<std::string>					MasterPageName;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_NOTES_CONFIGURATION_INCLUDE_H_
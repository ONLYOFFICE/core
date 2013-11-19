#pragma once
#ifndef ODT_STYLES_STYLES_INCLUDE_H_
#define ODT_STYLES_STYLES_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "OutlineStyle.h"
#include "NotesConfiguration.h"
#include "LinenumberingConfiguration.h"
#include "./../Logic/DefaultStyles.h"
#include "./../Logic/NamedStyles.h"
#include "./../Logic/ListStyles.h"
#include "property.h"


namespace Odt
{
	namespace Styles
	{
		class Styles : public WritingElement
		{
		public:
			Styles();
			virtual ~Styles();
			explicit Styles(const XML::XNode& node);
			const Styles& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const NotesConfiguration getNotesConfiguration(const std::string& noteClass) const;

		public:
			property<std::vector<NotesConfiguration> >	NotesConfigurations;
			property<Logic::DefaultStyles>				DefaultStyles;
			property<Logic::NamedStyles>				NamedStyles;
			property<OutlineStyle>						OutlineStyle;
			property<Logic::ListStyles>					ListStyles;
			property<LinenumberingConfiguration>		LinenumberingConfiguration;

		private:
//			OutlineStyle								m_outlineStyle;
//			LinenumberingConfiguration	m_linenumberingConfiguration;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_STYLES_INCLUDE_H_
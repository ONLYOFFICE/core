#pragma once
#ifndef ODT_STYLES_LINENUMBERING_CONFIGURATION_INCLUDE_H_
#define ODT_STYLES_LINENUMBERING_CONFIGURATION_INCLUDE_H_

#include "./../WritingElement.h"
#include "./../Logic/NumFormat.h"
#include <string>


namespace Odt
{
	namespace Styles
	{
		class LinenumberingConfiguration : public WritingElement
		{
		public:
			LinenumberingConfiguration();
			virtual ~LinenumberingConfiguration();
			explicit LinenumberingConfiguration(const XML::XNode& node);
			const LinenumberingConfiguration& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			bool							m_numberLines;
			std::string				m_offset;
			Logic::NumFormat	m_numFormat;
			std::string				m_numberPosition;
			int								m_increment;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_LINENUMBERING_CONFIGURATION_INCLUDE_H_
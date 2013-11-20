#pragma once
#ifndef OOX_SETTINGS_RULES_INCLUDE_H_
#define OOX_SETTINGS_RULES_INCLUDE_H_

#include "./../WritingElement.h"
#include "Rule.h"
#include <vector>


namespace OOX
{
	namespace Settings
	{
		class Rules : public WritingElement
		{
		public:
			Rules();
			virtual ~Rules();
			explicit Rules(const XML::XNode& node);
			const Rules& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::vector<Rule>	m_rules;
			std::string			m_ext;
		};	
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_RULES_INCLUDE_H_
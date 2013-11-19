#pragma once
#ifndef OOX_SETTINGS_RULE_INCLUDE_H_
#define OOX_SETTINGS_RULE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>


namespace OOX
{
	namespace Settings
	{
		class Rule : public WritingElement
		{
		public:
			Rule();
			virtual ~Rule();
			explicit Rule(const XML::XNode& node);
			const Rule& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string		m_id;
			std::string		m_type;
			std::string		m_idref;
		};	
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_RULE_INCLUDE_H_
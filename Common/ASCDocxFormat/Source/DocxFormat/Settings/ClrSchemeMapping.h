#pragma once
#ifndef OOX_SETTINGS_CLR_SCHEME_MAPPING_INCLUDE_H_
#define OOX_SETTINGS_CLR_SCHEME_MAPPING_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>


namespace OOX
{
	namespace Settings
	{
		class ClrSchemeMapping : public WritingElement
		{
		public:
			ClrSchemeMapping();
			virtual ~ClrSchemeMapping();
			explicit ClrSchemeMapping(const XML::XNode& node);
			const ClrSchemeMapping& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string	m_bg1;
			std::string m_t1;
			std::string m_bg2; 
			std::string	m_t2; 
			std::string m_accent1; 
			std::string m_accent2;
			std::string m_accent3;
			std::string m_accent4; 
			std::string m_accent5; 
			std::string m_accent6; 
			std::string m_hyperlink; 
			std::string m_followedHyperlink; 
		};
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_CLR_SCHEME_MAPPING_INCLUDE_H_
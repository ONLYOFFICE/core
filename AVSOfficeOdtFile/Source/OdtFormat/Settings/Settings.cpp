
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Settings.h"


namespace Odt
{
	namespace Settings
	{

		Settings::Settings()
		{
		}


		Settings::~Settings()
		{
		}


		Settings::Settings(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Settings& Settings::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Settings::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_configItemSets, element);
		}


		const XML::XNode Settings::toXML() const
		{
			return 
				XML::XElement(ns.office + "settings",
					XML::Write(m_configItemSets)
				);
		}

	} // namespace Settings
} // namespace Odt
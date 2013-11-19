#include "ClrMap.h"

namespace PPTX
{
	namespace Logic
	{

		ClrMap::ClrMap()
		{
		}


		ClrMap::~ClrMap()
		{
		}


		ClrMap::ClrMap(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ClrMap& ClrMap::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ClrMap::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			ColorMap->clear();
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("accent1", element.attribute("accent1").value().ToString()));
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("accent2", element.attribute("accent2").value().ToString()));
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("accent3", element.attribute("accent3").value().ToString()));
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("accent4", element.attribute("accent4").value().ToString()));
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("accent5", element.attribute("accent5").value().ToString()));
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("accent6", element.attribute("accent6").value().ToString()));
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("bg1", element.attribute("bg1").value().ToString()));
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("bg2", element.attribute("bg2").value().ToString()));
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("tx1", element.attribute("tx1").value().ToString()));
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("tx2", element.attribute("tx2").value().ToString()));
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("folHlink", element.attribute("folHlink").value().ToString()));
			ColorMap->insert(std::pair<std::string, property<std::string, Limit::ColorSchemeIndex> >("hlink", element.attribute("hlink").value().ToString()));

			//accent1 = element.attribute("accent1").value();
			//accent2 = element.attribute("accent2").value();
			//accent3 = element.attribute("accent3").value();
			//accent4 = element.attribute("accent4").value();
			//accent5 = element.attribute("accent5").value();
			//accent6 = element.attribute("accent6").value();
			//bg1 = element.attribute("bg1").value();
			//bg2 = element.attribute("bg2").value();
			//folHlink = element.attribute("folHlink").value();
			//hlink = element.attribute("hlink").value();
			//tx1 = element.attribute("tx1").value();
			//tx2 = element.attribute("tx2").value();
		}


		const XML::XNode ClrMap::toXML() const
		{
			return XML::XElement(m_namespace + m_name.get(),
				XML::XAttribute("bg1", ColorMap->find("bg1")->second) +
				XML::XAttribute("tx1", ColorMap->find("tx1")->second) +
				XML::XAttribute("bg2", ColorMap->find("bg2")->second) +
				XML::XAttribute("tx2", ColorMap->find("tx2")->second) +
				XML::XAttribute("accent1", ColorMap->find("accent1")->second) +
				XML::XAttribute("accent2", ColorMap->find("accent2")->second) +
				XML::XAttribute("accent3", ColorMap->find("accent3")->second) +
				XML::XAttribute("accent4", ColorMap->find("accent4")->second) +
				XML::XAttribute("accent5", ColorMap->find("accent5")->second) +
				XML::XAttribute("accent6", ColorMap->find("accent6")->second) +
				XML::XAttribute("hlink", ColorMap->find("hlink")->second) +
				XML::XAttribute("folHlink", ColorMap->find("folHlink")->second)
			);
		}

		std::string ClrMap::GetColorSchemeIndex(const std::string& str)const
		{
			std::map<std::string, property<std::string, Limit::ColorSchemeIndex> >::const_iterator iter = ColorMap->find(str);
			if(iter != ColorMap->end())
				return iter->second.get();
			return str;
		}
	} // namespace Logic
} // namespace PPTX
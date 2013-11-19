#include "ClrScheme.h"

namespace PPTX
{
	namespace nsTheme
	{
		ClrScheme::ClrScheme()
		{
		}

		ClrScheme::~ClrScheme()
		{
		}

		ClrScheme::ClrScheme(const XML::XNode& node)
		{
			fromXML(node);
		}

		const ClrScheme& ClrScheme::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void ClrScheme::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			name = element.attribute("name").value();
			Logic::UniColor lColor;
			Scheme->clear();
			lColor.GetColorFrom(element.element("dk1"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("dk1", lColor));
			lColor.GetColorFrom(element.element("lt1"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("lt1", lColor));
			lColor.GetColorFrom(element.element("dk2"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("dk2", lColor));
			lColor.GetColorFrom(element.element("lt2"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("lt2", lColor));
			lColor.GetColorFrom(element.element("accent1"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("accent1", lColor));
			lColor.GetColorFrom(element.element("accent2"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("accent2", lColor));
			lColor.GetColorFrom(element.element("accent3"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("accent3", lColor));
			lColor.GetColorFrom(element.element("accent4"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("accent4", lColor));
			lColor.GetColorFrom(element.element("accent5"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("accent5", lColor));
			lColor.GetColorFrom(element.element("accent6"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("accent6", lColor));
			lColor.GetColorFrom(element.element("hlink"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("hlink", lColor));
			lColor.GetColorFrom(element.element("folHlink"));
			Scheme->insert(std::pair<std::string, Logic::UniColor>("folHlink", lColor));

			FillParentPointersForChilds();

			//dk1.GetColorFrom(element.element("dk1"));
			//lt1.GetColorFrom(element.element("lt1"));
			//dk2.GetColorFrom(element.element("dk2"));
			//lt2.GetColorFrom(element.element("lt2"));
			//accent1.GetColorFrom(element.element("accent1"));
			//accent2.GetColorFrom(element.element("accent2"));
			//accent3.GetColorFrom(element.element("accent3"));
			//accent4.GetColorFrom(element.element("accent4"));
			//accent5.GetColorFrom(element.element("accent5"));
			//accent6.GetColorFrom(element.element("accent6"));
			//hlink.GetColorFrom(element.element("hlink"));
			//folHlink.GetColorFrom(element.element("folHlink"));
		}

		const XML::XNode ClrScheme::toXML() const
		{
			return XML::XElement(ns.a + "clrScheme",
					XML::XAttribute("name", name) +
					XML::XElement(ns.a + "dk1", XML::Write(Scheme->find("dk1")->second)) +
					XML::XElement(ns.a + "lt1", XML::Write(Scheme->find("lt1")->second)) +
					XML::XElement(ns.a + "dk2", XML::Write(Scheme->find("dk2")->second)) +
					XML::XElement(ns.a + "lt2", XML::Write(Scheme->find("lt2")->second)) +
					XML::XElement(ns.a + "accent1", XML::Write(Scheme->find("accent1")->second)) +
					XML::XElement(ns.a + "accent2", XML::Write(Scheme->find("accent2")->second)) +
					XML::XElement(ns.a + "accent3", XML::Write(Scheme->find("accent3")->second)) +
					XML::XElement(ns.a + "accent4", XML::Write(Scheme->find("accent4")->second)) +
					XML::XElement(ns.a + "accent5", XML::Write(Scheme->find("accent5")->second)) +
					XML::XElement(ns.a + "accent6", XML::Write(Scheme->find("accent6")->second)) +
					XML::XElement(ns.a + "hlink", XML::Write(Scheme->find("hlink")->second)) +
					XML::XElement(ns.a + "folHlink", XML::Write(Scheme->find("folHlink")->second))
				);
		}

		DWORD ClrScheme::GetRGBAFromScheme(const std::string& str)const
		{
			return Scheme->find(str)->second.GetRGBA();
		}

		DWORD ClrScheme::GetARGBFromScheme(const std::string& str)const
		{
			return Scheme->find(str)->second.GetARGB();
		}

		DWORD ClrScheme::GetBGRAFromScheme(const std::string& str)const
		{
			return Scheme->find(str)->second.GetBGRA();
		}
		
		DWORD ClrScheme::GetABGRFromScheme(const std::string& str)const
		{
			return Scheme->find(str)->second.GetABGR();
		}

		void ClrScheme::FillParentPointersForChilds()
		{
			for(std::map<std::string, Logic::UniColor>::iterator i = Scheme->begin(); i != Scheme->end(); i++)
				i->second.SetParentPointer(*this);
		}
	} // namespace nsTheme
} // namespace PPTX
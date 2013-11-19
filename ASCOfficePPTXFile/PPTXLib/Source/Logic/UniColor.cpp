#include "UniColor.h"
#include "Colors/SrgbClr.h"
#include "Colors/PrstClr.h"
#include "Colors/SchemeClr.h"
#include "Colors/SysClr.h"

namespace PPTX
{
	namespace Logic
	{

		UniColor::UniColor()
		{
		}


		UniColor::~UniColor()
		{
			//Color.reset();
		}
	

		UniColor::UniColor(const XML::XNode& node)
		{
			fromXML(node);
		}


		const UniColor& UniColor::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void UniColor::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if (name == "srgbClr")
				Color.reset(new Logic::SrgbClr(node));
			else if (name == "prstClr")
				Color.reset(new Logic::PrstClr(node));
			else if (name == "schemeClr")
				Color.reset(new Logic::SchemeClr(node));
			else if (name == "sysClr")
				Color.reset(new Logic::SysClr(node));
			else Color.reset();
		}

		void UniColor::GetColorFrom(const XML::XElement& element){
			if(element.element("srgbClr").exist())
				Color.reset(new Logic::SrgbClr(element.element("srgbClr")));
			else if(element.element("prstClr").exist())
				Color.reset(new Logic::PrstClr(element.element("prstClr")));
			else if(element.element("schemeClr").exist())
				Color.reset(new Logic::SchemeClr(element.element("schemeClr")));
			else if(element.element("sysClr").exist())
				Color.reset(new Logic::SysClr(element.element("sysClr")));
			else Color.reset();
		}

		const XML::XNode UniColor::toXML() const
		{
			return XML::Write(Color);
		}

		DWORD UniColor::GetRGBA(DWORD RGBA)const
		{
			if(is_init())
				return Color->GetRGBA(RGBA);
			return 0; //return 0; - заменить на просмотр настроек по умолчанию
		}

		DWORD UniColor::GetARGB(DWORD ARGB)const
		{
			if(is_init())
				return Color->GetARGB(ARGB);
			return 0; //return 0; - заменить на просмотр настроек по умолчанию
		}

		DWORD UniColor::GetBGRA(DWORD BGRA)const
		{
			if(is_init())
				return Color->GetBGRA(BGRA);
			return 0; //return 0; - заменить на просмотр настроек по умолчанию
		}

		DWORD UniColor::GetABGR(DWORD ABGR)const
		{
			if(is_init())
				return Color->GetABGR(ABGR);
			return 0; //return 0; - заменить на просмотр настроек по умолчанию
		}

	} // namespace Logic
} // namespace PPTX
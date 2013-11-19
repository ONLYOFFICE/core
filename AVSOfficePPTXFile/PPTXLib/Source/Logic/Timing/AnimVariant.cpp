#include "AnimVariant.h"

namespace PPTX
{
	namespace Logic
	{

		AnimVariant::AnimVariant()
		{
		}

		AnimVariant::~AnimVariant()
		{
		}

		AnimVariant::AnimVariant(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AnimVariant& AnimVariant::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AnimVariant::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			name = element.XName->Name;
			if(element.element("boolVal").exist())
				boolVal = element.element("boolVal").attribute("val").value();
			else if(element.element("intVal").exist())
				intVal = element.element("intVal").attribute("val").value();
			else if(element.element("fltVal").exist())
				fltVal = element.element("fltVal").attribute("val").value();
			else if(element.element("clrVal").exist())
				clrVal.GetColorFrom(element.element("boolVal"));
			else if(element.element("strVal").exist())
				strVal = element.element("strVal").attribute("val").value();

			FillParentPointersForChilds();
		}

		const XML::XNode AnimVariant::toXML() const
		{
			return XML::XElement(ns.p + name.get(),
				XML::WriteIf(XML::XElement(ns.p + "strVal", XML::XAttribute("val", strVal)), strVal.is_init()) +
				XML::WriteIf(XML::XElement(ns.p + "boolVal", XML::XAttribute("val", boolVal)), boolVal.is_init()) +
				XML::WriteIf(XML::XElement(ns.p + "intVal", XML::XAttribute("val", intVal)), intVal.is_init()) +
				XML::WriteIf(XML::XElement(ns.p + "fltVal", XML::XAttribute("val", fltVal)), fltVal.is_init()) +
				XML::WriteIf(XML::XElement(ns.p + "clrVal", XML::Write(clrVal)), clrVal.is_init())
				);
		}

		void AnimVariant::FillParentPointersForChilds()
		{
			clrVal.SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX
#include "ClrMapOvr.h"


namespace PPTX
{
	namespace Logic
	{

		ClrMapOvr::ClrMapOvr()
		{
		}


		ClrMapOvr::~ClrMapOvr()
		{
		}


		ClrMapOvr::ClrMapOvr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ClrMapOvr& ClrMapOvr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ClrMapOvr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			overrideClrMapping = element.element("overrideClrMapping");
			FillParentPointersForChilds();
		}


		const XML::XNode ClrMapOvr::toXML() const
		{
			if(overrideClrMapping.is_init())
				return XML::XElement(ns.p + "clrMapOvr", XML::Write(overrideClrMapping));
			return XML::XElement(ns.p + "clrMapOvr", XML::XElement(ns.a + "masterClrMapping"));
		}

		void ClrMapOvr::FillParentPointersForChilds()
		{
			if(overrideClrMapping.is_init())
				overrideClrMapping->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
#include "LightRig.h"

namespace PPTX
{
	namespace Logic
	{

		LightRig::LightRig()
		{
		}


		LightRig::~LightRig()
		{
		}


		LightRig::LightRig(const XML::XNode& node)
		{
			fromXML(node);
		}


		const LightRig& LightRig::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void LightRig::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			dir = element.attribute("dir").value();
			rig = element.attribute("rig").value();

			rot = element.element("rot");
			FillParentPointersForChilds();
		}


		const XML::XNode LightRig::toXML() const
		{
			return XML::XElement(ns.a + "lightRig",
					XML::XAttribute("rig", rig) +
					XML::XAttribute("dir", dir) +
					XML::Write(rot)
				);
		}

		void LightRig::FillParentPointersForChilds()
		{
			if(rot.is_init())
				rot->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
#include "Cell3D.h"

namespace PPTX
{
	namespace Logic
	{

		Cell3D::Cell3D()
		{
		}


		Cell3D::~Cell3D()
		{
		}


		Cell3D::Cell3D(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Cell3D& Cell3D::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Cell3D::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			prstMaterial = element.attribute("prstMaterial").value();

			bevel = element.element("bevel");
			lightRig = element.element("lightRig");

			FillParentPointersForChilds();
		}


		const XML::XNode Cell3D::toXML() const
		{
			return XML::XElement(ns.a + "cell3D",
					XML::XAttribute("prstMaterial", prstMaterial) +
					XML::Write(bevel) +
					XML::Write(lightRig)
				);
		}

		void Cell3D::FillParentPointersForChilds()
		{
			if(bevel.is_init())
				bevel->SetParentPointer(*this);
			if(lightRig.is_init())
				lightRig->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
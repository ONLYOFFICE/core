#include "Sp3d.h"

namespace PPTX
{
	namespace Logic
	{

		Sp3d::Sp3d()
		{
		}


		Sp3d::~Sp3d()
		{
		}


		Sp3d::Sp3d(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Sp3d& Sp3d::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Sp3d::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			contourW = element.attribute("contourW").value();
			extrusionH = element.attribute("extrusionH").value();
			prstMaterial = element.attribute("prstMaterial").value();
			z = element.attribute("z").value();

			bevelT = element.element("bevelT");
			bevelB = element.element("bevelB");
			if(element.element("extrusionClr").exist())
				extrusionClr.GetColorFrom(element.element("extrusionClr"));
			if(element.element("contourClr").exist())
				contourClr.GetColorFrom(element.element("contourClr"));

			FillParentPointersForChilds();
		}


		const XML::XNode Sp3d::toXML() const
		{
			XML::XElement element(ns.a + "sp3d",
					XML::XAttribute("contourW", contourW) +
					XML::XAttribute("extrusionH", extrusionH) +
					XML::XAttribute("prstMaterial", prstMaterial) +
					XML::XAttribute("z", z) +
					XML::Write(bevelT) +
					XML::Write(bevelB)
				);
			if(extrusionClr.is_init())
				element.Add(XML::XElement(ns.a + "extrusionClr", XML::Write(extrusionClr)));
			if(contourClr.is_init())
				element.Add(XML::XElement(ns.a + "contourClr", XML::Write(contourClr)));

			return element;
		}

		void Sp3d::FillParentPointersForChilds()
		{
			if(bevelT.is_init())
				bevelT->SetParentPointer(*this);
			if(bevelB.is_init())
				bevelB->SetParentPointer(*this);
			extrusionClr.SetParentPointer(*this);
			contourClr.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
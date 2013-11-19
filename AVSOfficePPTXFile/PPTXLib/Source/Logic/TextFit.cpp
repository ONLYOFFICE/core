#include "TextFit.h"

namespace PPTX
{
	namespace Logic
	{

		TextFit::TextFit()
		{
			type = FitEmpty;
		}


		TextFit::~TextFit()
		{
		}


		TextFit::TextFit(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TextFit& TextFit::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TextFit::GetTextFitFrom(const XML::XElement& element)
		{
			type = FitEmpty;
			
			if(element.element("noAutofit").exist())
				type = FitNo;
			else if(element.element("spAutoFit").exist())
				type = FitSpAuto;
			else if(element.element("normAutofit").exist())
			{
				type = FitNormAuto;
				fontScale = element.element("normAutofit").attribute("fontScale").value();
				lnSpcReduction = element.element("normAutofit").attribute("lnSpcReduction").value();
			}
		}

		void TextFit::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			type = FitEmpty;
			
			if(element->XName == "noAutofit")
				type = FitNo;
			else if(element->XName == "spAutoFit")
				type = FitSpAuto;
			else if(element->XName == "normAutofit")
			{
				type = FitNormAuto;
				fontScale = element.attribute("fontScale").value();
				lnSpcReduction = element.attribute("lnSpcReduction").value();
			}
		}


		const XML::XNode TextFit::toXML() const
		{
			XML::XNode node;
			if(type == FitNo)
				return XML::XElement(ns.a + "noAutofit");
			else if(type == FitSpAuto)
				return XML::XElement(ns.a + "spAutoFit");
			else if(type == FitNormAuto)
				return XML::XElement(ns.a + "normAutofit",
						XML::XAttribute("fontScale", fontScale) +
						XML::XAttribute("lnSpcReduction", lnSpcReduction)
				);
			return node;
		}

		void TextFit::Merge(TextFit& fit)const
		{
			if(*type != FitEmpty)
			{
				if((*type == FitNo) || (*type == FitSpAuto))
				{
					fit.type = type;
					fit.fontScale.reset();
					fit.lnSpcReduction.reset();
				}
				else if(*type == FitNormAuto)
				{
					fit.type = type;
					if(fontScale.is_init())
						fit.fontScale = *fontScale;
					if(lnSpcReduction.is_init())
						fit.lnSpcReduction = *lnSpcReduction;
				}
			}
		}
	} // namespace Logic
} // namespace PPTX
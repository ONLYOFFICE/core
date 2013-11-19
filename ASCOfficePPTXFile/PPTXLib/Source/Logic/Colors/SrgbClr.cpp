#include "SrgbClr.h"
#include "Parse.h"

namespace PPTX
{
	namespace Logic
	{

		SrgbClr::SrgbClr()
		{
		}


		SrgbClr::~SrgbClr()
		{
		}
	

		SrgbClr::SrgbClr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SrgbClr& SrgbClr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SrgbClr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			std::string val = element.attribute("val").value().ToString();
			red		= HexString2Int(val.substr(0, 2));
			green	= HexString2Int(val.substr(2, 2));
			blue	= HexString2Int(val.substr(4, 2));

			Modifiers->clear();
			XML::Fill(Modifiers, element);
		}


		const XML::XNode SrgbClr::toXML() const
		{
			char val[10]="";
			sprintf_s(&val[0],10,"%.02X%.02X%.02X", red, green, blue);
			return XML::XElement(ns.a + "srgbClr",
					XML::XAttribute("val", std::string(val)) +
					XML::Write(Modifiers)
				);
		}

	} // namespace Logic
} // namespace PPTX
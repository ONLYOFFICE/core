#include "PrstGeom.h"

namespace PPTX
{
	namespace Logic
	{

		PrstGeom::PrstGeom()
		{
		}

		PrstGeom::~PrstGeom()
		{
		}

		PrstGeom::PrstGeom(const XML::XNode& node)
		{
			fromXML(node);
		}

		const PrstGeom& PrstGeom::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void PrstGeom::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			prst = element.attribute("prst").value();
			XML::Fill(avLst, element.element("avLst"), "gd");

			FillParentPointersForChilds();
		}

		const XML::XNode PrstGeom::toXML() const
		{
			return XML::XElement(ns.a + "prstGeom",
					XML::XAttribute("prst", prst) +
					XML::XElement(ns.a + "avLst", XML::Write(avLst))
				);
		}

		void PrstGeom::FillParentPointersForChilds()
		{
			for(std::list<Gd>::iterator i = avLst->begin(); i != avLst->end(); i++)
				i->SetParentPointer(*this);
		}

		std::string PrstGeom::GetODString()const
		{
			//std::string str = "";
			std::list<std::string> strList;
			strList.push_back("<avLst>");
			for(std::list<Gd>::const_iterator i = avLst->begin(); i != avLst->end(); i++)
			{
				/*str += */i->GetODString(strList);
			}
			/*str = "<avLst>" + str + */strList.push_back("</avLst>");

			std::string str = "";
			size_t length = 0;
			for(std::list<std::string>::const_iterator iter = strList.begin(); iter != strList.end(); iter++)
			{
				length += iter->length();
			}
			str.reserve(length);
			for(std::list<std::string>::const_iterator iter = strList.begin(); iter != strList.end(); iter++)
			{
				str += *iter;
			}

			return str;
		}
	} // namespace Logic
} // namespace PPTX
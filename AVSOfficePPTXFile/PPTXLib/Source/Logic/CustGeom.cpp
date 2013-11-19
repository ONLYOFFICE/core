#include "CustGeom.h"

namespace PPTX
{
	namespace Logic
	{
		CustGeom::CustGeom()
		{
		}

		CustGeom::~CustGeom()
		{
		}

		CustGeom::CustGeom(const XML::XNode& node)
		{
			fromXML(node);
		}

		const CustGeom& CustGeom::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void CustGeom::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			XML::Fill(avLst, element.element("avLst"), "gd");
			XML::Fill(gdLst, element.element("gdLst"), "gd");
			XML::Fill(ahLst, element.element("ahLst"), "ahPolar", "ahXY");
			XML::Fill(cxnLst, element.element("cxnLst"), "cxn");
			rect = element.element("rect");
			XML::Fill(pathLst, element.element("pathLst"), "path");

			FillParentPointersForChilds();
		}

		const XML::XNode CustGeom::toXML() const
		{
			return XML::XElement(ns.a + "custGeom",
					XML::XElement(ns.a + "avLst", XML::Write(avLst)) +
					XML::XElement(ns.a + "gdLst", XML::Write(gdLst)) +
					XML::XElement(ns.a + "ahLst", XML::Write(ahLst)) +
					XML::XElement(ns.a + "cxnLst", XML::Write(cxnLst)) +
					XML::Write(rect) +
					XML::XElement(ns.a + "pathLst", XML::Write(pathLst))
				);
		}

		void CustGeom::FillParentPointersForChilds()
		{
			for(std::list<Gd>::iterator i = avLst->begin(); i != avLst->end(); i++)
				i->SetParentPointer(*this);
			for(std::list<Gd>::iterator i = gdLst->begin(); i != gdLst->end(); i++)
				i->SetParentPointer(*this);
			for(std::list<AhBase>::iterator i = ahLst->begin(); i != ahLst->end(); i++)
				i->SetParentPointer(*this);
			for(std::list<Cxn>::iterator i = cxnLst->begin(); i != cxnLst->end(); i++)
				i->SetParentPointer(*this);
			if(rect.is_init())
				rect->SetParentPointer(*this);
			for(std::list<Path2D>::iterator i = pathLst->begin(); i != pathLst->end(); i++)
				i->SetParentPointer(*this);
		}

		std::string CustGeom::GetODString() const
		{
			std::list<std::string> strList;
			/*std::string str = */strList.push_back("<ooxml-shape>");

			/*str += */strList.push_back("<avLst>");
			for(std::list<Gd>::const_iterator i = avLst->begin(); i != avLst->end(); i++)
			{
				/*str += */i->GetODString(strList);
			}
			/*str += */strList.push_back("</avLst>");

			/*str += */strList.push_back("<gdLst>");
			for(std::list<Gd>::const_iterator i = gdLst->begin(); i != gdLst->end(); i++)
			{
				/*str += */i->GetODString(strList);
			}
			/*str += */strList.push_back("</gdLst>");

			/*str += */strList.push_back("<ahLst>");
			for(std::list<AhBase>::const_iterator i = ahLst->begin(); i != ahLst->end(); i++)
			{
				/*str += */i->GetODString(strList);
			}
			/*str += */strList.push_back("</ahLst>");

			/*str += */strList.push_back("<cxnLst>");
			for(std::list<Cxn>::const_iterator i = cxnLst->begin(); i != cxnLst->end(); i++)
			{
				/*str += */i->GetODString(strList);
			}
			/*str += */strList.push_back("</cxnLst>");

			if(rect.is_init())
				/*str += */rect->GetODString(strList);

			/*str += */strList.push_back("<pathLst>");
			for(std::list<Path2D>::const_iterator i = pathLst->begin(); i != pathLst->end(); i++)
			{
				/*str += */i->GetODString(strList);
			}
			/*str += */strList.push_back("</pathLst>");

			/*str += */strList.push_back("</ooxml-shape>");

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
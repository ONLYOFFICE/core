#include "Path2D.h"

namespace PPTX
{
	namespace Logic
	{

		Path2D::Path2D()
		{
		}


		Path2D::~Path2D()
		{
		}


		Path2D::Path2D(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Path2D& Path2D::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Path2D::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			extrusionOk = element.attribute("extrusionOk").value();
			fill = element.attribute("fill").value();
			h = element.attribute("h").value();
			stroke = element.attribute("stroke").value();
			w = element.attribute("w").value();

			Paths->clear();
			XML::Fill(Paths, element);
			
			FillParentPointersForChilds();
		}


		const XML::XNode Path2D::toXML() const
		{
			return XML::XElement(ns.a + "path",
					XML::XAttribute("w", w) +
					XML::XAttribute("h", h) +
					XML::XAttribute("fill", fill) +
					XML::XAttribute("stroke", stroke) +
					XML::XAttribute("extrusionOk", extrusionOk) +
					XML::Write(Paths)
				);
		}

		void Path2D::FillParentPointersForChilds()
		{
			for(std::list<UniPath2D>::iterator i = Paths->begin(); i != Paths->end(); i++)
				i->SetParentPointer(*this);
		}

		//std::string Path2D::GetODString()const
		void Path2D::GetODString(std::list<std::string>& strList)const
		{
			//std::string str = "";
			XML::XAttribute aW("w", w);
			XML::XAttribute aH("h", h);
			XML::XAttribute aFill("fill", fill);
			XML::XAttribute aStroke("stroke", stroke);
			XML::XAttribute aExtrusionOk("extrusionOk", extrusionOk);
			/*str = */strList.push_back("<path");//w=\"" + aW.value().ToString() + "\" h=\"" + aH.value().ToString() + "\" fill=\"" + aFill.value().ToString() + 
				//"\" stroke=\"" + aStroke.value().ToString() + "\" extrusionOk=\"" + aExtrusionOk.value().ToString() + "\">";
			if(w.is_init())
			{
				//str += " w=\"" + aW.value().ToString() + "\" ";
				//strList.push_back(" w=\"");
				strList.push_back(" ");
				aW->SaveToStringList(strList);
				//strList.push_back("\" ");
			}
			if(h.is_init())
			{
				//str += " h=\"" + aH.value().ToString() + "\" ";
				//strList.push_back(" h=\"");
				strList.push_back(" ");
				aH->SaveToStringList(strList);
				//strList.push_back("\" ");
			}
			if(fill.is_init())
			{
				//str += " fill=\"" + aFill.value().ToString() + "\" ";
				//strList.push_back(" fill=\"");
				strList.push_back(" ");
				aFill->SaveToStringList(strList);
				//strList.push_back("\" ");
			}
			if(stroke.is_init())
			{
				//str += " stroke=\"" + aStroke.value().ToString() + "\" ";
				//strList.push_back(" stroke=\"");
				strList.push_back(" ");
				aStroke->SaveToStringList(strList);
				//strList.push_back("\" ");
			}
			if(extrusionOk.is_init())
			{
				//str += " extrusionOk=\"" + aExtrusionOk.value().ToString() + "\" ";
				//strList.push_back(" extrusionOk=\"");
				strList.push_back(" ");
				aExtrusionOk->SaveToStringList(strList);
				//strList.push_back("\" ");
			}
			//str += ">";
			strList.push_back(">");

			for(std::list<UniPath2D>::const_iterator i = Paths->begin(); i != Paths->end(); i++)
			{
				/*str += */i->GetODString(strList);
			}
			/*str += */strList.push_back("</path>");
			//return str;
		}
	} // namespace Logic
} // namespace PPTX
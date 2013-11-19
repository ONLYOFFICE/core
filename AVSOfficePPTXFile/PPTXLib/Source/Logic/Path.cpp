#include "Path.h"

namespace PPTX
{
	namespace Logic
	{

		Path::Path()
		{
		}


		Path::~Path()
		{
		}


		Path::Path(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Path& Path::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Path::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			path = element.attribute("path").value();
			rect = element.element("fillToRect");
			
			FillParentPointersForChilds();
		}


		const XML::XNode Path::toXML() const
		{
			return XML::XElement(ns.a + "path",
					XML::XAttribute("path", path) +
					XML::Write(rect)
				);
		}

		void Path::FillParentPointersForChilds()
		{
			if(rect.is_init())
				rect->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
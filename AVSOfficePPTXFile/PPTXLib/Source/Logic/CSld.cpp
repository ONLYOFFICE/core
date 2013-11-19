#include "CSld.h"

namespace PPTX
{
	namespace Logic
	{

		CSld::CSld()
		{
		}


		CSld::~CSld()
		{
		}


		CSld::CSld(const XML::XNode& node)
		{
			fromXML(node);
		}


		const CSld& CSld::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void CSld::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			attrName = element.attribute("name").value();
			bg = element.element("bg");
			spTree = element.element("spTree");
			
			FillParentPointersForChilds();
		}


		const XML::XNode CSld::toXML() const
		{
			return XML::XElement(ns.p + "cSld",
					XML::XAttribute("name", attrName) +
					XML::Write(bg) +
					XML::Write(spTree)
				);
		}

		void CSld::FillParentPointersForChilds()
		{
			if(bg.is_init())
				bg->SetParentPointer(*this);
			spTree->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
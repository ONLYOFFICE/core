#include "Run.h"

namespace PPTX
{
	namespace Logic
	{

		Run::Run()
		{
		}

		Run::~Run()
		{
		}

		Run::Run(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Run& Run::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Run::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			rPr = element.element("rPr");
			text = element.element("t").text();//.ToString();

			FillParentPointersForChilds();
		}

		const XML::XNode Run::toXML() const
		{
			return XML::XElement(ns.a + "r",
					XML::Write(rPr) +
					XML::XElement(ns.a + "t", XML::XText(text))
				);
		}

		void Run::FillParentPointersForChilds()
		{
			if(rPr.is_init())
				rPr->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
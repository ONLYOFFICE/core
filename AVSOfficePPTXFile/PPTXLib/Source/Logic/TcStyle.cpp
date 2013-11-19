#include "TcStyle.h"
#include "StyleRef.h"
#include "FillStyle.h"

namespace PPTX
{
	namespace Logic
	{

		TcStyle::TcStyle()
		{
		}


		TcStyle::~TcStyle()
		{
		}
	

		TcStyle::TcStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TcStyle& TcStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TcStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			tcBdr = element.element("tcBdr");
			cell3D = element.element("cell3D");
			fill = element.element("fill");
			fillRef = element.element("fillRef");


			//if(element.element("fill").exist())
			//	Fill.reset(new Logic::FillStyle(element.element("fill")));
			//else if(element.element("fillRef").exist())
			//	Fill.reset(new Logic::StyleRef(element.element("fillRef")));
			//else Fill.reset();

			FillParentPointersForChilds();
		}


		const XML::XNode TcStyle::toXML() const
		{
			return XML::XElement(ns.a + "tcStyle",
					XML::Write(tcBdr) +
					//XML::Write(Fill) +
					XML::Write(fill) +
					XML::Write(fillRef) +
					XML::Write(cell3D)
				);
		}

		void TcStyle::FillParentPointersForChilds()
		{
			if(tcBdr.is_init())
				tcBdr->SetParentPointer(*this);
			if(fill.is_init())
				fill->SetParentPointer(*this);
			if(fillRef.is_init())
				fillRef->SetParentPointer(*this);
			if(cell3D.is_init())
				cell3D->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
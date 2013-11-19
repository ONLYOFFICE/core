#include "Duotone.h"

namespace PPTX
{
	namespace Logic
	{

		Duotone::Duotone()
		{
		}


		Duotone::~Duotone()
		{
		}


		Duotone::Duotone(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Duotone& Duotone::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Duotone::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Colors->clear();
			XML::Fill(Colors, element);
			FillParentPointersForChilds();
		}


		const XML::XNode Duotone::toXML() const
		{
			return XML::XElement(ns.a + "duotone",
					XML::Write(Colors->front()) +
					XML::Write(Colors->back())
				);
		}

		void Duotone::FillParentPointersForChilds()
		{
			for(std::list<UniColor>::iterator i = Colors->begin(); i != Colors->end(); i++)
				i->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX
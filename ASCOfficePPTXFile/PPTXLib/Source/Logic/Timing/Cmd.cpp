#include "Cmd.h"

namespace PPTX
{
	namespace Logic
	{

		Cmd::Cmd()
		{
		}

		Cmd::~Cmd()
		{
		}

		Cmd::Cmd(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Cmd& Cmd::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Cmd::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			type = element.attribute("type").value();
			cmd = element.attribute("cmd").value();

			cBhvr = element.element("cBhvr");

			FillParentPointersForChilds();
		}

		const XML::XNode Cmd::toXML() const
		{
			return XML::XElement(ns.p + "cmd",
				XML::XAttribute("type", type) +
				XML::XAttribute("cmd", cmd) +
				XML::Write(cBhvr)
				);
		}

		void Cmd::FillParentPointersForChilds()
		{
			cBhvr->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX
#include "Audio.h"

namespace PPTX
{
	namespace Logic
	{

		Audio::Audio()
		{
		}

		Audio::~Audio()
		{
		}

		Audio::Audio(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Audio& Audio::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Audio::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			isNarration = element.attribute("isNarration").value();
			cMediaNode = element.element("cMediaNode");

			FillParentPointersForChilds();
		}

		const XML::XNode Audio::toXML() const
		{
			return XML::XElement(ns.p + "audio",
				XML::XAttribute("isNarration", isNarration) +
				XML::Write(cMediaNode)
				);
		}

		void Audio::FillParentPointersForChilds()
		{
			cMediaNode->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX
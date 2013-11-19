#include "AudioCD.h"

namespace PPTX
{
	namespace Logic
	{
		AudioCD::AudioCD()
		{
		}

		AudioCD::~AudioCD()
		{
		}

		AudioCD::AudioCD(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AudioCD& AudioCD::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AudioCD::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			stTrack = element.element("st").attribute("track").value();
			stTime = element.element("st").attribute("time").value();
			endTrack = element.element("end").attribute("track").value();
			endTime = element.element("end").attribute("time").value();
		}

		const XML::XNode AudioCD::toXML() const
		{
			return XML::XElement(ns.a + "audioCd",
					XML::XElement(ns.a + "st", XML::XAttribute("track", stTrack) + XML::XAttribute("time", stTime)) +
					XML::XElement(ns.a + "end", XML::XAttribute("track", endTrack) + XML::XAttribute("time", endTime))
				);
		}
	} // namespace Logic
} // namespace PPTX
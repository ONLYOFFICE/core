#include "WavAudioFile.h"

namespace PPTX
{
	namespace Logic
	{

		WavAudioFile::WavAudioFile()
		{
		}

		WavAudioFile::~WavAudioFile()
		{
		}

		WavAudioFile::WavAudioFile(const XML::XNode& node)
		{
			fromXML(node);
		}

		const WavAudioFile& WavAudioFile::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void WavAudioFile::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name = element.XName.get().Name;

			embed = element.attribute("embed").value().ToString();
			name = element.attribute("name").value();
		}

		const XML::XNode WavAudioFile::toXML() const
		{
			return XML::XElement(ns.a + m_name.get(),
					XML::XAttribute(ns.r + "embed", embed) + 
					XML::XAttribute("name", name)
				);
		}
	} // namespace Logic
} // namespace PPTX
#include "MediaFile.h"

namespace PPTX
{
	namespace Logic
	{

		MediaFile::MediaFile()
		{
		}

		MediaFile::~MediaFile()
		{
		}

		MediaFile::MediaFile(const XML::XNode& node)
		{
			fromXML(node);
		}

		const MediaFile& MediaFile::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void MediaFile::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			name = element.XName->Name;
			link = element.attribute("link").value().ToString();
			contentType = element.attribute("contentType").value();
		}

		const XML::XNode MediaFile::toXML() const
		{
			return XML::XElement(ns.a + name.get(),
					XML::XAttribute(ns.r + "link", link) +
					XML::XAttribute("contentType", contentType)
			);
		}
	} // namespace Logic
} // namespace PPTX
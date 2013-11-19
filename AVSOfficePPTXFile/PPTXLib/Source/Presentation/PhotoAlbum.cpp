#include "PhotoAlbum.h"

namespace PPTX
{
	namespace nsPresentation
	{
		PhotoAlbum::PhotoAlbum()
		{
		}

		PhotoAlbum::~PhotoAlbum()
		{
		}

		PhotoAlbum::PhotoAlbum(const XML::XNode& node)
		{
			fromXML(node);
		}

		const PhotoAlbum& PhotoAlbum::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void PhotoAlbum::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			bw = element.attribute("bw").value();
			frame = element.attribute("frame").value();
			layout = element.attribute("layout").value();
			showCaptions = element.attribute("showCaptions").value();
		}

		const XML::XNode PhotoAlbum::toXML() const
		{
			return	XML::XElement(ns.p + "photoAlbum",
				XML::XAttribute("bw", bw) +
				XML::XAttribute("frame", frame) +
				XML::XAttribute("layout", layout) +
				XML::XAttribute("showCaptions", showCaptions)
			);
		}
	} // namespace nsPresentation
} // namespace PPTX
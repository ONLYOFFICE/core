
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TextFrameProperties.h"


namespace OOX
{
	namespace Logic
	{

		TextFrameProperties::TextFrameProperties()
		{
		}


		TextFrameProperties::~TextFrameProperties()
		{
		}
	

		TextFrameProperties::TextFrameProperties(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TextFrameProperties& TextFrameProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TextFrameProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			HAnchor			=	element.attribute("hAnchor").value();
			VAnchor	        =	element.attribute("vAnchor").value();
			Wrap			=	element.attribute("wrap").value();
			if (element.attribute("x").exist())
			{
				int x = element.attribute("x").value();
				X = UniversalUnit(x, UniversalUnit::Dx);
			}
			if (element.attribute("y").exist())
			{
				int y = element.attribute("y").value();
				Y = UniversalUnit(y, UniversalUnit::Dx);
			}
			if (element.attribute("h").exist())
			{
				int h = element.attribute("h").value();
				H = UniversalUnit(h, UniversalUnit::Dx);
			}
			if (element.attribute("w").exist())
			{
				int w = element.attribute("w").value();
				W = UniversalUnit(w, UniversalUnit::Dx);
			}
			if (element.attribute("hSpace").exist())
			{
				int hSpace = element.attribute("hSpace").value();
				HSpace = UniversalUnit(hSpace, UniversalUnit::Dx);
			}
			if (element.attribute("vSpace").exist())
			{
				int vSpace = element.attribute("vSpace").value();
				VSpace = UniversalUnit(vSpace, UniversalUnit::Dx);
			}
			Lines	= element.attribute("lines").value();
			DropCap = element.attribute("dropCap").value();
		}


		const XML::XNode TextFrameProperties::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX

// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ImageData.h"


namespace OOX
{
	namespace Logic
	{

		ImageData::ImageData()
		{
		}


		ImageData::~ImageData()
		{
		}


		ImageData::ImageData(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ImageData& ImageData::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ImageData::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			rId		= element.attribute("id").value().ToWString();
			Title = element.attribute("title").value();
		}


		const XML::XNode ImageData::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX
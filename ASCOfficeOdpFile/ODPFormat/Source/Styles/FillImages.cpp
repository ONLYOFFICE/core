#include "FillImages.h"
#include <algorithm>
#include <boost/bind.hpp>

namespace Odp
{
	namespace Styles
	{
		FillImages::FillImages()
		{
		}

		FillImages::~FillImages()
		{
		}

		FillImages::FillImages(const XML::XNode& node)
		{
			fromXML(node);
		}

		const FillImages& FillImages::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void FillImages::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "fill-image");
		}

		const bool FillImages::exist(const std::string& name) const
		{
			return std::find_if(m_items.begin(), m_items.end(), boost::bind(&FillImage::Name, _1) == name) != m_items.end();
		}

		const FillImage& FillImages::operator[](const std::string& name) const
		{
			return find(name);
		}

		FillImage& FillImages::operator[](const std::string& name)
		{
			return find(name);
		}

		const FillImage& FillImages::find(const std::string& name) const
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&FillImage::Name, _1) == name);
		}
		
		FillImage& FillImages::find(const std::string& name)
		{
			return *std::find_if(m_items.begin(), m_items.end(), boost::bind(&FillImage::Name, _1) == name);
		}
	} // namespace Styles
} // namespace Odp
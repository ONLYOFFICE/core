#pragma once
#ifndef COMMON_SIZE_INCLUDE_H_
#define COMMON_SIZE_INCLUDE_H_

#include "Unit.h"
#include "property.h"
#include "nullable_property.h"
#include "XML.h"


namespace Common
{
	template<class Unit>
	class Size
	{
	public:
		Size() {}

		template<typename HeightType, typename WidthType>
		Size(const HeightType& height, WidthType width)
			: Height(height), Width(width)
		{
		}

		template<typename OtherUnit>
		Size(const OtherUnit& rhs)
			: Height(rhs.Height), Width(rhs.Width)
		{
		}

		template<typename OtherUnit>
		const Size& operator =(const OtherUnit& rhs)
		{
			Height = ths.Height;
			Width  = ths.Width;
			return *this;
		}

	public:
		property<Unit>	Height;
		property<Unit>	Width;
	};
} // namespace Common


namespace XML
{
	template<class Unit>
	const Common::Size<Unit> XElement2Size(const XML::XElement& element, const std::string& height, const std::string& width)
	{
		Common::Size<Unit>	size;
		size.Height = element.attribute(height).value().ToString();
		size.Width	 = element.attribute(width).value().ToString();
		return size;
	}

	template<class Unit>
	const nullable<Common::Size<Unit> > XElement2NullableSize(const XML::XElement& element, const std::string& height, const std::string& width)
	{
		nullable<Common::Size<Unit> > size;
		if (element.attribute(height).exist() || element.attribute(width).exist())
			size.init();
		if (element.attribute(height).exist())
			size->Height = element.attribute(height).value().ToString();
		if (element.attribute(width).exist())
			size->Width  = element.attribute(width).value().ToString();
		return size;
	}
} // namespace XML

#endif // COMMON_SIZE_INCLUDE_H_
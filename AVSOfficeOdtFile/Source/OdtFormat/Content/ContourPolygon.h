#pragma once
#ifndef ODT_CONTENT_CONTOUR_POLYGON_INCLUDE_H_
#define ODT_CONTENT_CONTOUR_POLYGON_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "./../Unit.h"


namespace Odt
{
	namespace Content
	{
		class ContourPolygon : public WritingElement
		{
		public:
			ContourPolygon();
			virtual ~ContourPolygon();
			explicit ContourPolygon(const XML::XNode& node);
			const ContourPolygon& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			// TODO, Заменить на Size
			property<Unit<double, Cm, 3> > Width;
			property<Unit<double, Cm, 3> > Height;
			property<std::string>					ViewBox;
			property<bool>								Points;
			property<std::string>					RecreateOnEdit;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_CONTOUR_POLYGON_INCLUDE_H_

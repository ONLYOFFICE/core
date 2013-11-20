#pragma once
#ifndef OOX_SETTINGS_HDR_SHAPE_DEFAULTS_INCLUDE_H_
#define OOX_SETTINGS_HDR_SHAPE_DEFAULTS_INCLUDE_H_

#include "./../WritingElement.h"
#include "ShapeDefault.h"


namespace OOX
{
	namespace Settings
	{
		class HdrShapeDefaults : public WritingElement
		{
		public:
			HdrShapeDefaults();
			virtual ~HdrShapeDefaults();
			explicit HdrShapeDefaults(const XML::XNode& node);
			const HdrShapeDefaults& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			ShapeDefault	m_shapeDefault;
		};	
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_HDR_SHAPE_DEFAULTS_INCLUDE_H_
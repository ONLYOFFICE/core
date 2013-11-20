#pragma once
#ifndef OOX_SETTINGS_SHAPE_DEFAULTS_INCLUDE_H_
#define OOX_SETTINGS_SHAPE_DEFAULTS_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "ShapeLayout.h"
#include "ShapeDefault.h"


namespace OOX
{
	namespace Settings
	{
		class ShapeDefaults : public WritingElement
		{
		public:
			ShapeDefaults();
			virtual ~ShapeDefaults();
			explicit ShapeDefaults(const XML::XNode& node);
			const ShapeDefaults& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			ShapeDefault m_shapeDefault;
			ShapeLayout	 m_shapeLayout;
		};
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_SHAPE_DEFAULTS_INCLUDE_H_
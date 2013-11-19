#pragma once
#ifndef ODT_LOGIC_FONT_FACE_INCLUDE_H_
#define ODT_LOGIC_FONT_FACE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "./../Limit/FontName.h"
#include "./../Limit/FontFamilyGeneric.h"
#include "./../Limit/FontPitch.h"
#include "property.h"
#include "nullable_property.h"


namespace Odt
{
	namespace Logic
	{
		class FontFace : public WritingElement
		{
		public:
			FontFace();
			virtual ~FontFace();
			explicit FontFace(const XML::XNode& node);
			const FontFace& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> Name;
			property<std::string> Family;
			nullable_property<std::string, Limit::FontFamilyGeneric>	Generic;
			nullable_property<std::string, Limit::FontPitch>					Pitch;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_FONT_FACE_INCLUDE_H_
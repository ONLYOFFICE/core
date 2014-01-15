#pragma once
#ifndef OOX_LOGIC_GRAPHIC_INCLUDE_H_
#define OOX_LOGIC_GRAPHIC_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include "string.h"
#include "Pic.h"

namespace OOX
{
	namespace Logic
	{
		class Graphic : public WritingElement
		{
		public:
			Graphic();
			virtual ~Graphic();			
			explicit Graphic(const XML::XNode& node);
			const Graphic& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string>	Uri;
			property<Pic>					Pic;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_GRAPHIC_INCLUDE_H
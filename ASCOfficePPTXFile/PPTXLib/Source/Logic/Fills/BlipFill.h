#pragma once
#ifndef PPTX_LOGIC_BLIPFILL_INCLUDE_H_
#define PPTX_LOGIC_BLIPFILL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Rect.h"
#include "Blip.h"
#include "Tile.h"
#include "Stretch.h"

namespace PPTX
{
	namespace Logic
	{

		class BlipFill : public WrapperWritingElement
		{
		public:
			BlipFill();
			virtual ~BlipFill();
			explicit BlipFill(const XML::XNode& node);
			const BlipFill& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<Blip> blip;
			nullable_property<Rect> srcRect;
			nullable_property<Tile> tile;
			nullable_property<Stretch> stretch;

			nullable_property<int, setter::only_positive<int> > dpi;
			nullable_property<bool> rotWithShape;
		private:
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLIPFILL_INCLUDE_H_
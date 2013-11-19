#pragma once
#ifndef PPTX_LOGIC_TILE_INCLUDE_H_
#define PPTX_LOGIC_TILE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../../Limit/RectAlign.h"
#include "./../../Limit/Flip.h"

namespace PPTX
{
	namespace Logic
	{

		class Tile : public WrapperWritingElement
		{
		public:
			Tile();
			virtual ~Tile();
			explicit Tile(const XML::XNode& node);
			const Tile& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string, Limit::RectAlign> algn;
			nullable_property<std::string, Limit::Flip> flip;
			nullable_property<int> sx;
			nullable_property<int> sy;
			nullable_property<int> tx;
			nullable_property<int> ty;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TILE_INCLUDE_H_
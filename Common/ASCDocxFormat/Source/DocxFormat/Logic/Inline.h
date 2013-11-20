#pragma once
#ifndef OOX_LOGIC_INLINE_INCLUDE_H_
#define OOX_LOGIC_INLINE_INCLUDE_H_

#include "./../WritingElement.h"
#include "Extent.h"
#include "EffectExtent.h"
#include "DocPr.h"
#include "Graphic.h"
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/InlineName.h"
#include "DrawingWrap.h"
#include "Point2D.h"

namespace OOX
{
	namespace Logic
	{
		class Inline : public WritingElement
		{
		public:
			Inline();
			virtual ~Inline();
			explicit Inline(const XML::XNode& node);
			const Inline& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string, Limit::InlineName>	Name;

			nullable<Common::Point<UnitDx>>				distanceLeftTop;
			nullable<Common::Point<UnitDx>>				distanceRightBottom;

			nullable_property<double>					DistT;	
			nullable_property<double>					DistB;	
			nullable_property<double>					DistL;	
			nullable_property<double>					DistR;	
			nullable_property<Extent>					Extent;
			property<EffectExtent>						EffectExtent;
			nullable_property<DocPr>					DocPr;
			nullable_property<int>						NoChangeAspect;
			property<Graphic>							Graphic;
			
			// anchor only

			nullable<Common::Point<UnitSx>>				anchorXY;

			nullable_property<int>						positionV;
			nullable_property<std::string>				positionVRelativeFrom;
			nullable_property<std::string>				positionVAlign;
			nullable_property<int>						positionH;	
			nullable_property<std::string>				positionHRelativeFrom;
			nullable_property<std::string>				positionHAlign;

			nullable_property<DrawingWrap>				Wrap;

			// required in anchor

			nullable_property<bool>						SimplePos;
			nullable_property<Point2D>					SimplePosPoint;
			nullable_property<int>						RelativeHeight;
			nullable_property<bool>						BehindDoc;
			nullable_property<bool>						Locked;
			nullable_property<bool>						LayoutInCell;
			nullable_property<bool>						AllowOverlap;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_INLINE_INCLUDE_H
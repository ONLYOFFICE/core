#pragma once
#ifndef ODT_CONTENT_OLE_OBJECT_INCLUDE_H_
#define ODT_CONTNET_OLE_OBJECT_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/Anchortype.h"
#include "./../Unit.h"
#include "Image.h"
#include "ObjectOle.h"
#include "ContourPolygon.h"


namespace Odt
{
	namespace Content
	{
		class OleObject : public ParagraphItemBase
		{
		public:
			OleObject();
			virtual ~OleObject();
			explicit OleObject(const XML::XNode& node);
			const OleObject& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			property<std::string>												StyleName;
			property<std::string>												Name;
			property<std::string, Limit::Anchortype>		Anchortype;
			nullable_property<Unit<double, Cm, 3> >			X;
			nullable_property<Unit<double, Cm, 3> >			Y;
			property<Unit<double, Cm, 3> >							Width;
			property<Unit<double, Cm, 3> >							Height;
			property<int, setter::only_positive<int> >	ZIndex;
			property<Image>															Image;
			property<ObjectOle>													ObjectOle;
			nullable_property<ContourPolygon>						ContourPolygon;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTNET_OLE_OBJECT_INCLUDE_H_
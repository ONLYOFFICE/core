#pragma once
#ifndef OOX_LOGIC_PICTURE_INCLUDE_H_
#define OOX_LOGIC_PICTURE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "Common.h"
#include "property.h"
#include "nullable_property.h"
#include "./../RId.h"
#include "./../Unit.h"


namespace OOX
{
	namespace Logic
	{
		class Pic : public WritingElement
		{
		public:
			Pic();
			virtual ~Pic();			
			explicit Pic(const XML::XNode& node);
			const Pic& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<int>						Id;
			nullable_property<std::string>				Name;
			nullable_property<OOX::RId>					rId;
			nullable_property<Common::Point<UnitSx> >	Off;
			nullable_property<Common::Size<UnitSx> >	Ext;
			nullable_property<std::string>				Prst;
			nullable_property<std::string>				rot;
			nullable_property<std::string>				flipH;
			nullable_property<std::string>				flipV;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PICTURE_INCLUDE_H
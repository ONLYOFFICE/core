#pragma once
#ifndef OOX_LOGIC_DRAWING_INCLUDE_H_
#define OOX_LOGIC_DRAWING_INCLUDE_H_

#include "RunItemBase.h"
#include <string>
#include "Inline.h"
#include "property.h"
#include "./../RId.h"

namespace OOX
{
	namespace Logic
	{
		class Drawing : public RunItemBase
		{
		public:
			Drawing();
			virtual ~Drawing();
			Drawing(const RId& rId, const OOX::CPath& filename, const long width, const long height);
			Drawing(const RId& rId, const OOX::CPath& filename, const long xEmu, const std::string& hRelativeFrom, const long yEmu, const std::string& vRelativeFrom, const long widthEmu, const long heightEmu);
			explicit Drawing(const XML::XNode& node);
			const Drawing& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			const std::string toTxt() const;

		public:
			bool				haveAnchor;
			property<Inline>	Inline;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_INCLUDE_H_
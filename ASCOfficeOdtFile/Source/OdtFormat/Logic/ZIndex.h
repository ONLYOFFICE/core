#pragma once
#ifndef ODT_LOGIC_Z_INDEX_INCLUDE_H_
#define ODT_LOGIC_Z_INDEX_INCLUDE_H_

#include "Common.h"
#include "./../Namespaces.h"
#include "./../Unit.h"
#include "property.h"


namespace Odt
{
	namespace Logic
	{
		class ZIndex : public Common::ZIndex
		{
		public:
			ZIndex();
			ZIndex(const Common::ZIndex& zIndex);
			ZIndex(const XML::XElement& element);
			const XML::XNode toXML(const Namespaces& ns) const;

			const ZIndex& operator = (const Common::ZIndex& zIndex);
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_Z_INDEX_INCLUDE_H_
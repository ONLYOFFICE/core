#pragma once
#ifndef OOX_STYLES_GS_LST_INCLUDE_H_
#define OOX_STYLES_GS_LST_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "Gs.h"


namespace OOX
{
	namespace Theme
	{
		class GsLst : public WritingElement
		{
		public:
			GsLst();
			virtual ~GsLst();
			explicit GsLst(const XML::XNode& node);
			const GsLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::vector<Gs> m_gss;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_STYLES_GS_LST_INCLUDE_H_
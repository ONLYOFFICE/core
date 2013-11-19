#pragma once
#ifndef OOX_STYLES_LN_STYLE_LST_INCLUDE_H_
#define OOX_STYLES_LN_STYLE_LST_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "Ln.h"


namespace OOX
{
	namespace Theme
	{
		class LnStyleLst : public WritingElement
		{
		public:
			LnStyleLst();
			virtual ~LnStyleLst();
			explicit LnStyleLst(const XML::XNode& node);
			const LnStyleLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::vector<Ln> m_lns;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_STYLES_LN_STYLE_LST_INCLUDE_H_
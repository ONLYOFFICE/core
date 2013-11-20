#pragma once
#ifndef OOX_THEME_FILL_STYLE_LST_INCLUDE_H_
#define OOX_THEME_FILL_STYLE_LST_INCLUDE_H_

#include "./../WritingElement.h"
#include "SolidFill.h"
#include <vector>
#include "GradFill.h"


namespace OOX
{
	namespace Theme
	{
		class FillStyleLst : public WritingElement
		{
		public:
			FillStyleLst();
			virtual ~FillStyleLst();
			explicit FillStyleLst(const XML::XNode& node);
			const FillStyleLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			SolidFill m_solidFill;
			std::vector<GradFill> m_gradFills;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_FILL_STYLE_LST_INCLUDE_H_

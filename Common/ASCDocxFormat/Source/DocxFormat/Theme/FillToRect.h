#pragma once
#ifndef OOX_THEME_FILL_TO_RECT_INCLUDE_H_
#define OOX_THEME_FILL_TO_RECT_INCLUDE_H_

#include "./../WritingElement.h"
#include "nullable.h"


namespace OOX
{
	namespace Theme
	{
		class FillToRect : public WritingElement
		{
		public:
			FillToRect();
			virtual ~FillToRect();
			explicit FillToRect(const XML::XNode& node);
			const FillToRect& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			nullable<int>		m_l;
			nullable<int>		m_t;
			nullable<int>		m_r;
			nullable<int>		m_b;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_FILL_TO_RECT_INCLUDE_H_
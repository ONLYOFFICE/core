#pragma once
#ifndef OOX_THEME_OUTER_SHDW_INCLUDE_H_
#define OOX_THEME_OUTER_SHDW_INCLUDE_H_

#include "./../WritingElement.h"
#include "SrgbClr.h"


namespace OOX
{
	namespace Theme
	{
		class OuterShdw : public WritingElement
		{
		public:
			OuterShdw();
			virtual ~OuterShdw();
			explicit OuterShdw(const XML::XNode& node);
			const OuterShdw& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int		m_blurRad;
			int		m_dist;
			int		m_dir;
			int		m_rotWithShare;
			SrgbClr m_srgbClr;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_OUTER_SHDW_INCLUDE_H_
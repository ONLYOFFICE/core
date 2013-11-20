#pragma once
#ifndef OOX_THEME_SP3D_INCLUDE_H_
#define OOX_THEME_SP3D_INCLUDE_H_

#include "./../WritingElement.h"
#include "BevelT.h"


namespace OOX
{
	namespace Theme
	{
		class Sp3d : public WritingElement
		{
		public:
			Sp3d();
			virtual ~Sp3d();
			explicit Sp3d(const XML::XNode& node);
			const Sp3d& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			BevelT m_bevelT;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_SP3D_INCLUDE_H_
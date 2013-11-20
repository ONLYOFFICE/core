#pragma once
#ifndef OOX_STYLES_LIN_INCLUDE_H_
#define OOX_STYLES_LIN_INCLUDE_H_

#include "./../WritingElement.h"


namespace OOX
{
	namespace Theme
	{
		class Lin : public WritingElement
		{
		public:
			Lin();
			virtual ~Lin();
			explicit Lin(const XML::XNode& node);
			const Lin& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int m_ang;
			int m_scaled;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_STYLES_LIN_INCLUDE_H_
#pragma once
#ifndef OOX_SETTINGS_SHAPE_DEFAULT_INCLUDE_H_
#define OOX_SETTINGS_SHAPE_DEFAULT_INCLUDE_H_

#include "./../WritingElement.h"

#include "./../Unit.h"


namespace OOX
{
	namespace Settings
	{
		class ShapeDefault : public WritingElement
		{
		public:
			ShapeDefault();
			virtual ~ShapeDefault();
			explicit ShapeDefault(const XML::XNode& node);
			const ShapeDefault& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::string		m_ext;
			UnitDx			m_spidmax;
		};	
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_SHAPE_DEFAULT_INCLUDE_H_
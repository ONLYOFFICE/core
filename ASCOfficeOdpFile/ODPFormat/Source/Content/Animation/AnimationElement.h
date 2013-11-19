#pragma once
#ifndef ODP_CONTENT_ANIMATION_ELEMENT_INCLUDE_H_
#define ODP_CONTENT_ANIMATION_ELEMENT_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include <boost/shared_ptr.hpp>

namespace Odp
{
	namespace Content
	{
		class AnimationElement : public Odt::WritingElement
		{
		public:
			AnimationElement();
			virtual ~AnimationElement();
			explicit AnimationElement(const XML::XNode& node);
			const AnimationElement& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			template<class T> const bool is() const { return (m_item == 0)?false:(typeid(*m_item) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*m_item);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_item);}

		private:
			boost::shared_ptr<Odt::WritingElement> m_item;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_ANIMATION_ELEMENT_INCLUDE_H_
#pragma once
#ifndef ODP_CONTENT_DRAWINGELEMENT_INCLUDE_H_
#define ODP_CONTENT_DRAWINGELEMENT_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include <boost/shared_ptr.hpp>

namespace Odp
{
	namespace Content
	{
		class DrawingElement : public Odt::WritingElement
		{
		public:
			DrawingElement();
			virtual ~DrawingElement();
			explicit DrawingElement(const XML::XNode& node);
			const DrawingElement& operator =(const XML::XNode& node);
			virtual bool is_init() const {return (m_item != 0);}

			template<class T> const bool is() const { return (m_item == 0)?false:(typeid(*m_item) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*m_item);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_item);}
		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		private:
			boost::shared_ptr<Odt::WritingElement> m_item;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_DRAWINGELEMENT_INCLUDE_H_
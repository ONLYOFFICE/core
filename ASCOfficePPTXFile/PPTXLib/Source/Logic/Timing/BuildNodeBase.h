#pragma once
#ifndef PPTX_LOGIC_BUILDNODEBASE_INCLUDE_H_
#define PPTX_LOGIC_BUILDNODEBASE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include <boost/shared_ptr.hpp>

namespace PPTX
{
	namespace Logic
	{
		class BuildNodeBase : public WrapperWritingElement
		{
		public:
			BuildNodeBase();
			virtual ~BuildNodeBase();
			explicit BuildNodeBase(const XML::XNode& node);
			const BuildNodeBase& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void GetBuildNodeFrom(const XML::XElement& element);
			virtual bool is_init()const{return (m_node != 0);};
			
			template<class T> const bool is() const { return (node == 0)?false:(typeid(*m_node) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*m_node);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_node);}

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<WrapperWritingElement> m_node;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					m_node->SetParentPointer(pParent);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUILDNODEBASE_INCLUDE_H_
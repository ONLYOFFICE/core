#pragma once
#ifndef PPTX_LOGIC_BUILDNODEBASE_INCLUDE_H_
#define PPTX_LOGIC_BUILDNODEBASE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuildNodeBase : public WrapperWritingElement
		{
		public:
			BuildNodeBase();
			virtual ~BuildNodeBase();
			explicit BuildNodeBase(XmlUtils::CXmlNode& node);
			const BuildNodeBase& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void GetBuildNodeFrom(XmlUtils::CXmlNode& element);
			virtual bool is_init()const{return (m_node.IsInit());};
			
			template<class T> const bool is() const { return (!m_node.IsInit())?false:(typeid(*m_node) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*m_node);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_node);}

			virtual CString toXML() const;
		//public:
		private:
			smart_ptr<WrapperWritingElement> m_node;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					m_node->SetParentPointer(pParent);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUILDNODEBASE_INCLUDE_H_
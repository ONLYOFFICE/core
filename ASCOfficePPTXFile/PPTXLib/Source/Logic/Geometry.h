#pragma once
#ifndef PPTX_LOGIC_GEOMETRY_INCLUDE_H_
#define PPTX_LOGIC_GEOMETRY_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class Geometry : public WrapperWritingElement
		{
		public:
			Geometry();
			virtual ~Geometry();
			explicit Geometry(const XML::XNode& node);
			const Geometry& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual void GetGeometryFrom(const XML::XElement& element);
			virtual bool is_init() const {return (m_geometry != 0);};

			template<class T> const bool is() const { return (m_geometry == 0)?false:(typeid(*m_geometry) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*m_geometry);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_geometry);}
		//public:
		private:
			boost::shared_ptr<WrapperWritingElement> m_geometry;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					m_geometry->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GEOMETRY_INCLUDE_H_

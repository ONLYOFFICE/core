#pragma once
#ifndef PPTX_LOGIC_BULLETSIZE_INCLUDE_H_
#define PPTX_LOGIC_BULLETSIZE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include <boost/shared_ptr.hpp>
#include "BuSzTx.h"
#include "BuSzPct.h"
#include "BuSzPts.h"

namespace PPTX
{
	namespace Logic
	{
		class BulletSize : public WrapperWritingElement
		{
		public:
			BulletSize();
			virtual ~BulletSize();			
			explicit BulletSize(const XML::XNode& node);
			const BulletSize& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void ReadBulletSizeFrom(const XML::XElement& element);
			virtual bool is_init()const{return (m_Size != 0);};
			virtual bool has_spec_size()const{return ((is_init()) && (!is<BuSzTx>()));};
			
			template<class T> const bool is() const { return (m_Size == 0)?false:(typeid(*m_Size) == typeid(T));};
			template<class T> T& as() {return static_cast<T&>(*m_Size);};
			template<class T> const T& as() const {return static_cast<const T&>(*m_Size);};

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<WrapperWritingElement> m_Size;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					m_Size->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BULLETSIZE_INCLUDE_H
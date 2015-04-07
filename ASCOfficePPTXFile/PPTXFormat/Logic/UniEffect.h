#pragma once
#ifndef PPTX_LOGIC_UNIEFFECT_INCLUDE_H_
#define PPTX_LOGIC_UNIEFFECT_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class UniEffect : public WrapperWritingElement
		{
		public:
			UniEffect();
			virtual ~UniEffect();			
			explicit UniEffect(XmlUtils::CXmlNode& node);
			const UniEffect& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void GetEffectFrom(XmlUtils::CXmlNode& element);
			virtual bool is_init()const{return (Effect.IsInit());};
			
			template<class T> const bool is() const { return Effect.is<T>();}
			template<class T> T& as() {return Effect.as<T>();}
			template<class T> const T& as() const {return static_cast<const T&>(*Effect);}

			virtual CString toXML() const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (Effect.is_init())
					Effect->toPPTY(pWriter);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (Effect.is_init())
					Effect->toXmlWriter(pWriter);
			}

			void InitPointer(WrapperWritingElement* pElem)
			{
				Effect = pElem;
			}

		//public:
		private:
			smart_ptr<WrapperWritingElement> Effect;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					Effect->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_UNIEFFECT_INCLUDE_H